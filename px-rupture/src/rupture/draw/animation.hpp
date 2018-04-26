// name: animation.hpp

#pragma once

#include "keyframe.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

namespace px {

    class animation {
    public:
        sprite const* select_frame(double time) const {
            if (frames.empty()) return nullptr;

            if (loop) {
                time = std::fmod(std::max<decltype(time)>(0, time), frames.back().time);
            }

            return frame(time);
        }

        sprite const* last_frame() const {
            return frames.empty() ? nullptr : frames.back().frame;
        }

        bool is_loop() const noexcept {
            return loop;
        }

    private:
        sprite const* frame(double time) const {
            size_t i = 0;
            for (auto const& key : frames) {
                if (key.time > time) break;
                ++i;
            }
            i = (i == 0) ? 0 : (i - 1);
            return frames[i].frame;
        }

    public:
        std::vector<keyframe>   frames;
        bool                    loop;
    };
}