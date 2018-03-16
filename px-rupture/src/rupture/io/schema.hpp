// name: schema.hpp
// type: c++ header
// auth: is0urce
// desc: template class

#pragma once

// build unit from document

#include "../es/builder.hpp"
#include "../es/body_component.hpp"
#include "../es/container_component.hpp"
#include "../es/character_component.hpp"

#include <bitset>	// movement mask
#include <string>	// keys

namespace px {

    class schema {
        template <typename Document>
        using node_builder = void(Document &&, builder &);

    public:
        // build unit from document
        template <typename Document>
        static auto load(Document && doc, builder & factory) {
            factory.begin();

            // component props
            load_node(doc, factory, "transform", &load_transform);
            load_node(doc, factory, "sprite", &load_sprite);
            load_node(doc, factory, "body", &load_body);
            load_node(doc, factory, "container", &load_container);
            load_node(doc, factory, "character", &load_character);
            load_node(doc, factory, "npc", &load_npc);
            load_node(doc, factory, "deposite", &load_deposite);

            auto result = factory.request();

            // composite unit props
            std::string name = doc.value("name", std::string{ "" });
            persistency lifetime = static_cast<persistency>(doc.value("persistency", static_cast<long long>(persistency::serialized)));
            result->set_name(name);
            result->set_persistency(lifetime);

            return result;
        }

    private:
        template <typename Document>
        static void load_node(Document && node, builder & factory, std::string const& key, node_builder<Document> && fn) {
            auto find = node.find(key);
            if (find != node.end()) {
                fn(*find, factory);
            }
        }

        // add transform
        template <typename Document>
        static void load_transform(Document && node, builder & factory) {
            factory.add_transform({ node.value("x", 0), (node.value("y", 0)) });
        }

        // add sprite
        template <typename Document>
        static void load_sprite(Document && node, builder & factory) {
            factory.add_sprite(node);
        }

        // add body
        template <typename Document>
        static void load_body(Document && node, builder & factory) {
            auto body = factory.add_body();

            // entity
            load_entity(node, *body);

            // standings
            body->join_faction(node.value("faction", 0));

            // mass
            bool is_transparent = node.value("transparent", false);
            unsigned long long blocking_mask = node.value("traversable", 0);
            unsigned long long movement_mask = node.value("movement", 0);
            body->blocking().make_transparent(is_transparent);
            body->blocking().make_traversable(body_component::mass_type::bitset_type{ blocking_mask });
            body->movement().make_traversable(body_component::mass_type::bitset_type{ movement_mask });

            // resources
            auto hp_node = node.find("hp");
            if (hp_node != node.end()) {
                int hp = *hp_node;
                body->health().emplace(hp, hp);
            }
            auto mp_node = node.find("mp");
            if (mp_node != node.end()) {
                int mp = *mp_node;
                body->energy().emplace(mp, mp);
            }

            // equipment
            auto equipment_node = node.find("equipment");
            if (equipment_node != node.end()) {
                load_equipment(equipment_node, *body, "hand", rl::equipment::hand);
                load_equipment(equipment_node, *body, "teeth", rl::equipment::teeth);
                load_equipment(equipment_node, *body, "hide", rl::equipment::hide);
            }

            // level & exp
            body->set_experience(node.value("experience", 0));
            body->set_level(node.value("level", 0));
        }

        // add conteiner
        template <typename Document>
        static void load_container(Document && container_node, builder & factory) {
            auto container = factory.add_container();
            for (auto const & item_node : container_node["items"]) {
                container->add(make_item(item_node));
            }
        }

        // add character
        template <typename Document>
        static void load_character(Document && node, builder & factory) {
            auto person = factory.add_character();
            auto skills = node.find("skills");
            if (skills != node.end()) {
                for (auto const& i : *skills) {
                    person->learn(i);
                }
            }
            auto traits = node.find("traits");
            if (traits != node.end()) {
                for (auto const& i : *traits) {
                    person->add_trait(i);
                }
            }
        }

        // add npc ai
        template <typename Document>
        static void load_npc(Document && node, builder & factory) {
            auto ai = factory.add_npc();
            ai->set_range(node.value("idle_range", 10000), node.value("alert_range", 10000)); // zero is bad default value, use "bigenought" instead
        }

        // append useable
        template <typename Document>
        static void load_deposite(Document && node, builder & factory) {
            auto deposite = factory.add_deposite();
            deposite->set_dissolve(node.value("dissolve", true));
            deposite->set_use_duration(node.value("timer", 0));
        }

        // fill entity props
        template <typename Document>
        static void load_entity(Document && node, rl::entity & obj) {
            obj.set_tag(node.value("tag", std::string{ "" }));
            obj.set_name(node.value("name", std::string{ "" }));
            obj.set_description(node.value("description", std::string{ "" }));
        }

        // item in slot
        template <typename Document>
        static void load_equipment(Document && node, body_component & body, std::string const& key, rl::equipment slot_id) {
            auto slot_node = node->find(key);
            if (slot_node != node->end()) {
                body.get_mannequin().equip(slot_id, make_item(*slot_node));
            }
        }

        template <typename Document>
        static auto make_item(Document && node) {
            auto item = make_uq<rl::item>();
            load_entity(node, *item);

            load_effect(node, *item, "damage", rl::effect::damage);
            load_effect(node, *item, "critical", rl::effect::critical);
            load_effect(node, *item, "accuracy", rl::effect::accuracy);
            load_effect(node, *item, "dodge", rl::effect::dodge);

            // ingredient properties
            auto ingredient_node = node.find("ingredient");
            if (ingredient_node != node.end()) {
                rl::craft_activity workshop = ingredient_node->value("activity", { "" }) == "blacksmith" ? rl::craft_activity::blacksmith : rl::craft_activity::alchemy;
                item->add(rl::item::enhancement_type::real(rl::effect::ingredient_power, static_cast<int>(workshop), ingredient_node->value("power", 0)));
                item->add(rl::item::enhancement_type::integral(rl::effect::essence, 0x00, ingredient_node->value("essence", 1)));
            }

            return item;
        }

        // fill effect props
        template <typename Document>
        static void load_effect(Document && node, rl::item & obj, std::string const& key, rl::effect effect_id) {
            auto value_node = node.find(key);
            if (value_node != node.end()) {
                obj.add(rl::item::enhancement_type::real(effect_id, 0x00, *value_node));
            }
        }
    };
}