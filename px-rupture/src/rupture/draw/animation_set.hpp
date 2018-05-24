// name: animation_set.hpp

#pragma once

#include "animation.hpp"

#include <map>
#include <string>
#include <vector>

namespace px {

    struct animation_set {
        std::vector<animation>          clips;
        std::map<std::string, size_t>   names;
    };
}