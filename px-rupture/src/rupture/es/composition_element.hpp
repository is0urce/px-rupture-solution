// name: composition_element.hpp
// type: c++ header
// desc: enum class
// auth: is0urce

#pragma once

#include <cstdint>

namespace px {

	// values used by serizlizer, so they are set to some actual values to avoid versions conflicts
	enum class composition_element : std::uint32_t
	{
		// core data
		composition = 'X',
		transform	= 'T',	// location
		sprite		= 'S',
		animator	= 'A',
		light		= 'L',
		sound		= 'W',
		body		= 'B',
		container	= 'I',	// inventory
		character	= 'C',

		// useables
		door		= 'd',
		storage		= 'l',	// l for loot
		workshop	= 'w',
		deposit		= 'r',	// r for resource

		// controls
		player		= 'P',
		npc			= 'M',

		undefined	= '!'	// unspecified, used mostly for indicating components that is not serialized
	};
}