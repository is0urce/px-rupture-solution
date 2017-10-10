// name: light.hpp
// type: c++ header
// desc: class
// auth: is0urce

#pragma once

#include <px/common/color.hpp>

namespace px {

	class light
	{
	public:
		color tint;
		double elevation;
		bool is_on;
	};
}