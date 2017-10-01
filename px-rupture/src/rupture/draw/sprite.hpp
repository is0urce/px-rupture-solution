// name: sprite.hpp

#include "image.hpp"

#pragma once

namespace px {

	struct sprite
		: public image
	{
		double x_transpose;
		double y_transpose;
		double x_multiple;
		double y_multiple;

		unsigned int texture_index;
		const char* name;
	};
}