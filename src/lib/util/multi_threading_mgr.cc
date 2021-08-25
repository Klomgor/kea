// Copyright (C) 2019-2021 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <util/multi_threading_mgr.h>

namespace isc {
namespace util {

MultiThreadingMgr::MultiThreadingMgr()
    : enabled_(false), critical_section_count_(0), thread_pool_size_(0) {
}

MultiThreadingMgr::~MultiThreadingMgr() {
}

MultiThreadingMgr&
MultiThreadingMgr::instance() {
    static MultiThreadingMgr manager;
    return (manager);
}

bool
MultiThreadingMgr::getMode() const {
    return (enabled_);
}

void
MultiThreadingMgr::setMode(bool enabled) {
    enabled_ = enabled;
}

void
MultiThreadingMgr::enterCriticalSection() {
    checkCallbacksPermissions();
    std::lock_guard<std::mutex> lk(mutex_);
    stopProcessing();
    ++critical_section_count_;
}

void
MultiThreadingMgr::exitCriticalSection() {
    std::lock_guard<std::mutex> lk(mutex_);
    if (critical_section_count_ == 0) {
        isc_throw(InvalidOperation, "invalid negative value for override");
    }
    --critical_section_count_;
    startProcessing();
}

bool
MultiThreadingMgr::isInCriticalSection() {
    checkCallbacksPermissions();
    std::lock_guard<std::mutex> lk(mutex_);
    return (isInCriticalSectionInternal());
}

bool
MultiThreadingMgr::isInCriticalSectionInternal() {
    return (critical_section_count_ != 0);
}

ThreadPool<std::function<void()>>&
MultiThreadingMgr::getThreadPool() {
    return thread_pool_;
}

uint32_t
MultiThreadingMgr::getThreadPoolSize() const {
    return (thread_pool_size_);
}

void
MultiThreadingMgr::setThreadPoolSize(uint32_t size) {
    thread_pool_size_ = size;
}

uint32_t
MultiThreadingMgr::getPacketQueueSize() {
    return (thread_pool_.getMaxQueueSize());
}

void
MultiThreadingMgr::setPacketQueueSize(uint32_t size) {
    thread_pool_.setMaxQueueSize(size);
}

uint32_t
MultiThreadingMgr::detectThreadCount() {
    return (std::thread::hardware_concurrency());
}

void
MultiThreadingMgr::apply(bool enabled, uint32_t thread_count, uint32_t queue_size) {
    // check the enabled flag
    if (enabled) {
        // check for auto scaling (enabled flag true but thread_count 0)
        if (!thread_count) {
            // might also return 0
            thread_count = MultiThreadingMgr::detectThreadCount();
        }
    } else {
        thread_count = 0;
        queue_size = 0;
    }
    // check enabled flag and explicit number of threads or system supports
    // hardware concurrency
    if (thread_count) {
        if (thread_pool_.size()) {
            thread_pool_.stop();
        }
        setThreadPoolSize(thread_count);
        setPacketQueueSize(queue_size);
        setMode(true);
        if (!isInCriticalSection()) {
            thread_pool_.start(thread_count);
        }
    } else {
        removeAllCriticalSectionCallbacks();
        thread_pool_.reset();
        setMode(false);
        setThreadPoolSize(thread_count);
        setPacketQueueSize(queue_size);
    }
}

void
MultiThreadingMgr::checkCallbacksPermissions() {
    if (getMode()) {
        for (const auto& cb : cs_callbacks_.getCallbackSets()) {
            try {
                (cb.check_cb_)();
            } catch (const isc::MultiThreadingInvalidOperation& ex) {
                // If any registered callback throws, the exception needs to be
                // propagated to the caller of the
                // @ref MultiThreadingCriticalSection constructor.
                // Because this function is called by the
                // @ref MultiThreadingCriticalSection constructor, throwing here
                // is safe.
                throw;
            } catch (...) {
                // We can't log it and throwing could be chaos.
                // We'll swallow it and tell people their callbacks
                // must be exception-proof
            }
        }
    }
}

void
MultiThreadingMgr::callEntryCallbacks() {
    if (getMode()) {
        const auto& callbacks = cs_callbacks_.getCallbackSets();
        for (auto cb_it = callbacks.begin(); cb_it != callbacks.end(); cb_it++) {
            try {
                (cb_it->entry_cb_)();
            } catch (...) {
                // We can't log it and throwing could be chaos.
                // We'll swallow it and tell people their callbacks
                // must be exception-proof
            }
        }
    }
}

void
MultiThreadingMgr::callExitCallbacks() {
    if (getMode()) {
        const auto& callbacks = cs_callbacks_.getCallbackSets();
        for (auto cb_it = callbacks.rbegin(); cb_it != callbacks.rend(); cb_it++) {
            try {
                (cb_it->exit_cb_)();
            } catch (...) {
                // We can't log it and throwing could be chaos.
                // We'll swallow it and tell people their callbacks
                // must be exception-proof
                // Because this function is called by the
                // @ref MultiThreadingCriticalSection destructor, throwing here
                // is not safe and will cause the process to crash.
            }
        }
    }
}

void
MultiThreadingMgr::stopProcessing() {
    if (getMode() && !isInCriticalSectionInternal()) {
        if (getThreadPoolSize()) {
            thread_pool_.stop();
        }
        callEntryCallbacks();
    }
}

void
MultiThreadingMgr::startProcessing() {
    if (getMode() && !isInCriticalSectionInternal()) {
        if (getThreadPoolSize()) {
            thread_pool_.start(getThreadPoolSize());
        }
        callExitCallbacks();
    }
}

void
MultiThreadingMgr::addCriticalSectionCallbacks(const std::string& name,
                                               const CSCallbackSet::Callback& check_cb,
                                               const CSCallbackSet::Callback& entry_cb,
                                               const CSCallbackSet::Callback& exit_cb) {
    cs_callbacks_.addCallbackSet(name, check_cb, entry_cb, exit_cb);
}

void
MultiThreadingMgr::removeCriticalSectionCallbacks(const std::string& name) {
    cs_callbacks_.removeCallbackSet(name);
}

void
MultiThreadingMgr::removeAllCriticalSectionCallbacks() {
    cs_callbacks_.removeAll();
}

MultiThreadingCriticalSection::MultiThreadingCriticalSection() {
    MultiThreadingMgr::instance().enterCriticalSection();
}

MultiThreadingCriticalSection::~MultiThreadingCriticalSection() {
    MultiThreadingMgr::instance().exitCriticalSection();
}

void
CSCallbackSetList::addCallbackSet(const std::string& name,
                                  const CSCallbackSet::Callback& check_cb,
                                  const CSCallbackSet::Callback& entry_cb,
                                  const CSCallbackSet::Callback& exit_cb) {
    if (name.empty()) {
        isc_throw(BadValue, "CSCallbackSetList - name cannot be empty");
    }

    if (!check_cb) {
        isc_throw(BadValue, "CSCallbackSetList - check callback for " << name
                  << " cannot be empty");
    }

    if (!entry_cb) {
        isc_throw(BadValue, "CSCallbackSetList - entry callback for " << name
                  << " cannot be empty");
    }

    if (!exit_cb) {
        isc_throw(BadValue, "CSCallbackSetList - exit callback for " << name
                  << " cannot be empty");
    }

    for (auto const& callback : cb_sets_) {
        if (callback.name_ == name) {
            isc_throw(BadValue, "CSCallbackSetList - callbacks for " << name
                      << " already exist");
        }
    }

    cb_sets_.push_back(CSCallbackSet(name, check_cb, entry_cb, exit_cb));
}

void
CSCallbackSetList::removeCallbackSet(const std::string& name) {
    for (auto it = cb_sets_.begin(); it != cb_sets_.end(); ++it) {
        if ((*it).name_ == name) {
            cb_sets_.erase(it);
            break;
        }
    }
}

void
CSCallbackSetList::removeAll() {
    cb_sets_.clear();
}

const std::list<CSCallbackSet>&
CSCallbackSetList::getCallbackSets() {
    return (cb_sets_);
}

}  // namespace util
}  // namespace isc
