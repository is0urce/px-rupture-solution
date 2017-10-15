// name: message.hpp

#pragma once

#include <px/common/color.hpp>

#include <string>

namespace px {
	
	class message
	{
	public:
		std::string text;
		color tint;
		double magnitude;
	};
}