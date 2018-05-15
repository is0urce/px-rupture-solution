// name: assert.hpp

#pragma once

#ifndef NDEBUG

#include <string>
#include <iostream>

namespace px {

    namespace {
        void px_debug(std::string const& text) {
            std::cout << text << std::endl;
        }
    }
}

#else

#include <string>

namespace px {

    namespace {
        void px_debug(std::string const& /* discarded */) {
        }
    }
}

#endif