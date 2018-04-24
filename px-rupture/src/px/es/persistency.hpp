// name: persistency.hpp
// type: c++ header
// desc: enum class
// auth: is0urce

#pragma once

namespace px {

    enum class persistency : unsigned char {
        serialized = 0, // should be partialy serialized
        permanent = 1,  // should not be serialized if possible
        temporary = 2,  // should be discarded on serializaton
        destroying = 3, // in destruction
    };
}