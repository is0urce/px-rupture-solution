// name: traits.hpp
// type: c++

#pragma once

#include "trait.hpp"

#include "rupture/es/body_component.hpp"
#include "rupture/es/character_component.hpp"

#include <cmath>
#include <map>
#include <tuple>
#include <utility>
#include <vector>

namespace px {

	class traits {
	public:
		enum class class_branch : unsigned char { not_valid, warrior, mage, archer };

	public:
		traits() {
			register_traits();
			register_options();
		}

	public:
		static unsigned int calculate_level(unsigned int experience) {
			return cap_level(level_by_experience(experience));
		}
		static unsigned int levelup_required(unsigned int experience, unsigned int level) {
			return calculate_level(experience) > level;
		}
		static unsigned int levelup_required(body_component const& body) {
			return levelup_required(body.experience(), body.level());
		}

		static class_branch specialization(character_component const& character) {
			if (character.has_trait("class_mage")) {
				return class_branch::mage;
			}
			if (character.has_trait("class_archer")) {
				return class_branch::archer;
			}
			if (character.has_trait("class_warrior")) {
				return class_branch::warrior;
			}
			return class_branch::not_valid;
		}

		void give_trait(std::string const& trait_tag, body_component & body) {
			if (trait const* feature = by_tag[trait_tag]) {
				give_trait(*feature, body);
			};
		}

		void give_trait(unsigned int trait_id, body_component & body) {
			if (trait const* feature = by_id[trait_id]) {
				give_trait(*feature, body);
			}
		}

		trait const* operator[](unsigned int trait_id) const {
			auto it = by_id.find(trait_id);
			if (it == by_id.end()) return nullptr;
			return it->second;
		}

		std::tuple<unsigned int, unsigned int, unsigned int> select_traits(body_component const& body) {
			auto level = body.level();
			std::tuple<unsigned int, unsigned int, unsigned int> result = { 0, 0, 0 };
			character_component * character = body.linked<character_component>();
			if (character) {
				result = select_traits(level, specialization(*character));
			}
			return result;
		}
		std::tuple<unsigned int, unsigned int, unsigned int> select_traits(unsigned int level, character_component const& character) {
			return select_traits(level, specialization(character));
		}
		std::tuple<unsigned int, unsigned int, unsigned int> select_traits(unsigned int level, class_branch skill_branch) {
			auto const& trait_options = trait_selection[bucket(skill_branch)][level];

			std::tuple<unsigned int, unsigned int, unsigned int> result = { 0, 0, 0 };

			if (trait_options.size() == 3) {
				result = { trait_options[0], trait_options[1], trait_options[2] };
			}
			return result;
		}

	private:
		static unsigned int level_by_experience(unsigned int experience) {
			return 1 + (static_cast<unsigned int>(std::sqrt(2500 + 200 * experience)) - 50) / 100;
		}
		static unsigned int cap_level(unsigned int level) {
			return level > 60 ? 60 : level;
		}
		static unsigned int bucket(class_branch branch) {
			switch (branch) {
			case class_branch::warrior: return 0;
			case class_branch::mage: return 1;
			case class_branch::archer: return 2;
			}
			return 0;
		}

		void add_trait(trait const& feature) {
			lib.push_back(feature);

			// vector can be reallocated
			for (auto const& record : lib) {
				by_tag[record.tag] = &record;
				by_id[record.index] = &record;
			}
		}

		void give_trait(trait const& feature, body_component & body) const {
			if (auto character = body.linked<character_component>()) {
				character->add_trait(feature.tag);
			}
			mod_stats(feature.index, body);
		}

		void mod_stats(unsigned int trait_id, body_component & /*body*/) const {
			switch (trait_id) {
			case 1:
				break;
			}
		}
		void register_traits() {
			add_trait({ 1, "class_warrior", "Strongest gnome under ground", "" });
			add_trait({ 2, "class_archer", "Stealth Archery", "" });
			add_trait({ 3, "class_mage", "Mage", "" });
		}
		void register_options() {
			trait_selection[0][0] = { 1, 2, 3 };
		}


	private:
		std::vector<trait>						lib;
		std::map<std::string, trait const*>		by_tag;
		std::map<unsigned int, trait const*>	by_id;
		std::array<std::array<std::vector<unsigned int>, 60>, 3> trait_selection;
	};
}