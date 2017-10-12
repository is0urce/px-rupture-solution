// name: light_works.hpp

#pragma once

#include "transform_component.hpp"

#include "light_component.hpp"

#include "rupture/draw/lightmap_data.hpp"

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
			last_data = current_data;
			current_data.ox = ox;
			current_data.oy = oy;
			++current_data.version;
			
			float * pen = current_texels.data();
			map.enumerate([&](size_t /*x*/, size_t /*y*/, color & c) {
				pen[0] = static_cast<float>(c.R);
				pen[1] = static_cast<float>(c.G);
				pen[2] = static_cast<float>(c.B);
				pen[3] = static_cast<float>(c.A);
				//pen += 4;
				//c.write(pen);
				pen += 4;
			});
		}
		void set_radius(unsigned int new_radius)
		{
			radius = new_radius;

			unsigned int w = radius * 2 + 1;
			map.resize(w, w);
			current_texels.resize(w * w * 4, 0);
			last_texels.resize(w * w * 4, 0);

			current_data.width = w;
			current_data.height = w;
			current_data.raw = current_texels.data();
			current_data.version = 1;

			last_data.width = w;
			last_data.height = w;
			last_data.raw = last_texels.data();
			last_data.version = 0;
		}
		lightmap_data const* current() noexcept
		{
			return &current_data;
		}
		lightmap_data const* last() noexcept
		{
			return &last_data;
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

		lightmap_data						current_data;
		lightmap_data						last_data;

		std::vector<float>					current_texels;
		std::vector<float>					last_texels;
	};
}