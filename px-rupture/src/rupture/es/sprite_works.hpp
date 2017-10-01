// name: sprite_works.hpp
// type: c++
// auth: is0urce
// desc: internal sprite system works

#pragma once

#include "sprite_component.hpp"

#include "transform_component.hpp"
#include "rupture/draw/vertex.hpp"

#include <px/common/pool_chain.hpp>
#include <px/common/vector.hpp>

#include <vector>

namespace px {

	class sprite_works final
	{
	public:
		uq_ptr<sprite_component> make()
		{
			return pool.make_uq();
		}
		void target(transform_component const* follow) noexcept
		{
			camera = follow;
		}
		void batch(double interpolation)
		{
			vector2 focus = camera ? lerp(*camera, interpolation) : vector2(0, 0);
			vertices.clear();

			pool.enumerate([&](sprite_component & sprite) {

				if (sprite.is_active()) {

					if (transform_component * location = sprite.linked<transform_component>()) {

						vector2 pos = lerp(*location, interpolation) - focus;

						float x = static_cast<float>(pos.x() + sprite.x_transpose);
						float y = static_cast<float>(pos.y() + sprite.y_transpose);
						float sx = static_cast<float>(sprite.sx_texture);
						float sy = static_cast<float>(sprite.sy_texture);
						float dx = static_cast<float>(sprite.dx_texture);
						float dy = static_cast<float>(sprite.dy_texture);
						float mx = static_cast<float>(sprite.x_multiple);
						float my = static_cast<float>(sprite.y_multiple);

						vertices.push_back({ { x + 0, y + 0 }, { sx, sy } });
						vertices.push_back({ { x + 0, y + my }, { sx, dy } });
						vertices.push_back({ { x + mx, y + my }, { dx, dy } });
						vertices.push_back({ { x + mx, y + 0 }, { dx, sy } });
					}
				}
			});
		}
		std::vector<vertex> const* data() const noexcept
		{
			return &vertices;
		}

	public:
		sprite_works() noexcept
			: camera(nullptr)
		{
		}

	private:
		vector2 lerp(transform_component const& transform, double interpolation) noexcept
		{
			return vector2(transform.last()).lerp(transform.position(), interpolation);
		}

	private:
		pool_chain<sprite_component, 10000>	pool;		// sprite pool
		transform_component const*			camera;		// focus camera transform
		std::vector<vertex>					vertices;	// batch vertices data
	};
}