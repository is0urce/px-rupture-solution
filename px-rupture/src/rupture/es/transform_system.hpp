// name: transform_system.hpp

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include "transform_component.hpp"

#include <px/common/qtree.hpp>
#include <px/memory/memory.hpp>

namespace px {
	
	class transform_system
		: public system<delta>
	{
	public:
		uq_ptr<transform_component> make()
		{
			auto result = make_uq<transform_component>();
			result->incarnate(&space);
			return result;
		}

	public:
		virtual ~transform_system()
		{
		}
		transform_system()
			: space(64)
		{
		}

	private:
		qtree<transform_component*> space;
	};
}