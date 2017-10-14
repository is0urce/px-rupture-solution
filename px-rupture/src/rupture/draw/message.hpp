// name: message.hpp

#pragma once

#include <px/common/color.hpp>

#include <string>

namespace px {
	
	struct message
	{
		std::string text;
		color tint;
		double magnitude;
	};
}