// name: light_works.hpp
// type: c++ header
// auth: is0urce
// desc: class

#pragma once

// implementation of light works

#include "transform_component.hpp"

#include "light_component.hpp"

#include "rupture/draw/lightmap_data.hpp"
#include "rupture/rl/scene.hpp"

#include <px/algorithm/recursive_shadowcasting.hpp>
#include <px/algorithm/fov.hpp>
#include <px/common/matrix.hpp>
#include <px/common/color.hpp>
#include <px/common/pool_chain.hpp>

namespace px {

	class light_works {
	public:
		void assign_scene(scene const* blocking) noexcept {
			stage = blocking;
		}
		uq_ptr<light_component> make() {
			return lights.make_uq();
		}
		void target(transform_component const* target) {
			camera = target;
		}
		void set_fov_cropping(bool do_cropping) noexcept {
			crop_fov = do_cropping;
		}
		void calculate_lights() {
			if (!camera) return;

			set_offsets();

			color ambient(0, 0, 0, 1);
			double ambient_inv_distance = 0;

			// illuminate light matrix
			map.fill({ 0, 0, 0, 0 });
			lights.enumerate([&](light_component & light) {
				if (!light.is_active()) return;
				if (!light.is_on) return;
				transform_component * location = light.linked<transform_component>();
				if (!location) return;

				int light_x = location->position().x();
				int light_y = location->position().y();
				double elevation2 = light.elevation * light.elevation;
				std::fill(painted_flags.begin(), painted_flags.end(), false);

				switch (light.source) {
				case light_source::ambient: {
					double inv_distance = 1 / (1 + distance2(light_x - camera->position().x(), light_y - camera->position().y(), elevation2));
					if (inv_distance > ambient_inv_distance) {
						ambient_inv_distance = inv_distance;
						ambient = light.tint;
					}
					break;
				}
				case light_source::point: {
					recursive_shadowcasting::light(light_x, light_y, radius, [&](int x, int y) { return is_transparent(x, y); }, [&]
					(int x, int y) {
						double distance = std::sqrt(distance2(light_x - x, light_y - y, elevation2));
						distance = std::max(distance, 1.0);
						illuminate(x, y, light.tint / (distance * distance));
					});
					break;
				}
				default:
					break;
				}
			});

			// add ambient
			map.enumerate([&](size_t /*x*/, size_t /*y*/, color & clr) {
				clr += ambient;
			});

			// only light field-of-view
			if (crop_fov) {
				fov perception;
				perception.calculate(camera->position(), radius, [&](int x, int y) { return is_transparent(x, y); });
				map.enumerate([&](size_t x, size_t y, color & clr) {
					if (!perception.contains({ static_cast<int>(x) + ox, static_cast<int>(y) + oy })) {
						clr = 0;
					}
				});
			}

			// rotate data
			move_ligtmaps(); 
			
			// dump colors to texture bitmap
			float * pen = current_data.raw;
			map.enumerate([&](size_t /*x*/, size_t /*y*/, color & c) {
				c.write(pen);
				pen += 4;
			});
		}
		void set_radius(unsigned int new_radius) {
			radius = new_radius;

			width = radius * 2 + 1;

			map.resize(width, width);
			painted_flags.resize(width * width, false);
			current_texels.resize(width * width * 4, 0);
			last_texels.resize(width * width * 4, 0);

			current_data.width = width;
			current_data.height = width;
			current_data.raw = current_texels.data();
			current_data.version = 1;

			last_data.width = width;
			last_data.height = width;
			last_data.raw = last_texels.data();
			last_data.version = 0;
		}
		void clear_lightmap() {
			std::fill(current_texels.begin(), current_texels.end(), 0.0f);
			std::fill(last_texels.begin(), last_texels.end(), 0.0f);

			set_offsets();
			move_ligtmaps();
		}
		lightmap_data const* current() noexcept {
			return &current_data;
		}
		lightmap_data const* last() noexcept {
			return &last_data;
		}

	public:
		~light_works() = default;
		light_works(unsigned int start_radius)
			: camera(nullptr)
			, stage(nullptr)
			, crop_fov(true)
		{
			set_radius(start_radius);
		}
		light_works(light_works const&) = delete;
		light_works & operator=(light_works const&) = delete;

	private:
		void set_offsets() {
			if (camera) {
				ox = camera->position().x() - radius;
				oy = camera->position().y() - radius;
			}
		}
		// store current into last
		void move_ligtmaps() {
			last_data = current_data; // update export data structure

			current_data.ox = ox;
			current_data.oy = oy;
			current_data.version++;
			current_data.raw = (last_data.raw == current_texels.data() ? last_texels : current_texels).data(); // alternate
		}
		double distance2(double dx, double dy, double elevation2) {
			return std::sqrt(dx * dx + dy * dy + elevation2);
		}
		bool contains(int x, int y) const {
			return x >= 0 && y >= 0 && map.contains(static_cast<size_t>(x), static_cast<size_t>(y));
		}
		bool is_transparent(int x, int y) const {
			return stage && stage->is_transparent({ x, y });
		}
		void illuminate(int x, int y, color c) {
			if (is_transparent(x, y)) {
				pset(x - ox, y - oy, c);
			}
		}
		// add to point color
		void pset(int x, int y, color c) {
			if (contains(x, y)) {
				if (!painted_flags[y * width + x]) {
					painted_flags[y * width + x] = true;
					map.at(x, y) += c;
				}
			}
		}

	private:
		transform_component const*			camera;
		scene const*						stage;

		matrix2<color>						map;
		pool_chain<light_component, 1000>	lights;
		unsigned int						radius;
		size_t								width;

		lightmap_data						current_data;
		lightmap_data						last_data;

		std::vector<float>					current_texels;
		std::vector<float>					last_texels;

		std::vector<bool>					painted_flags;	// exclude diagonals hack
		int									ox;
		int									oy;

		bool								crop_fov;
	};
}