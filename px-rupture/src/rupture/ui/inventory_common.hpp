// name: inventory_common.hpp
// type: c++
// auth: is0urce
// desc: aux functions

#pragma once

#include <string>
#include <vector>

#include "../es/container_component.hpp"

namespace px {

    namespace {

        template <typename Predicate>
        void format_names(container_component & container, std::vector<std::string> & names, Predicate && predicate) {
            names.clear();
            container.enumerate([&](auto const& item) {
                if (predicate(item)) {
                    auto count = item.count();
                    names.push_back(item.name() + ((count == 1) ? std::string("") : std::string(" x") + std::to_string(count)));
                }
            });
        }

        void format_names(container_component & container, std::vector<std::string> & names) {
            format_names(container, names, [](auto const&) { return true; });
        }
    }
}