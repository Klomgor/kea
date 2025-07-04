// Copyright (C) 2015-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <exceptions/exceptions.h>
#include <dhcp/opaque_data_tuple.h>
#include <dhcp/option_opaque_data_tuples.h>
#include <sstream>

namespace isc {
namespace dhcp {

OptionOpaqueDataTuples::OptionOpaqueDataTuples(Option::Universe u,
                                               const uint16_t type,
                                               OpaqueDataTuple::LengthFieldType length_field_type)
    : Option(u, type), length_field_type_(length_field_type) {
    if (length_field_type_ == OpaqueDataTuple::LENGTH_EMPTY) {
        length_field_type_ = OptionDataTypeUtil::getTupleLenFieldType(u);
    }
}

OptionOpaqueDataTuples::OptionOpaqueDataTuples(Option::Universe u,
                                               const uint16_t type,
                                               OptionBufferConstIter begin,
                                               OptionBufferConstIter end,
                                               OpaqueDataTuple::LengthFieldType length_field_type)
    : Option(u, type), length_field_type_(length_field_type) {
    if (length_field_type_ == OpaqueDataTuple::LENGTH_EMPTY) {
        length_field_type_ = OptionDataTypeUtil::getTupleLenFieldType(u);
    }
    unpack(begin, end);
}

OptionPtr
OptionOpaqueDataTuples::clone() const {
    return (cloneInternal<OptionOpaqueDataTuples>());
}

void
OptionOpaqueDataTuples::pack(isc::util::OutputBuffer& buf, bool check) const {
    packHeader(buf, check);

    for (auto const& it : tuples_) {
        it.pack(buf);
    }
    // That's it. We don't pack any sub-options here, because this option
    // must not contain sub-options.
}

void
OptionOpaqueDataTuples::unpack(OptionBufferConstIter begin,
                               OptionBufferConstIter end) {
    // We are skipping typical OutOfRange check for Option#unpack(begin, end),
    // since empty collection of tuples is also a valid case where
    // std::distance(begin, end) = 0

    // Start reading opaque data.
    size_t offset = 0;
    while (offset < static_cast<size_t>(std::distance(begin, end))) {
        // Parse a tuple.
        OpaqueDataTuple tuple(length_field_type_, begin + offset, end);
        addTuple(tuple);
        // The tuple has been parsed correctly which implies that it is safe to
        // advance the offset by its total length.
        offset += tuple.getTotalLength();
    }
}

void
OptionOpaqueDataTuples::addTuple(const OpaqueDataTuple& tuple) {
    if (tuple.getLengthFieldType() != length_field_type_) {
        isc_throw(isc::BadValue, "attempted to add opaque data tuple having"
                  " invalid size of the length field "
                  << tuple.getDataFieldSize() << " to opaque data tuple option");
    }

    tuples_.push_back(tuple);
}


void
OptionOpaqueDataTuples::setTuple(const size_t at, const OpaqueDataTuple& tuple) {
    if (at >= getTuplesNum()) {
        isc_throw(isc::OutOfRange, "attempted to set an opaque data for the"
                  " opaque data tuple option at position " << at << " which"
                  " is out of range");

    } else if (tuple.getLengthFieldType() != length_field_type_) {
        isc_throw(isc::BadValue, "attempted to set opaque data tuple having"
                  " invalid size of the length field "
                  << tuple.getDataFieldSize() << " to opaque data tuple option");
    }

    tuples_[at] = tuple;
}

OpaqueDataTuple
OptionOpaqueDataTuples::getTuple(const size_t at) const {
    if (at >= getTuplesNum()) {
        isc_throw(isc::OutOfRange, "attempted to get an opaque data for the"
                  " opaque data tuple option at position " << at << " which is"
                  " out of range. There are only " << getTuplesNum() << " tuples");
    }
    return (tuples_[at]);
}

bool
OptionOpaqueDataTuples::hasTuple(const std::string& tuple_str) const {
    // Iterate over existing tuples (there shouldn't be many of them),
    // and try to match the searched one.
    for (auto const& it : tuples_) {
        if (it == tuple_str) {
            return (true);
        }
    }
    return (false);
}

uint16_t
OptionOpaqueDataTuples::len() const {
    // The option starts with the header.
    uint16_t length = getHeaderLen();
    // Now iterate over existing tuples and add their size.
    for (auto const& it : tuples_) {
        length += it.getTotalLength();
    }

    return (length);
}

std::string
OptionOpaqueDataTuples::toText(int indent) const {
    std::ostringstream s;

    // Apply indentation
    s << std::string(indent, ' ');

    // Print type and length
    s << "type=" << getType() << ", len=" << len() - getHeaderLen() << std::dec;
    // Iterate over all tuples and print their size and contents.
    for (unsigned i = 0; i < getTuplesNum(); ++i) {
        // Print the tuple.
        s << ", data-len" << i << "=" << getTuple(i).getLength();
        s << ", data" << i << "='" << getTuple(i) << "'";
    }

    return (s.str());
}

} // namespace isc::dhcp
} // namespace isc
