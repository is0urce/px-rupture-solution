// name: canera_uniform.hpp
// type: c++ struct

#pragma once

#define GLM_FORCE_RADIANS
#pragma warning(push)
#pragma warning(disable:4201) // warning C4201: nonstandard extension used: nameless struct/union
#include <glm/glm.hpp>
#pragma warning(pop)

namespace px {

	struct camera_uniform
	{
		glm::vec2 scale;
		glm::vec2 translate;
		glm::vec2 screen;
		glm::vec2 lightmap_size;
		glm::vec2 current_offset;
		glm::vec2 last_offset;
	};

}