// name: builder.hpp
// type: c++ header
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>

#include <string>

namespace px {

    class animator_component;
    class body_component;
    class facility;
    class character_component;
    class composite_component;
    class container_component;
    class deposite_component;
    class door_component;
    class light_component;
    class npc_component;
    class player_component;
    class sprite_component;
    class storage_component;
    class transform_component;
    class workshop_component;
    class value_component;
    class script_component;

    class builder final {
    public:
        animator_component * add_animator(std::string const& name);
        body_component *            add_body();
        character_component *       add_character();
        container_component *       add_container();
        deposite_component *        add_deposite();
        door_component *            add_door();
        light_component *           add_light();
        npc_component *             add_npc();
        player_component *          add_player();
        sprite_component *          add_sprite(std::string const& name);
        storage_component *         add_storage();
        transform_component *       add_transform(point2 const& location);
        workshop_component *        add_workshop();
        value_component *           add_value();
        script_component *          add_script();
        void                        begin();
        void                        begin(uq_ptr<composite_component> strip);
        transform_component *       get_transform() const;
        bool                        has_player() const noexcept;
        void                        remove_animator();
        void                        remove_body();
        void                        remove_character();
        void                        remove_container();
        void                        remove_deposite();
        void                        remove_door();
        void                        remove_light();
        void                        remove_npc();
        void                        remove_player();
        void                        remove_sprite();
        void                        remove_transform();
        void                        remove_workshop();
        void                        remove_value();
        void                        remove_script();
        uq_ptr<composite_component> request();
        transform_component *       created_player() const;

    public:
        ~builder();
        builder(facility * production_factory);

    private:
        void                        link_components();

    private:
        uq_ptr<composite_component> unit;

        facility *                  factory;

        animator_component *        animator;
        body_component *            body;
        character_component *       character;
        container_component *       container;
        deposite_component *        deposite;
        door_component *            door;
        light_component *           light;
        npc_component *             npc;
        player_component *          player;
        sprite_component *          sprite;
        storage_component *         storage;
        transform_component *       transform;
        workshop_component *        workshop;
        value_component *           value;
        script_component *          script;
        transform_component *       last_player;
    };
}