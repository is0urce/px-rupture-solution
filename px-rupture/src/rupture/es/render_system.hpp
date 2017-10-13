// name: render_system.hpp

#pragma once

#include "rupture/draw/sprite_vertex.hpp"

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

#include <vector>

namespace px {

	class renderer;
	class lightmap_data;

	class render_system final
		: public system<delta>
	{
	public:
		void resize(int width, int height);
		void assign_sprite_data(std::vector<std::vector<sprite_vertex>> const* data) noexcept;
		void assign_lightmap_data(lightmap_data const* current, lightmap_data const* last) noexcept;
		void add_texture(unsigned int texture_width, unsigned int texture_height, void const* data);
		void zoom(bool up);

	public:
		virtual ~render_system();
		render_system(unsigned int width, unsigned int height);

	protected:
		virtual void update_system(delta_type const& delta_time);

	private:
		uq_ptr<renderer> works;
	};
}