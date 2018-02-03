// name: craft_result.hpp

#pragma once

#include "item.hpp"
#include "craft_recipe.hpp"

#include <px/memory/uq_ptr.hpp>
#include <px/memory/memory.hpp>

#include <string>

namespace px::rl {

	class craft_result {
	public:
		using enhancement_type = rl::item::enhancement_type;
		using integer_type = enhancement_type::integer_type;
		using real_type = enhancement_type::real_type;

	public:
		static auto create_weapon(rl::craft_recipe const& recipe, integer_type essence, real_type power) {
			auto result = make_uq<rl::item>();
			setup_text(*result, recipe);
			setup_enhancements(*result, recipe, essence, power);
			result->add(enhancement_type::real(rl::effect::damage, 0, power * recipe.power_raw));
			return result;
		}
		static auto create_armor(rl::craft_recipe const& recipe, integer_type essence, real_type power) {
			auto result = make_uq<rl::item>();
			setup_text(*result, recipe);
			setup_enhancements(*result, recipe, essence, power);
			result->add(enhancement_type::real(rl::effect::armor, 0, power * recipe.power_raw));
			return result;
		}
		static uq_ptr<rl::item> create_potion(integer_type /*essence*/, real_type /*power*/) {
			auto result = make_uq<rl::item>();
			return result;
		}

	private:
		static void setup_text(rl::item & item, rl::craft_recipe const& recipe) {
			item.set_name(recipe.name);
			item.set_tag(recipe.tag);
			item.set_description(recipe.description);
		}
		static void setup_enhancements(rl::item & item, rl::craft_recipe const& recipe, integer_type essence, real_type power) {
			if (recipe.equipment_slot != rl::equipment::not_valid) {
				item.add(enhancement_type::zero(rl::effect::equipment, static_cast<integer_type>(recipe.equipment_slot)));
			}
			item.add(enhancement_type::integral(rl::effect::essence, 0, essence));
			item.add(enhancement_type::real(rl::effect::critical, 0, power * recipe.power_enhancement));
		}
	};
}