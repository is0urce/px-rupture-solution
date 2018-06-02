// name: animation.hpp
// type: c++
// auth: is0urce
// desc: one animation clip

#pragma once

#include "keyframe.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

namespace px {

    struct animation {
    public:
        sprite const* select_frame(double time) const {
            if (frames.empty()) return nullptr;

            if (loop) {
                time = std::fmod(std::max<decltype(time)>(time, 0), frames.back().time);
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
            return frames[frame_index(time)].frame;
        }

        size_t frame_index(double time) const {
            size_t i = 0;
            for (auto const& key : frames) {
                if (key.time > time) break;
                ++i;
            }
            return (i == 0) ? 0 : (i - 1);
        }

    public:
        std::vector<keyframe>   frames;
        bool                    loop;
    };
}