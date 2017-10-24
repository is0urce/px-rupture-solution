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

			if (has_loop) {
				time = std::fmod(std::min(0.0, time), frames.back().time);
			}

			size_t i = 0;
			for (auto const& key : frames) {
				if (key.time > time) break;
				++i;
			}
			i = (i == 0) ? 0 : (i - 1);
			return frames[i].frame;
		}
		sprite const* last_frame() const {
			return frames.empty() ? nullptr : frames.back().frame;
		}

	public:
		std::vector<keyframe> frames;
		bool has_loop;
	};
}