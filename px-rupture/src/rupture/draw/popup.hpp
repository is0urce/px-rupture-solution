// name: message.hpp

#pragma once

#include "message.hpp"

#include <px/common/point.hpp>

namespace px {

	struct popup
	{
		message msg;
		point2 position;
		double lift;
	};
}