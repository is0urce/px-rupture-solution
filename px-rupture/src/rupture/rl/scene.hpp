// name: scene.hpp

#pragma once

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>

#include <px/rl/traverse_options.hpp>
#include <px/rl/traverse.hpp>

#include <cstdint>
#include <functional>

namespace px {

    class scene_internal;

    class transform_component;
    class body_component;
    class composite_component;
    class sprite_system;

    template <typename T> class qtree;

    class scene final {
    public:
        using visit_event = std::function<void(point2 const&)>;
        using area_query = std::function<void(point2 const&, body_component*)>;
    public:
        void                            assign_sprites(sprite_system * system) noexcept;
        void                            focus(point2 const& location);
        void                            set_enter_event(std::function<void(point2 const&)> fn);
        void                            set_leave_event(std::function<void(point2 const&)> fn);
        void                            unload();
        bool                            is_transparent(point2 const& location) const;
        bool                            is_traversable(point2 const& location, rl::traverse_options<rl::traverse> const& opts) const;

        void                            pset(std::uint32_t block_id, point2 const& location);
        qtree<transform_component*> *   space() noexcept;

        transform_component *           any(point2 const& location) const;
        body_component *                anybody(point2 const& location) const;
        void                            query_targets(point2 const& location, unsigned int radius, std::function<void(point2 const&, body_component*)> fn) const;

        uq_ptr<composite_component> &   spawn(uq_ptr<composite_component> && ptr);                                  // add object
        void                            clear_units();                                                              // remove all objects
        size_t                          size() const;                                                               // objects count
        void                            enumerate(std::function<void(composite_component&)>);                       // enumerate objects
        void                            discard(std::function<void(composite_component&)>);                         // remove destroyed objects
        void                            pull(point2 const& cell, std::function<void(uq_ptr<composite_component>)>); // remove destroyed objects

    public:
        ~scene();
        scene();

    private:
        uq_ptr<scene_internal>          works;
    };
}