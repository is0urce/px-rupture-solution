#pragma once

#include "core.hpp"

namespace px {
	class environment
		: public core
	{
	public:
		environment(unsigned int w, unsigned int h)
			: core(w, h)
		{
		}
	};
}