// name: sprite_works.hpp
// type: c++
// auth: is0urce
// desc: internal sprite system works

#pragma once

#include "sprite_component.hpp"

#include <px/common/pool_chain.hpp>

namespace px {

	class sprite_works final
	{
	public:
		uq_ptr<sprite_component> make()
		{
			return pool.make_uq();
		}
		void batch(double interpolation_time)
		{
			pool.enumerate([&](sprite_component & sprite) {
				sprite.sx_texture = static_cast<float>(interpolation_time);
			});
		}

	public:
		sprite_works() noexcept
		{
		}

	private:
		pool_chain<sprite_component, 10000> pool;
	};
}