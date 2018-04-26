// name: message_overlay.hpp

#pragma once

#include "message.hpp"
#include "message_data.hpp"
#include "popup.hpp"

#include <px/es/transform.hpp>

#include <px/common/point.hpp>

#include <vector>
#include <map>

namespace px {

    class message_works final {
    public:
        void add_message(message && msg, point2 const& location) {
            auto & grid = map[location];
            for (auto & prev : grid) {
                prev.lift += msg.magnitude;
            }
            grid.emplace_back(popup{ std::move(msg), location, 0.0 });
            ++total;
        }

        void cls() {
            map.clear();
            total = 0;
        }

        void target(transform const* center) noexcept {
            camera = center;
        }

        void flatten() {
            if (camera) {
                flat.messages.resize(total);
                unsigned int i = 0;
                for (auto && kv : map) {
                    for (auto && msg : kv.second) {
                        msg.position -= camera->position();
                        flat.messages[i] = std::forward<popup>(msg);
                        ++i;
                    }
                }
                ++flat.version;
            }
            cls();
        }

        message_data const* data() const noexcept {
            return &flat;
        }

    public:
        ~message_works() = default;

        message_works() noexcept
            : total(0)
            , camera(nullptr) {
        }

    private:
        struct lex_less {
            bool operator()(point2 const& a, point2 const& b) const { return a.lex_less(b); }
        };

    private:
        unsigned int                                    total;
        transform const*                                camera;
        message_data                                    flat;
        std::map<point2, std::vector<popup>, lex_less>  map;
    };
}