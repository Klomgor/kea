// Copyright (C) 2012-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <asiolink/io_address.h>
#include <asiolink/addr_utilities.h>
#include <dhcpsrv/pool.h>
#include <boost/make_shared.hpp>
#include <sstream>

using namespace isc::asiolink;
using namespace isc::data;

namespace isc {
namespace dhcp {

Pool::Pool(Lease::Type type, const isc::asiolink::IOAddress& first,
           const isc::asiolink::IOAddress& last)
    : id_(0), first_(first), last_(last), type_(type), capacity_(0),
      cfg_option_(new CfgOption()) {
}

bool
Pool::inRange(const isc::asiolink::IOAddress& addr) const {
    return (first_ <= addr && addr <= last_);
}

bool
Pool::clientSupported(const ClientClasses& classes) const {
    return (client_classes_.empty() || client_classes_.intersects(classes));
}

void
Pool::allowClientClass(const ClientClass& class_name) {
    if (!client_classes_.contains(class_name)) {
        client_classes_.insert(class_name);
    }
}

void
Pool::addAdditionalClass(const isc::dhcp::ClientClass& class_name) {
    if (!additional_classes_.contains(class_name)) {
        additional_classes_.insert(class_name);
    }
}

std::string
Pool::toText() const {
    std::stringstream tmp;
    tmp << "type=" << Lease::typeToText(type_) << ", " << first_
        << "-" << last_;
    return (tmp.str());
}

bool
Pool::hasDdnsParameters() const {
    return (!(ddns_send_updates_.unspecified() &&
              ddns_override_no_update_.unspecified() &&
              ddns_override_client_update_.unspecified() &&
              ddns_replace_client_name_mode_.unspecified() &&
              ddns_generated_prefix_.unspecified() &&
              ddns_qualifying_suffix_.unspecified() &&
              ddns_update_on_renew_.unspecified() &&
              ddns_conflict_resolution_mode_.unspecified() &&
              ddns_ttl_percent_.unspecified() &&
              ddns_ttl_.unspecified() &&
              ddns_ttl_min_.unspecified() &&
              ddns_ttl_max_.unspecified() &&
              hostname_char_set_.unspecified() &&
              hostname_char_replacement_.unspecified()));
}

Pool4::Pool4(const isc::asiolink::IOAddress& first,
             const isc::asiolink::IOAddress& last)
    : Pool(Lease::TYPE_V4, first, last) {
    // check if specified address boundaries are sane
    if (!first.isV4() || !last.isV4()) {
        isc_throw(BadValue, "Invalid Pool4 address boundaries: not IPv4");
    }

    if (last < first) {
        isc_throw(BadValue, "Upper boundary is smaller than lower boundary.");
    }

    // This is IPv4 pool, which only has one type. We can calculate
    // the number of theoretically possible leases in it. As there's 2^32
    // possible IPv4 addresses, we'll be able to accurately store that
    // info.
    capacity_ = addrsInRange(first, last);
}

Pool4::Pool4(const isc::asiolink::IOAddress& prefix, uint8_t prefix_len)
    : Pool(Lease::TYPE_V4, prefix, IOAddress("0.0.0.0")) {

    // check if the prefix is sane
    if (!prefix.isV4()) {
        isc_throw(BadValue, "Invalid Pool4 address boundaries: not IPv4");
    }

    // check if the prefix length is sane
    if (prefix_len == 0 || prefix_len > 32) {
        isc_throw(BadValue, "Invalid prefix length");
    }

    IOAddress first_address = firstAddrInPrefix(prefix, prefix_len);
    if (first_address != prefix) {
        isc_throw(BadValue, "Invalid Pool4 address boundaries: " << prefix
                  << " is not the first address in prefix: " << first_address
                  << "/" << static_cast<uint32_t>(prefix_len));
    }

    // Let's now calculate the last address in defined pool
    last_ = lastAddrInPrefix(prefix, prefix_len);

    // This is IPv4 pool, which only has one type. We can calculate
    // the number of theoretically possible leases in it. As there's 2^32
    // possible IPv4 addresses, we'll be able to accurately store that
    // info.
    capacity_ = addrsInRange(prefix, last_);
}

Pool4Ptr
Pool4::create(const IOAddress& first, const IOAddress& last) {
    return (boost::make_shared<Pool4>(first, last));
}

Pool4Ptr
Pool4::create(const IOAddress& prefix, uint8_t prefix_len) {
    return (boost::make_shared<Pool4>(prefix, prefix_len));
}

data::ElementPtr
Pool::toElement() const {
    // Prepare the map
    ElementPtr map = Element::createMap();

    // Set user-context
    contextToElement(map);

    // Set pool options
    ConstCfgOptionPtr opts = getCfgOption();
    map->set("option-data", opts->toElement());

    // Set client-classes
    if (!client_classes_.empty()) {
        map->set("client-classes", client_classes_.toElement());
    }

    // Set evaluate-additional-classes
    if (!additional_classes_.empty()) {
        map->set("evaluate-additional-classes",
                 additional_classes_.toElement());
    }

    if (id_) {
        map->set("pool-id", Element::create(static_cast<long long>(id_)));
    }

    // Add in DDNS parameters for non-prefix pools.
    if (type_ != Lease::TYPE_PD) {
        if (!ddns_send_updates_.unspecified()) {
            map->set("ddns-send-updates", Element::create(ddns_send_updates_));
        }

        if (!ddns_override_no_update_.unspecified()) {
            map->set("ddns-override-no-update", Element::create(ddns_override_no_update_));
        }

        if (!ddns_override_client_update_.unspecified()) {
            map->set("ddns-override-client-update", Element::create(ddns_override_client_update_));
        }

        if (!ddns_replace_client_name_mode_.unspecified()) {
            map->set("ddns-replace-client-name",
                      Element::create(D2ClientConfig::
                                      replaceClientNameModeToString(ddns_replace_client_name_mode_)));
        }

        if (!ddns_generated_prefix_.unspecified()) {
            map->set("ddns-generated-prefix", Element::create(ddns_generated_prefix_));
        }

        if (!ddns_qualifying_suffix_.unspecified()) {
            map->set("ddns-qualifying-suffix", Element::create(ddns_qualifying_suffix_));
        }

        if (!ddns_update_on_renew_.unspecified()) {
            map->set("ddns-update-on-renew", Element::create(ddns_update_on_renew_));
        }

        if (!ddns_conflict_resolution_mode_.unspecified()) {
            map->set("ddns-conflict-resolution-mode", Element::create(ddns_conflict_resolution_mode_));
        }

        if (!ddns_ttl_percent_.unspecified()) {
            map->set("ddns-ttl-percent", Element::create(ddns_ttl_percent_));
        }

        if (!ddns_ttl_.unspecified()) {
            map->set("ddns-ttl", Element::create(ddns_ttl_));
        }

        if (!ddns_ttl_min_.unspecified()) {
            map->set("ddns-ttl-min", Element::create(ddns_ttl_min_));
        }

        if (!ddns_ttl_max_.unspecified()) {
                map->set("ddns-ttl-max", Element::create(ddns_ttl_max_));
        }

        if (!hostname_char_set_.unspecified()) {
            map->set("hostname-char-set", Element::create(hostname_char_set_));
        }

        if (!hostname_char_replacement_.unspecified()) {
            map->set("hostname-char-replacement", Element::create(hostname_char_replacement_));
        }
    }

    return (map);
}

data::ElementPtr
Pool4::toElement() const {
    // Prepare the map
    ElementPtr map = Pool::toElement();

    // Set pool
    const IOAddress& first = getFirstAddress();
    const IOAddress& last = getLastAddress();
    std::string range = first.toText() + "-" + last.toText();

    // Try to output a prefix (vs a range)
    int prefix_len = prefixLengthFromRange(first, last);
    if (prefix_len >= 0) {
        std::ostringstream oss;
        oss << first.toText() << "/" << prefix_len;
        range = oss.str();
    }

    map->set("pool", Element::create(range));
    return (map);
}

Pool6::Pool6(Lease::Type type, const isc::asiolink::IOAddress& first,
             const isc::asiolink::IOAddress& last)
    : Pool(type, first, last), prefix_len_(128), pd_exclude_option_() {

    // check if specified address boundaries are sane
    if (!first.isV6() || !last.isV6()) {
        isc_throw(BadValue, "Invalid Pool6 address boundaries: not IPv6");
    }

    if ((type != Lease::TYPE_NA) && (type != Lease::TYPE_TA) &&
        (type != Lease::TYPE_PD)) {
        isc_throw(BadValue, "Invalid Pool6 type: " << static_cast<int>(type)
                  << ", must be TYPE_IA, TYPE_TA or TYPE_PD");
    }

    if (last < first) {
        isc_throw(BadValue, "Upper boundary is smaller than lower boundary.");
        // This check is a bit strict. If we decide that it is too strict,
        // we need to comment it and uncomment lines below.
        // On one hand, letting the user specify 2001::f - 2001::1 is nice, but
        // on the other hand, 2001::1 may be a typo and the user really meant
        // 2001::1:0 (or 1 followed by some hex digit), so a at least a warning
        // would be useful.

        // first_  = last;
        // last_ = first;
    }

    // TYPE_PD is not supported by this constructor. first-last style
    // parameters are for IA and TA only. There is another dedicated
    // constructor for that (it uses prefix/length)
    if ((type != Lease::TYPE_NA) && (type != Lease::TYPE_TA)) {
        isc_throw(BadValue, "Invalid Pool6 type specified: "
                  << static_cast<int>(type));
    }

    // Let's calculate the theoretical number of leases in this pool.
    // If the pool is extremely large (i.e. contains more than 2^64 addresses,
    // we'll just cap it at max value of uint64_t).
    capacity_ = addrsInRange(first, last);
}

Pool6::Pool6(Lease::Type type, const isc::asiolink::IOAddress& prefix,
             const uint8_t prefix_len, const uint8_t delegated_len /* = 128 */)
    : Pool(type, prefix, IOAddress::IPV6_ZERO_ADDRESS()),
      prefix_len_(delegated_len), pd_exclude_option_() {

    init(type, prefix, prefix_len, delegated_len,
         IOAddress::IPV6_ZERO_ADDRESS(), 0);
}

Pool6::Pool6(const asiolink::IOAddress& prefix, const uint8_t prefix_len,
             const uint8_t delegated_len,
             const asiolink::IOAddress& excluded_prefix,
             const uint8_t excluded_prefix_len)
    : Pool(Lease::TYPE_PD, prefix, IOAddress::IPV6_ZERO_ADDRESS()),
      prefix_len_(delegated_len), pd_exclude_option_() {

    init(Lease::TYPE_PD, prefix, prefix_len, delegated_len, excluded_prefix,
         excluded_prefix_len);

    // The excluded prefix can only be specified using this constructor.
    // Therefore, the initialization of the excluded prefix is takes place
    // here, rather than in the init(...) function.
    if (!excluded_prefix.isV6()) {
        isc_throw(BadValue, "excluded prefix must be an IPv6 prefix");
    }

    // An "unspecified" prefix should have both value and length equal to 0.
    if ((excluded_prefix.isV6Zero() && (excluded_prefix_len != 0)) ||
        (!excluded_prefix.isV6Zero() && (excluded_prefix_len == 0))) {
        isc_throw(BadValue, "invalid excluded prefix "
                  << excluded_prefix << "/"
                  << static_cast<unsigned>(excluded_prefix_len));
    }

    // If excluded prefix has been specified.
    if (!excluded_prefix.isV6Zero() && (excluded_prefix_len != 0)) {
        // Excluded prefix length must not be greater than 128.
        if (excluded_prefix_len > 128) {
            isc_throw(BadValue, "excluded prefix length "
                      << static_cast<unsigned>(excluded_prefix_len)
                      << " must not be greater than 128");
        }

        // Excluded prefix must be a sub-prefix of a delegated prefix. First
        // check the prefix length as it is less involved.
        if (excluded_prefix_len <= prefix_len_) {
            isc_throw(BadValue, "excluded prefix length "
                      << static_cast<unsigned>(excluded_prefix_len)
                      << " must be longer than the delegated prefix length "
                      << static_cast<unsigned>(prefix_len_));
        }

        /// @todo Check that the prefixes actually match. Theoretically, a
        /// user could specify a prefix which sets insignificant bits. We should
        /// clear insignificant bits based on the prefix length but this
        /// should be considered a part of the IOAddress class, perhaps and
        /// requires a bit of work (mainly in terms of testing).
    }
}

Pool6Ptr
Pool6::create(Lease::Type type, const IOAddress& first, const IOAddress& last) {
    return (boost::make_shared<Pool6>(type, first, last));
}

Pool6Ptr
Pool6::create(Lease::Type type, const IOAddress& prefix,
              uint8_t prefix_len, uint8_t delegated_len) {
    return (boost::make_shared<Pool6>(type, prefix, prefix_len, delegated_len));
}

Pool6Ptr
Pool6::create(const IOAddress& prefix, const uint8_t prefix_len,
              const uint8_t delegated_len, const IOAddress& excluded_prefix,
              const uint8_t excluded_prefix_len) {
    return (boost::make_shared<Pool6>(prefix, prefix_len,
                                      delegated_len, excluded_prefix,
                                      excluded_prefix_len));
}

void
Pool6::init(const Lease::Type& type,
            const asiolink::IOAddress& prefix,
            const uint8_t prefix_len,
            const uint8_t delegated_len,
            const asiolink::IOAddress& excluded_prefix,
            const uint8_t excluded_prefix_len) {
    // Check if the prefix is sane
    if (!prefix.isV6()) {
        isc_throw(BadValue, "Invalid Pool6 address boundaries: not IPv6");
    }

    // Check if the prefix length is sane
    if (prefix_len == 0 || prefix_len > 128) {
        isc_throw(BadValue, "Invalid prefix length: "
                  << static_cast<unsigned>(prefix_len));
    }

    if (prefix_len > delegated_len) {
        isc_throw(BadValue, "Delegated length ("
                  << static_cast<int>(delegated_len)
                  << ") must be longer than or equal to prefix length ("
                  << static_cast<int>(prefix_len) << ")");
    }

    if ((type != Lease::TYPE_PD) && (delegated_len != 128)) {
        isc_throw(BadValue, "For NA or TA pools, delegated prefix length must"
                  << " be 128.");
    }

    // excluded_prefix_len == 0 means there's no excluded prefix at all.
    if (excluded_prefix_len && (excluded_prefix_len <= delegated_len)) {
        isc_throw(BadValue, "Excluded prefix ("
                  << static_cast<int>(excluded_prefix_len)
                  << ") must be longer than the delegated prefix length ("
                  << static_cast<int>(delegated_len) << ")");
    }

    if (prefix_len != 128) {
        IOAddress first_address = firstAddrInPrefix(prefix, prefix_len);
        if (first_address != prefix) {
            isc_throw(BadValue, "Invalid Pool6 address boundaries: " << prefix
                      << " is not the first address in prefix: " << first_address
                      << "/" << static_cast<uint32_t>(prefix_len));
        }
    }

    /// @todo: We should probably implement checks against weird addresses
    /// here, like ::, starting with fe80, starting with ff etc. .

    // Let's now calculate the last address in defined pool
    last_ = lastAddrInPrefix(prefix, prefix_len);

    // Let's calculate the theoretical number of leases in this pool.
    // For addresses, we could use addrsInRange(prefix, last_), but it's
    // much faster to do calculations on prefix lengths.
    capacity_ = prefixesInRange(prefix_len, delegated_len);

    // If user specified an excluded prefix, create an option that will
    // be sent to clients obtaining prefixes from this pool.
    if (excluded_prefix_len > 0) {
        pd_exclude_option_.reset(new Option6PDExclude(prefix, delegated_len,
                                                      excluded_prefix,
                                                      excluded_prefix_len));
    }
}

data::ElementPtr
Pool6::toElement() const {
    // Prepare the map
    ElementPtr map = Pool::toElement();

    switch (getType()) {
        case Lease::TYPE_NA: {
            const IOAddress& first = getFirstAddress();
            const IOAddress& last = getLastAddress();
            std::string range = first.toText() + "-" + last.toText();

            // Try to output a prefix (vs a range)
            int prefix_len = prefixLengthFromRange(first, last);
            if (prefix_len >= 0) {
                std::ostringstream oss;
                oss << first.toText() << "/" << prefix_len;
                range = oss.str();
            }

            map->set("pool", Element::create(range));
            break;
        }
        case Lease::TYPE_PD: {
            // Set prefix
            const IOAddress& prefix = getFirstAddress();
            map->set("prefix", Element::create(prefix.toText()));

            // Set prefix-len (get it from min - max)
            const IOAddress& last = getLastAddress();
            int prefix_len = prefixLengthFromRange(prefix, last);
            if (prefix_len < 0) {
                // The pool is bad: give up
                isc_throw(ToElementError, "invalid prefix range "
                          << prefix.toText() << "-" << last.toText());
            }
            map->set("prefix-len", Element::create(prefix_len));

            // Set delegated-len
            uint8_t len = getLength();
            map->set("delegated-len", Element::create(static_cast<int>(len)));

            // Set excluded prefix
            const Option6PDExcludePtr& xopt = getPrefixExcludeOption();
            if (xopt) {
                const IOAddress& xprefix = xopt->getExcludedPrefix(prefix, len);
                map->set("excluded-prefix", Element::create(xprefix.toText()));

                uint8_t xlen = xopt->getExcludedPrefixLength();
                map->set("excluded-prefix-len",
                         Element::create(static_cast<int>(xlen)));
            }
            // Let's not insert empty excluded-prefix values. If we ever
            // decide to insert it after all, here's the code to do it:
            // else {
            //    map->set("excluded-prefix",
            //             Element::create(std::string("::")));
            //    map->set("excluded-prefix-len", Element::create(0));
            /// }

            break;
        }
        default:
            isc_throw(ToElementError, "Lease type: " << getType()
                      << ", unsupported for Pool6");
            break;
    }

    return (map);
}

std::string
Pool6::toText() const {
    std::ostringstream s;
    s << "type=" << Lease::typeToText(type_) << ", " << first_
      << "-" << last_ << ", delegated_len="
      << static_cast<unsigned>(prefix_len_);

    if (pd_exclude_option_) {
       s << ", excluded_prefix="
         << pd_exclude_option_->getExcludedPrefix(first_, prefix_len_).toText()
         << "/"
         << static_cast<unsigned>(pd_exclude_option_->getExcludedPrefixLength());
    }
    return (s.str());
}

}  // namespace dhcp
}  // namespace isc
