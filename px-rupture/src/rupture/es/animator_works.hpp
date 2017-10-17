#pragma once

#include "animator_component.hpp"

#include <px/memory/memory.hpp>

namespace px {
	class animator_works
	{
	public:
		uq_ptr<animator_component> make(std::string const& /*name*/)
		{
			return make_uq<animator_component>();
		}
		void update(double /*time*/)
		{

		}
		void finish_animations()
		{

		}

	public:
		~animator_works()
		{
		}
		animator_works()
		{
		}
	};
}