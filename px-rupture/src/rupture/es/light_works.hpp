// name: light_works.hpp

#pragma once

#include "transform_component.hpp"

#include "light_component.hpp"

#include <px/algorithm/recursive_shadowcasting.hpp>
#include <px/common/matrix.hpp>

#include <px/common/color.hpp>

#include <px/common/pool_chain.hpp>

namespace px {

	class light_works
	{
	public:
		void target(transform_component const* target)
		{
			camera = target;
		}
		void calculate_lights()
		{
			if (!camera) return;
			//int center_x = camera->position().x();
			//int center_y = camera->position().y();

			lights.enumerate([&](light_component & light) {
				if (!light.is_active()) return;
				transform_component * location = light.linked<transform_component>();
				if (!location) return;

				int light_x = location->position().x();
				int light_y = location->position().y();
				recursive_shadowcasting::light(light_x, light_y, 10, [&](int x, int y) { return is_transparent(x, y); }, [&](int x, int y) { illuminate(x, y, light_x, light_y, light.tint); });
			});
		}
		uq_ptr<light_component> make()
		{
			return lights.make_uq();
		}


	public:
		~light_works()
		{
		}
		light_works() noexcept
			: camera(nullptr)
		{
		}
		light_works(light_works const&) = delete;
		light_works & operator=(light_works const&) = delete;

	private:
		bool contains(int x, int y) const
		{
			return x >= 0 && y >= 0 && lightmap.contains(static_cast<size_t>(x), static_cast<size_t>(y));
		}
		bool is_transparent(int x, int y) const
		{
			return contains(x, y);
		}

		void illuminate(int x, int y, color c)
		{
			if (contains(x, y)) {
				lightmap.get(x, y) += c;
			}
		}
		void illuminate(int x, int y, int sx, int sy, color c)
		{
			sx -= x;
			sy -= y;
			illuminate(x, y, c / (1 + std::sqrt(sx * sx + sy * sy)));
		}

	private:
		transform_component const*			camera;
		matrix2<color, 10, 10>				lightmap;
		pool_chain<light_component, 1000>	lights;
	};
}