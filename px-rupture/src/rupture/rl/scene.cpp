// name: scene.cpp

#include "scene.hpp"

#include "scene_internal.hpp"

#include <px/memory/memory.hpp>

namespace px {

	// ctor

	scene::~scene() = default;
	scene::scene()
		: works(make_uq<scene_internal>())
	{
	}

	// methods

	bool scene::is_transparent(point2 const& location) const {
		return works->is_transparent(location);
	}
	bool scene::is_traversable(point2 const& location, rl::traverse_options<rl::traverse> const& opts) const {
		return works->is_traversable(location, opts);
	}

	uq_ptr<composite_component> & scene::spawn(uq_ptr<composite_component> && ptr, point2 const& location) {
		return works->spawn(std::forward<uq_ptr<composite_component>>(ptr), location);
	}
	uq_ptr<composite_component> & scene::spawn(uq_ptr<composite_component> && ptr) {
		return works->spawn(std::forward<uq_ptr<composite_component>>(ptr));
	}

	void scene::assign_sprites(sprite_system * system) {
		works->assign_sprites(system);
	}

	void scene::pset(std::uint32_t block_id, point2 const& location) {
		works->pset(block_id, location);
	}

	qtree<transform_component*> * scene::space() noexcept {
		return works->get_space();
	}

	transform_component * scene::any(point2 const& location) const {
		return works->any(location);
	}
	body_component * scene::anybody(point2 const& location) const {
		return works->anybody(location);
	}
	void scene::discard(std::function<void(composite_component&)> fn) {
		works->discard(fn);
	}
	size_t scene::size() const {
		return works->size();
	}
	void scene::clear() {
		works->clear();
	}
	void scene::enumerate(std::function<void(composite_component&)> fn) {
		works->enumerate(fn);
	}
}