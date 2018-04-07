// name: environment.hpp
// type: c++
// auth: is0urce
// desc: class declaration

#pragma once

#include "core.hpp"

#include "es/notification_system.hpp"
#include "rl/scene.hpp"

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>
#include <px/rl/damage_type.hpp>
#include <px/rl/hit_result.hpp>
#include <px/rl/craft_activity.hpp>

#include <cstdint>
#include <random>
#include <string>
#include <vector>

namespace px {

    class body_component;
    class transform_component;
    class composite_component;
    class repository;

    struct visual;

    class environment
        : public core
    {
    public:
        point2                              area() const noexcept;              // hovered point
        transform_component *               target() noexcept;                  // hover point in world
        void                                target(point2 offset);              // hovered unit
        bool                                is_running() const noexcept;
        void                                shutdown();                         // close game
        void                                step(point2 const& movement);       // move player
        void                                advance();                          // move player to target
        bool                                action(unsigned int action_idx);    // use skill
        void                                use(unsigned int mods);             // use object
        void                                function_edit(std::uint32_t idx);
        void                                start();
        void                                end();
        void                                clear();
        transform_component *               possessed() noexcept;                                                   // get current player
        void                                start_turn();                                                           // clear from previous turn
        void                                end_turn(unsigned int turns);                                           // end player turn and sart world processing
        bool                                turn_passed() const noexcept;                                           // true if it's the world processing stage
        unsigned int                        current_turn() const noexcept;                                          // returns turn number
        void                                return_turn();                                                          // pass turn back to player
        void                                popup(std::string, color tint, point2 location);
        void                                popup(std::string, color tint);
        int                                 distance(point2 const& a, point2 const& b) const;
        void                                give_experience(unsigned int experience, unsigned int level);
        bool                                has_access(rl::craft_activity station) const noexcept;
        void                                open_workshop(rl::craft_activity station);
        void                                close_workshop();
        bool                                save(std::string const& name);
        bool                                load(std::string const& name);
        uq_ptr<composite_component> &       spawn(uq_ptr<composite_component> unit);
        uq_ptr<composite_component> &       spawn(std::string const& blueprint, point2 const& position);
        void                                update_blueprints(std::string const& out_path, std::string const& blueprint);

        // script bindings
    public:
        bool                                in_sight(body_component const& body, point2 const& location) const;     // in line of sight
        bool                                in_line(body_component const& body, point2 const& location) const;      // in line of movement
        void                                emit_visual(std::string const& name, point2 start, point2 finish, transform_component const* track);
        void                                emit_animation(std::string const& name, unsigned int clip_id, point2 start, point2 finish, transform_component const* track);
        void                                emit_light(point2 position, color const& light);
        rl::hit_result                      hit(body_component const& source, body_component const& target);
        std::tuple<float, rl::damage_type>  dps(body_component const& source) const;
        int                                 damage(body_component & target, int damage, rl::damage_type source);    // returns damage done
        bool                                is_traversable(point2 const location, body_component const& body) const;
        std::tuple<bool, point2>            neighbour(point2 const location, unsigned int pos) const;
        int                                 roll(int min, int max);

    public:
        virtual                             ~environment();
        environment();

    private:
        void                                incarnate(transform_component * target);
        void                                lock_target();
        void                                save_main();
        void                                load_main();
        void                                save_scene(point2 const& cell);
        void                                load_scene(point2 const& cell);
        void                                prewarm();                          // start routine
        transform_component *               create_player();                    // start routine
        bool                                has_control() const;                // returns true if player can act now

    protected:
        notification_system                 messages;
        scene                               stage;

    private:
        transform_component *               player;
        uq_ptr<repository>                  current;
        uq_ptr<repository>                  parent;
        bool                                run;
        unsigned int                        turn_number;
        bool                                turn_pass;                          // true if it's the world processing stage
        point2                              target_hover;                       // offset of target area
        point2                              target_area;                        // target area in world coordinates
        transform_component *               target_unit;                        // targeted unit
        rl::craft_activity                  opened_workshop;
        std::vector<visual>                 vfx;                                // current visual effects
        std::mt19937                        rng;                                // random number generator engine
    };
}