// name: transform_works.hpp

#pragma once

#include "transform_component.hpp"

#include <px/common/qtree.hpp>
#include <px/memory/memory.hpp>

#include <px/common/pool_chain.hpp>

namespace px {

	class transform_works final
	{
	public:
		uq_ptr<transform_component> make()
		{
			auto result = pool.make_uq();
			result->incarnate(&space);
			return result;
		}
		void store()
		{
			pool.enumerate([](transform_component & position) {
				position.store();
			});
		}

	public:
		transform_works() noexcept
			: space(64)
		{
		}

	private:
		qtree<transform_component*> space;
		pool_chain<transform_component, 1000> pool;
	};
}