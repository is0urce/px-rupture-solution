// name: npc_works.hpp

#pragma once

#include "npc_component.hpp"

#include <px/memory/memory.hpp>

namespace px {

	class npc_works
	{
	public:
		uq_ptr<npc_component> make()
		{
			return make_uq<npc_component>();
		}

	public:
		~npc_works()
		{
		}
		npc_works()
		{
		}

	private:

	};
}