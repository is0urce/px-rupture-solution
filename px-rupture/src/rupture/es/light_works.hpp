// name: light_works.hpp

#pragma once

#include "transform_component.hpp"

#include "light_component.hpp"

#include "rupture/draw/lightmap.hpp"

#include <px/algorithm/recursive_shadowcasting.hpp>
#include <px/common/matrix.hpp>
#include <px/common/color.hpp>
#include <px/common/pool_chain.hpp>

namespace px {

	class light_works
	{
	public:
		uq_ptr<light_component> make()
		{
			return lights.make_uq();
		}
		void target(transform_component const* target)
		{
			camera = target;
		}
		void calculate_lights()
		{
			if (!camera) return;

			int ox = camera->position().x() - radius;
			int oy = camera->position().y() - radius;

			// illuminate light matrix
			map.fill({ 0, 0, 0, 0 });
			lights.enumerate([&](light_component & light) {
				if (!light.is_active()) return;
				transform_component * location = light.linked<transform_component>();
				if (!location) return;

				int light_x = location->position().x();
				int light_y = location->position().y();
				double elevation2 = light.elevation * light.elevation;
				recursive_shadowcasting::light(light_x, light_y, radius, [&](int x, int y) { return is_transparent(x, y); }, [&]
				(int x, int y) {
					int dx = light_x - x;
					int dy = light_y - y;
					double distance = std::sqrt(dx * dx + dy * dy + elevation2);
					illuminate(x - ox, y - oy, light.tint / (1 + distance));
				});
			});

			// update export data structure
			draw_data.ox = ox;
			draw_data.oy = oy;
			++draw_data.version;
		}
		void set_radius(unsigned int new_radius)
		{
			radius = new_radius;
			map.resize(radius * 2 + 1, radius * 2 + 1);
			draw_data.width = radius * 2 + 1;
			draw_data.height = radius * 2 + 1;
			draw_data.data = &(map.get(0, 0).R);
			draw_data.version = 0;
		}
		lightmap const* current() noexcept
		{
			return &draw_data;
		}
		lightmap const* last() noexcept
		{
			return &draw_data;
		}


	public:
		~light_works()
		{
		}
		light_works(unsigned int start_radius)
			: camera(nullptr)
		{
			set_radius(start_radius);
		}
		light_works(light_works const&) = delete;
		light_works & operator=(light_works const&) = delete;

	private:
		bool contains(int x, int y) const
		{
			return x >= 0 && y >= 0 && map.contains(static_cast<size_t>(x), static_cast<size_t>(y));
		}
		bool is_transparent(int /*x*/, int /*y*/) const
		{
			return true;
		}
		void illuminate(int x, int y, color c)
		{
			if (contains(x, y)) {
				map.get(x, y) += c;
			}
		}

	private:
		transform_component const*			camera;
		matrix2<color>						map;
		pool_chain<light_component, 1000>	lights;
		unsigned int						radius;
		lightmap							draw_data;
	};
}