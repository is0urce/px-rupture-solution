// name: scene.cpp

#include "scene.hpp"

#include "scene_internal.hpp"

#include <px/memory/memory.hpp>

namespace px {

    // constructor & desctuctor

    scene::~scene() = default;

    scene::scene()
        : works(make_uq<scene_internal>()) {
    }

    // methods

    void scene::focus(point2 const& location) {
        works->focus(location);
    }

    bool scene::is_transparent(point2 const& location) const {
        return works->is_transparent(location);
    }

    bool scene::is_traversable(point2 const& location, rl::traverse_options<rl::traverse> const& opts) const {
        return works->is_traversable(location, opts);
    }

    uq_ptr<composite_component> & scene::spawn(uq_ptr<composite_component> && ptr) {
        return works->spawn(std::forward<uq_ptr<composite_component>>(ptr));
    }

    void scene::assign_sprites(sprite_system * system) noexcept {
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

    void scene::query_targets(point2 const& location, unsigned int radius, std::function<void(point2 const&, body_component*)> fn) const {
        works->query_targets(location, radius, fn);
    }

    //void scene::query_targets(point2 const& location, unsigned int radius, area_query && fn) {
    //    works->query_targets(location, radius, std::move(fn));
    //}

    void scene::discard(std::function<void(composite_component&)> fn) {
        works->discard(fn);
    }

    size_t scene::size() const {
        return works->size();
    }

    void scene::clear_units() {
        works->clear_units();
    }

    void scene::set_enter_event(std::function<void(point2 const&)> evt) {
        works->set_enter_event(evt);
    }

    void scene::set_leave_event(std::function<void(point2 const&)> evt) {
        works->set_leave_event(evt);
    }

    void scene::enumerate(std::function<void(composite_component&)> fn) {
        works->enumerate(fn);
    }

    void scene::unload() {
        works->unload();
    }

    void scene::pull(point2 const& cell, std::function<void(uq_ptr<composite_component>)> fn) {
        works->pull(cell, fn);
    }
}