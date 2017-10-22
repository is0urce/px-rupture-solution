// name: scene.hpp

#pragma once

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>

#include <px/rl/traverse_options.hpp>
#include <px/rl/traverse.hpp>

#include <cstdint> // uint32_t block_id

namespace px {

	class scene_internal;

	class transform_component;
	class body_component;
	class composite_component;
	class sprite_system;

	template <typename T> class qtree;

	class scene final
	{
	public:
		bool is_transparent(point2 const& location) const;
		bool is_traversable(point2 const& location, rl::traverse_options<rl::traverse> const& opts) const;

		uq_ptr<composite_component> & spawn(uq_ptr<composite_component> && ptr, transform_component * transform, point2 const& location);
		uq_ptr<composite_component> & spawn(uq_ptr<composite_component> && ptr, transform_component * transform);

		void assign_sprites(sprite_system * system);
		void pset(std::uint32_t block_id, point2 const& location);
		qtree<transform_component*> * space() noexcept;

		transform_component * any(point2 const& location) const;
		body_component * anybody(point2 const& location) const;

	public:
		~scene();
		scene();

	private:
		uq_ptr<scene_internal> works;
	};
}