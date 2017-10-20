#pragma once

#include "rupture/es/builder.hpp"
#include "rupture/es/body_component.hpp"

namespace px {

	class schema
	{
	public:
		template <typename Document>
		static auto load(Document && doc, builder & factory)
		{
			factory.begin();

			std::string name = doc.value("name", std::string{});
			persistency lifetime = static_cast<persistency>(doc.value("persistency", static_cast<int>(persistency::serialized)));

			auto transform_node = doc.find("transform");
			if (transform_node != doc.end()) {
				load_transform(transform_node, factory);
			}

			auto sprite_node = doc.find("sprite");
			if (sprite_node != doc.end()) {
				load_sprite(sprite_node, factory);
			}

			auto body_node = doc.find("body");
			if (body_node != doc.end()) {
				load_body(body_node, factory);
			}

			auto result = factory.request();
			result->set_name(name);
			result->set_persistency(lifetime);
			return result;
		}

		template <typename Document>
		static void load_transform(Document && transform_node, builder & factory)
		{
			point2 position;
			position.set<0>(transform_node->at("x"));
			position.set<1>(transform_node->at("y"));
			factory.add_transform(position);
		}

		template <typename Document>
		static void load_sprite(Document && sprite_node, builder & factory)
		{
			std::string src = *sprite_node;
			factory.add_sprite(src);
		}

		// body
		template <typename Document>
		static void load_body(Document && body_node, builder & factory)
		{
			auto body = factory.add_body();

			// entity
			body->set_tag(body_node->value("tag", std::string{ "" }));
			body->set_name(body_node->value("name", std::string{ "" }));
			body->set_description(body_node->value("description", std::string{ "" }));

			// standings
			body->join_faction(body_node->value("faction", 0));

			// mass
			bool is_transparent = body_node->value("transparent", false);
			unsigned long long blocking_mask = body_node->value("traversable", 0);
			unsigned long long movement_mask = body_node->value("movement", 0);
			body->blocking().make_transparent(is_transparent);
			body->blocking().make_traversable(body_component::mass_type::bitset_type{ blocking_mask });
			body->movement().make_traversable(body_component::mass_type::bitset_type{ movement_mask });

			//	auto transperency_node = body_node->find("transparent");
			//	if (transperency_node != body_node->end()) {
			//		body->mass().make_transparent(*transperency_node);
			//	}

			//	auto traversable_node = body_node->find("traversable");
			//	if (traversable_node != body_node->end()) {
			//		for (unsigned int layer : *traversable_node) {
			//			body->mass().make_traversable(static_cast<rl::traverse>(layer));
			//		}
			//	}

			//	auto traverse_node = body_node->find("traverse");
			//	if (traverse_node != body_node->end()) {
			//		for (unsigned int layer : *traverse_node) {
			//			body->traverse().make_traversable(static_cast<rl::traverse>(layer));
			//		}
			//	}

			//	// resources

			auto hp_node = body_node->find("hp");
			if (hp_node != body_node->end()) {
				int hp = *hp_node;
				body->health().emplace(hp, hp);
			}

			auto mp_node = body_node->find("mp");
			if (mp_node != body_node->end()) {
				int mp = *mp_node;
				body->energy().emplace(mp, mp);
			}
		}
	};
}