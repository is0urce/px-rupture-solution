// name: sprite_system.hpp

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

#include <string>
#include <vector>

namespace px {

	class sprite_component;
	class sprite_works;
	class transform_component;
	struct sprite_vertex;

	class sprite_system final
		: public system<delta>
	{
	public:
		uq_ptr<sprite_component> make(std::string const& name);
		std::vector<sprite_vertex> const* data() const noexcept;
		void target(transform_component const*) noexcept;
		void add_sprite(std::string const& name, float sx, float sy, float dx, float dy, unsigned int texture);

		template <typename Document>
		void load_atlas(Document && doc, unsigned int texture_id, bool reverse_y);

	public:
		virtual ~sprite_system();
		sprite_system();

	protected:
		virtual void update_system(delta_type const& delta_time);

	private:
		uq_ptr<sprite_works> works;
	};

	template <typename Document>
	void sprite_system::load_atlas(Document && doc, unsigned int texture_id, bool reverse_y) {
		for (auto const& frame : doc["atlas"]) {
			std::string name = frame["name"];
			float sx = frame["sx"];
			float sy = frame["sy"];
			float dx = frame["dx"];
			float dy = frame["dy"];
			sy = reverse_y ? 1 - sy : sy;
			dy = reverse_y ? 1 - dy : dy;
			add_sprite(name, sx, sy, dx, dy, texture_id);
		}
	}
}