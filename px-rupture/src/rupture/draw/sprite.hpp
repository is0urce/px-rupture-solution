#include "image.hpp"

#pragma once

namespace px {

	struct sprite
		: public image
	{
		unsigned int texture_index;
		const char* name;
	};
}