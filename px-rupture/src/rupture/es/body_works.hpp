#pragma once

#include "body_component.hpp"


#include <px/memory/memory.hpp>

namespace px {
	class body_works final
	{
	public:
		uq_ptr<body_component> make()
		{
			return make_uq<body_component>();
		}
	};
}