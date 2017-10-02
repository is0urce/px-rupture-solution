// name: sprite_works.hpp
// type: c++
// auth: is0urce
// desc: internal sprite system works

#pragma once

#include "rupture/draw/sprite_vertex.hpp"

#include "sprite_component.hpp"
#include "transform_component.hpp"

#include <px/common/pool_chain.hpp>
#include <px/common/vector.hpp>

#include <map>
#include <string>
#include <vector>

namespace px {

	class sprite_works final
	{
	public:
		uq_ptr<sprite_component> make(std::string const& name)
		{
			uq_ptr<sprite_component> result;

			auto iterator = lib.find(name);
			if (iterator != lib.end()) {

				result = pool.make_uq();
				static_cast<sprite&>(*result) = iterator->second;

				result->name = iterator->first.c_str();
			}

			return result;
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
		void add_image(std::string const& name, float sx, float sy, float dx, float dy, unsigned int texture)
		{
			sprite & img = lib[name];

			img.sx_texture = sx;
			img.sy_texture = sy;
			img.dx_texture = dx;
			img.dy_texture = dy;
			img.x_transpose = 0;
			img.y_transpose = 0;
			img.x_multiple = 1;
			img.y_multiple = 1;
			img.texture_index = texture;
		}
		void add_image(std::string const& name, sprite && element)
		{
			lib[name] = element;
		}
		std::vector<sprite_vertex> const* data() const noexcept
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
		std::vector<sprite_vertex>			vertices;	// batch vertices data
		std::map<std::string, sprite>		lib;
	};
}