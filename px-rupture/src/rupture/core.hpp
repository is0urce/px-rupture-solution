// name: core.hpp

#pragma once

#include "draw/renderer.hpp"
#include "es/sprite_system.hpp"
#include "es/transform_system.hpp"
#include "rl/terrain.hpp"

#include <string>

namespace px {

	class core
	{
	public:
		void resize(int screen_width, int screen_height);

	public:
		~core();
		core(unsigned int start_widht, unsigned int start_height);

	private:
		void connect_managers();
		void load_data();
		void add_texture(std::string const& path);
		void add_atlas(std::string const& path, unsigned int texture_index);

	protected:
		sprite_system		sprites;
		transform_system	transforms;
		renderer			render;
		terrain				terrain;

	private:
		unsigned int		width;
		unsigned int		height;
	};
}