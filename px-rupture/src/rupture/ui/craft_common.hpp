#pragma once

#include <vector>
#include <string>

#include "rupture/es/container_component.hpp"

namespace px {

    namespace {

        bool name_getter(void * data, int n, const char** result) {
            auto & vector = *static_cast<std::vector<std::string>*>(data);
            if (n >= 0 && n < static_cast<int>(vector.size())) {
                *result = vector[n].c_str();
                return true;
            }
            return false;
        }

        void sort(container_component & container) {
            container.sort([](auto const& a, auto const& b) { return a->name() < b->name(); });
        }

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