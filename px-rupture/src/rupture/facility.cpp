// name: core.cpp
// desc: c++
// auth: is0urce
// type: class methods "implementation"

#include "facility.hpp"

namespace px {

    facility::~facility() {
        static_cast<character_system&>(*this).close();
        static_cast<script_system&>(*this).close();
    }

    facility::facility() = default;
}