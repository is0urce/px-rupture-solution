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
				if (composite_component * unit = body.linked<composite_component>()) {
					switch (unit->lifetime()) {
					case persistency::permanent: // skip
						break;
					case persistency::destroying:
						unit->decay(1);
						break;
					default:
						if (body.is_dead()) {
							unit->destroy(0);
						}
						break;
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