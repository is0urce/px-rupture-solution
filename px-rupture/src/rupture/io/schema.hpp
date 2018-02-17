// name: schema.hpp
// type: c++ header
// auth: is0urce
// desc: template class

#pragma once

// build unit from document

#include "rupture/es/builder.hpp"
#include "rupture/es/body_component.hpp"
#include "rupture/es/container_component.hpp"
#include "rupture/es/character_component.hpp"

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
		static void load_transform(Document && transform_node, builder & factory) {
			point2 position(transform_node.value("x", 0), (transform_node.value("y", 0)));
			factory.add_transform(position);
		}

		// add sprite
		template <typename Document>
		static void load_sprite(Document && sprite_node, builder & factory) {
			std::string src = sprite_node;
			factory.add_sprite(src);
		}

		// add body
		template <typename Document>
		static void load_body(Document && body_node, builder & factory) {
			auto body = factory.add_body();

			// entity
			body->set_tag(body_node.value("tag", std::string{ "" }));
			body->set_name(body_node.value("name", std::string{ "" }));
			body->set_description(body_node.value("description", std::string{ "" }));

			// standings
			body->join_faction(body_node.value("faction", 0));

			// mass
			bool is_transparent = body_node.value("transparent", false);
			unsigned long long blocking_mask = body_node.value("traversable", 0);
			unsigned long long movement_mask = body_node.value("movement", 0);
			body->blocking().make_transparent(is_transparent);
			body->blocking().make_traversable(body_component::mass_type::bitset_type{ blocking_mask });
			body->movement().make_traversable(body_component::mass_type::bitset_type{ movement_mask });

			// resources
			auto hp_node = body_node.find("hp");
			if (hp_node != body_node.end()) {
				int hp = *hp_node;
				body->health().emplace(hp, hp);
			}
			auto mp_node = body_node.find("mp");
			if (mp_node != body_node.end()) {
				int mp = *mp_node;
				body->energy().emplace(mp, mp);
			}

			// level & exp
			body->set_experience(body_node.value("experience", 0));
			body->set_level(body_node.value("level", 0));
		}

		// add conteiner
		template <typename Document>
		static void load_container(Document && /*container_node*/, builder & factory) {
			factory.add_container();
		}

		// add character
		template <typename Document>
		static void load_character(Document && /*character_node*/, builder & factory) {
			factory.add_character();
		}
	};
}