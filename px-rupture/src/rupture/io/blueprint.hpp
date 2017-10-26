#pragma once

#include "rupture/es/builder.hpp"
#include "rupture/es/composite_component.hpp"
#include "rupture/es/composition_element.hpp"

#include "rupture/es/body_component.hpp"
#include "rupture/es/container_component.hpp"
#include "rupture/es/deposit_component.hpp"
#include "rupture/es/transform_component.hpp"
#include "rupture/es/sprite_component.hpp"

namespace px {

	class blueprint
	{
	public:
		template <typename Archive>
		static void save(Archive && archive, composite_component & unit)
		{
			std::string name = unit.name();
			persistency lifetime = unit.lifetime();
			archive(name);
			archive(lifetime);

			size_t total_components = unit.size();
			archive(total_components);
			unit.enumerate_components([&](auto const& part) {
				component * raw = part.get();
				if (auto transform = dynamic_cast<transform_component const*>(raw)) {
					archive(composition_element::transform);
					archive(*transform);
				}
				else if (auto sprite = dynamic_cast<sprite_component const*>(raw)) {
					archive(composition_element::sprite);
					archive(std::string(sprite->name));
				}
				else if (auto body = dynamic_cast<body_component const*>(raw)) {
					archive(composition_element::body);
					archive(*body);
				}
				else if (auto container = dynamic_cast<container_component const*>(raw)) {
					archive(composition_element::container);
					archive(*container);
				}
				//else if (auto character = dynamic_cast<character_component const*>(raw)) {
				//	archive(composition_element::character);
				//	archive(*character);
				//}
				//else if (auto storage = dynamic_cast<storage_component const*>(part.raw)) {
				//	archive(composition_element::storage);
				//}
				else if (auto deposit = dynamic_cast<deposite_component const*>(raw)) {
					archive(composition_element::deposit);
					archive(*deposit);
				}
				//else if (auto player = dynamic_cast<player_component const*>(part.raw)) {
				//	archive(composition_element::player);
				//}
				//else if (auto npc = dynamic_cast<npc_component const*>(part.raw)) {
				//	archive(composition_element::npc);
				//	archive(*npc);
				//}
				else {
					archive(composition_element::undefined);
				}
			});
		}
		template <typename Archive>
		static uq_ptr<composite_component> load(Archive && archive, builder & factory)
		{
			std::string name;
			persistency lifetime;
			archive(name);
			archive(lifetime);

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
					std::string tag;
					archive(tag);
					factory.add_sprite(tag);
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
				//case composition_element::character: {
				//	archive(*factory.add_character());
				//	break;
				//}
				//case composition_element::storage: {
				//	factory.add_storage();
				//	break;
				//}
				case composition_element::deposit: {
					archive(*factory.add_deposite());
					break;
				}
				//case composition_element::player: {
				//	factory.add_player();
				//	break;
				//}
				//case composition_element::npc: {
				//	archive(*factory.add_npc());
				//	break;
				//}
				case composition_element::undefined: {
					break; // there was unserilized (temporary) component, just skip it
				}
				default:
					// component defined, but not supported (version conflict?)
					throw std::runtime_error("px::environment::load_unit(builder, archive) - unknown component");
				}
			}

			auto unit = factory.request();
			unit->set_name(name);
			unit->set_persistency(lifetime);
			return unit;
		}
	};
}