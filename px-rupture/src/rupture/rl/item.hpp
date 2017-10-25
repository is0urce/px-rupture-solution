#pragma once

#include <px/rl/entity.hpp>

#include "status.hpp"

namespace px::rl {

	class item
		: public entity
		, public status
	{
	public:
		item()
		{
		}
	};
}