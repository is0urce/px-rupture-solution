// name: craft_result.hpp
// type: c++
// auth: is0urce
// desc: static class-aggregated functions

#pragma once

#include "item.hpp"
#include "craft_recipe.hpp"

#include <px/memory/uq_ptr.hpp>
#include <px/memory/memory.hpp>

#include <functional>	// for hash
#include <string>		// for tags

namespace px::rl {

	class craft_result {
	public:
		using enhancement_type = rl::item::enhancement_type;
		using integer_type = enhancement_type::integer_type;
		using real_type = enhancement_type::real_type;

	public:
		static auto create_weapon(rl::craft_recipe const& recipe, integer_type essence, real_type power) {
			auto item = make_uq<rl::item>();

			setup_equipment(*item, recipe, essence, power);
			item->add(enhancement_type::real(rl::effect::damage, 0, power * recipe.power_raw));
			return item;
		}
		static auto create_armor(rl::craft_recipe const& recipe, integer_type essence, real_type power) {
			auto item = make_uq<rl::item>();

			setup_equipment(*item, recipe, essence, power);
			item->add(enhancement_type::real(rl::effect::armor, 0, power * recipe.power_raw));
			return item;
		}
		static uq_ptr<rl::item> create_potion(integer_type essence, real_type power) {
			return create_solution("potion", essence, power);
		}
		static uq_ptr<rl::item> create_solution(std::string const& name, integer_type essence, real_type power) {
			auto item = make_uq<rl::item>();
			item->setup_entity(name, "i_potion#" + std::to_string(essence));

			item->add(enhancement_type::zero(rl::effect::useable));

			switch (hash_mod(essence, 4)) {
			case 0:
				item->add(enhancement_type::real(rl::effect::hp_bonus, 0, power));
				break;
			case 1:
				item->add(enhancement_type::real(rl::effect::hp_regen, 0, power));
				break;
			case 2:
				item->add(enhancement_type::real(rl::effect::mp_bonus, 0, power));
				break;
			case 3:
				item->add(enhancement_type::real(rl::effect::mp_regen, 0, power));
				break;
			}


			item->add(enhancement_type::integral(rl::effect::essence, 0, essence)); // store essence origin

			return item;
		}

	private:
		static void setup_equipment(rl::item & item, rl::craft_recipe const& recipe, integer_type essence, real_type power) {
			item.setup_entity(recipe.name, recipe.tag, recipe.description);
			power *= recipe.power_enhancement;

			if (recipe.equipment_slot != rl::equipment::not_valid) {
				item.add(enhancement_type::zero(rl::effect::equipment, static_cast<integer_type>(recipe.equipment_slot)));
			}

			switch (hash_mod(essence, 2)) {
			case 0:
				item.add(enhancement_type::real(rl::effect::critical, 0, power));
				break;
			case 1:
				item.add(enhancement_type::real(rl::effect::dodge, 0, power));
				break;
			}


			item.add(enhancement_type::integral(rl::effect::essence, 0, essence)); // store essence origin
		}
		static unsigned int hash_mod(unsigned int x, unsigned int modulo) {
			return std::hash<unsigned int>{}(x) % modulo;
		}
	};
}