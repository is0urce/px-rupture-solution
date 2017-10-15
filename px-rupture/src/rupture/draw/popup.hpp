// name: message.hpp

#pragma once

#include "message.hpp"

#include <px/common/vector.hpp>

namespace px {

	class popup
	{
	public:
		message msg;
		vector2 position;
		double lift;
		//unsigned int line;
	};
}