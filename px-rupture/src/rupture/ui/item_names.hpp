// name: item_names.hpp
// type: c++
// auth: is0urce
// desc: aux functions

#pragma once

#include "../es/container_component.hpp"

#include <string>
#include <vector>

namespace px {

    class item_names {
    public:
        template <typename Predicate>
        static void format(container_component & container, std::vector<std::string> & names, Predicate && predicate) {
            names.clear();
            container.enumerate([&](auto const& item) {
                if (predicate(item)) {
                    auto count = item.count();
                    names.push_back(item.name() + ((count == 1) ? std::string("") : std::string(" x") + std::to_string(count)));
                }
            });
        }

        static void format(container_component & container, std::vector<std::string> & names) {
            format(container, names, [](auto const& /* any */) { return true; });
        }

        static bool getter(void * data, int n, char const** result) {
            auto const& vector = *static_cast<std::vector<std::string> const*>(data);
            if (n >= 0 && n < static_cast<int>(vector.size())) {
                *result = vector[n].c_str();
                return true;
            }
            return false;
        }
    };
}