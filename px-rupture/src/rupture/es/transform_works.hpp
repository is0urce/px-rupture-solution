// name: transform_works.hpp

#pragma once

#include "transform_component.hpp"

#include <px/common/qtree.hpp>
#include <px/memory/memory.hpp>

namespace px {

	class transform_works final
	{
	public:
		uq_ptr<transform_component> make()
		{
			auto result = make_uq<transform_component>();
			result->incarnate(&space);
			return result;
		}

	public:
		transform_works() noexcept
			: space(64)
		{
		}

	private:
		qtree<transform_component*> space;
	};
}