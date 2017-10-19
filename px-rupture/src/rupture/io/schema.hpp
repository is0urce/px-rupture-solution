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

			std::string name;
			persistency lifetime = persistency::serialized;

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

			auto name_node = doc.find("name");
			if (name_node != doc.end()) {
				name = *name_node;
			}
			auto pers_node = doc.find("persistency");
			if (pers_node != doc.end()) {
				int pers = *name_node;
				lifetime = static_cast<persistency>(pers);
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
			auto tag_node = body_node->find("tag");
			if (tag_node != body_node->end()) {
				body->set_tag(*tag_node);
			}
			auto name_node = body_node->find("name");
			if (name_node != body_node->end()) {
				body->set_name(*name_node);
			}
			auto description_node = body_node->find("description");
			if (description_node != body_node->end()) {
				body->set_description(description_node.value());
			}

			// standings
			auto faction_node = body_node->find("faction");
			if (faction_node != body_node->end()) {
				body->join_faction(*faction_node);
			}

			//	// mass

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

			//	auto hp_node = body_node->find("hp");
			//	if (hp_node != body_node->end()) {
			//		body->health().create(hp_node.value());
			//	}

			//	auto mp_node = body_node->find("mp");
			//	if (mp_node != body_node->end()) {
			//		body->energy().create(mp_node.value());
			//	}
		}
	};
}