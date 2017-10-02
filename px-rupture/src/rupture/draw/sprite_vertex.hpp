// name: sprite_vertex.hpp
// type: c++ struct

#pragma once

#define GLM_FORCE_RADIANS
#pragma warning(push)
#pragma warning(disable:4201) // warning C4201: nonstandard extension used: nameless struct/union
#include <glm/glm.hpp>
#pragma warning(pop)

namespace px {

	struct sprite_vertex
	{
		glm::vec2 position;
		glm::vec2 texture;
	};
}