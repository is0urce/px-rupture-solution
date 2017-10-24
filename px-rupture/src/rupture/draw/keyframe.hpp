// name: keyframe.hpp

#pragma once

namespace px {

	struct sprite;

	struct keyframe {
		sprite const*	frame;	// what
		double			time;	// where
	};
}