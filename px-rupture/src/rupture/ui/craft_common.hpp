// name: craft_common.hpp

#pragma once

#include <vector>
#include <string>

#include "../es/container_component.hpp"

namespace px {

    namespace {

        template <typename Predicate>
        void format_names(container_component & container, std::vector<std::string> & names, Predicate && predicate) {
            names.clear();
            container.enumerate([&](auto const& item) {
                if (predicate(item)) {
                    auto count = item.count();
                    names.push_back(item.name() + " x" + std::to_string(count));
                }
            });
        }

        void format_names(container_component & container, std::vector<std::string> & names) {
            format_names(container, names, [](auto const&) { return true; });
        }
    }
}