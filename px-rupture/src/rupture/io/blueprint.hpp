// name: blueprint.hpp

#pragma once

#include "../es/builder.hpp"
#include "../es/composite_component.hpp"
#include "../es/composition_element.hpp"

#include "../es/animator_component.hpp"
#include "../es/body_component.hpp"
#include "../es/character_component.hpp"
#include "../es/container_component.hpp"
#include "../es/deposite_component.hpp"
#include "../es/door_component.hpp"
#include "../es/light_component.hpp"
#include "../es/npc_component.hpp"
#include "../es/player_component.hpp"
#include "../es/sprite_component.hpp"
#include "../es/transform_component.hpp"
#include "../es/workshop_component.hpp"

namespace px {

    class blueprint {
    public:

        template <typename Archive>
        static void store(Archive && archive, composite_component const& unit) {
            size_t total_components = unit.size();
            archive(total_components);
            unit.enumerate_components([&](auto const& component_ptr) {
                component * raw = component_ptr.get();
                if (auto transform = dynamic_cast<transform_component const*>(raw)) {
                    archive(composition_element::transform);
                    archive(*transform);
                }
                else if (auto sprite = dynamic_cast<sprite_component const*>(raw)) {
                    archive(composition_element::sprite);
                    archive(std::string(sprite->name));
                }
                else if (auto animator = dynamic_cast<animator_component const*>(raw)) {
                    archive(composition_element::animator);
                    archive(std::string(animator->get_id()));
                    archive(*animator);
                }
                else if (auto light = dynamic_cast<light_component const*>(raw)) {
                    archive(composition_element::light);
                    archive(*light);
                }
                else if (auto body = dynamic_cast<body_component const*>(raw)) {
                    archive(composition_element::body);
                    archive(*body);
                }
                else if (auto container = dynamic_cast<container_component const*>(raw)) {
                    archive(composition_element::container);
                    archive(*container);
                }
                else if (auto character = dynamic_cast<character_component const*>(raw)) {
                    archive(composition_element::character);
                    archive(*character);
                }
                else if (auto deposit = dynamic_cast<deposite_component const*>(raw)) {
                    archive(composition_element::deposit);
                    archive(*deposit);
                }
                else if (auto door = dynamic_cast<door_component const*>(raw)) {
                    archive(composition_element::door);
                    archive(*door);
                }
                else if (workshop_component const* workshop = dynamic_cast<workshop_component const*>(raw)) {
                    archive(composition_element::workshop);
                    archive(*workshop);
                }
                else if (auto player = dynamic_cast<player_component const*>(raw)) {
                    archive(composition_element::player);
                    archive(*player);
                }
                else if (auto npc = dynamic_cast<npc_component const*>(raw)) {
                    archive(composition_element::npc);
                    archive(*npc);
                }
                else {
                    archive(composition_element::undefined);
                }
            });

            archive(unit);
        }

        template <typename Archive>
        static uq_ptr<composite_component> assemble(Archive && archive, builder & factory) {
            size_t size;								// total components in unit
            composition_element variant;				// current component type
            transform_component * transform = nullptr;	// transform hint

            archive(size);
            for (size_t i = 0; i != size; ++i) {
                archive(variant);
                switch (variant) {
                case composition_element::transform: {
                    transform = factory.add_transform({});
                    archive(*transform); // transform disabled by default, so we can write into internal state
                    break;
                }
                case composition_element::sprite: {
                    std::string sprite_tag;
                    archive(sprite_tag);
                    factory.add_sprite(sprite_tag);
                    break;
                }
                case composition_element::animator: {
                    std::string name_id;
                    archive(name_id);
                    archive(*factory.add_animator(name_id));
                    break;
                }
                case composition_element::light: {
                    archive(*factory.add_light());
                    break;
                }
                case composition_element::body: {
                    archive(*factory.add_body());
                    break;
                }
                case composition_element::container: {
                    archive(*factory.add_container());
                    break;
                }
                case composition_element::character: {
                    archive(*factory.add_character());
                    break;
                }
                case composition_element::deposit: {
                    archive(*factory.add_deposite());
                    break;
                }
                case composition_element::door: {
                    archive(*factory.add_door());
                    break;
                }
                case composition_element::workshop: {
                    archive(*factory.add_workshop());
                    break;
                }
                case composition_element::player: {
                    archive(*factory.add_player());
                    break;
                }
                case composition_element::npc: {
                    archive(*factory.add_npc());
                    break;
                }
                case composition_element::undefined: {
                    break; // there was unserilized (temporary) component, just skip it
                }
                default:
                    // component defined, but not supported (version conflict?)
                    throw std::runtime_error("px::environment::load_unit(builder, archive) - unknown component");
                }
            }

            auto unit = factory.request();
            archive(*unit);
            return unit;
        }
    };
}