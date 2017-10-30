// name: body_works.hpp

#pragma once

#include "body_component.hpp"
#include "composite_component.hpp"

#include <px/common/pool_chain.hpp>

namespace px {
	class body_works final
	{
	public:
		void tick() {

			pool.enumerate([](body_component & body) {

				// rip
				if (body.is_dead()) {
					composite_component * unit = body.linked<composite_component>();
					if (unit) {
						//unit->destroy();
					}
				}
			});
		}
		uq_ptr<body_component> make() {
			return pool.make_uq();
		}

	public:
		pool_chain<body_component, 1024> pool;
	};
}