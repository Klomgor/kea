// Copyright (C) 2013-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <hooks/callout_manager.h>
#include <hooks/hooks_manager.h>
#include <hooks/library_manager.h>
#include <hooks/library_manager_collection.h>
#include <boost/range/adaptor/reversed.hpp>

namespace isc {
namespace hooks {

// Return callout manager for the loaded libraries.  This call is only valid
// after one has been created for the loaded libraries (which includes the
// case of no loaded libraries).
//
// Note that there is no real connection between the callout manager and the
// libraries, other than it knows the number of libraries so can do sanity
// checks on values passed to it.  However, this may change in the future,
// so the hooks framework is written such that a callout manager is used only
// with the LibraryManagerCollection that created it.  It is also the reason
// why each LibraryManager contains a pointer to this CalloutManager.

boost::shared_ptr<CalloutManager>
LibraryManagerCollection::getCalloutManager() const {

    // Only return a pointer if we have a CalloutManager created.
    if (!callout_manager_) {
        isc_throw(LoadLibrariesNotCalled, "must load hooks libraries before "
                  "attempting to retrieve a CalloutManager for them");
    }

    return (callout_manager_);
}

LibraryManagerCollection::LibraryManagerCollection(const HookLibsCollection& libraries)
    : library_info_(libraries) {

    // We need to split hook libs into library names and library parameters.
    for (auto const& it : libraries) {
        library_names_.push_back(it.libname_);
    }
}

// Load a set of libraries

bool
LibraryManagerCollection::loadLibraries(bool multi_threading_enabled) {

    // There must be no libraries still in memory.
    if (!lib_managers_.empty()) {
        isc_throw(LibrariesStillOpened, "some libraries are still opened");
    }

    // Access the callout manager, (re)creating it if required.
    //
    // A pointer to the callout manager is maintained by each as well as by
    // the HooksManager itself.  Note that the callout manager does not hold any
    // memory allocated by a library: although a library registers a callout
    // (and so causes the creation of an entry in the CalloutManager's callout
    // list), that creation is done by the CalloutManager itself.  The
    // CalloutManager is created within the server. The upshot of this is that
    // it is therefore safe for the CalloutManager to be deleted after all
    // associated libraries are deleted, hence this link (LibraryManager ->
    // CalloutManager) is safe.
    //
    // The call of this function will result in re-creating the callout manager.
    // This deletes all callouts (including the pre-library and post-
    // library) ones.  It is up to the libraries to re-register their callouts.
    // The pre-library and post-library callouts will also need to be
    // re-registered.
    callout_manager_.reset(new CalloutManager(library_names_.size()));

    // Now iterate through the libraries are load them one by one.  We'll
    for (size_t i = 0; i < library_names_.size(); ++i) {
        // Create a pointer to the new library manager.  The index of this
        // library is determined by the number of library managers currently
        // loaded: note that the library indexes run from 1 to (number of loaded
        // libraries).
        boost::shared_ptr<LibraryManager> manager(
                new LibraryManager(library_names_[i], lib_managers_.size() + 1,
                                   callout_manager_));

        // Load the library.  On success, add it to the list of loaded
        // libraries.  On failure, unload all currently loaded libraries,
        // leaving the object in the state it was in before loadLibraries was
        // called.
        if (manager->loadLibrary(multi_threading_enabled)) {
            lib_managers_.push_back(manager);
        } else {
            static_cast<void>(unloadLibraries());
            return (false);
        }
    }

    return (true);
}

// Unload the libraries.

void
LibraryManagerCollection::unloadLibraries() {

    // Delete the library managers in the reverse order to which they were
    // created, then clear the library manager vector.
    while (!lib_managers_.empty()) {
        lib_managers_.pop_back();
    }

    // Get rid of the callout manager. (The other member, the list of library
    // names, was cleared when the libraries were loaded.)
    callout_manager_.reset();
}

// Prepare the unloading of libraries.
bool
LibraryManagerCollection::prepareUnloadLibraries() {
    bool result = true;
    // Iterate on library managers in reverse order.
    for (auto const& lm : boost::adaptors::reverse(lib_managers_)) {
        result = lm->prepareUnloadLibrary() && result;
    }
    return (result);
}

// Return number of loaded libraries.
int
LibraryManagerCollection::getLoadedLibraryCount() const {
    return (lib_managers_.size());
}

// Validate the libraries.
std::vector<std::string>
LibraryManagerCollection::validateLibraries(const std::vector<std::string>& libraries,
                                            bool multi_threading_enabled) {

    std::vector<std::string> failures;
    for (size_t i = 0; i < libraries.size(); ++i) {
        if (!LibraryManager::validateLibrary(libraries[i], multi_threading_enabled)) {
            failures.push_back(libraries[i]);
        }
    }

    return (failures);
}

} // namespace hooks
} // namespace isc
