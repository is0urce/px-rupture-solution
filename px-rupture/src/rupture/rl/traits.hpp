#pragma once

#include "trait.hpp"

#include "rupture/es/body_component.hpp"
#include "rupture/es/character_component.hpp"

#include <cmath>
#include <map>
#include <utility>

namespace px {

	class traits {
	public:
		traits() {
			fill();
		}

	public:
		static unsigned int calculate_level(unsigned int experience) {
			return 1 + (static_cast<unsigned int>(std::sqrt(2500 + 200 * experience)) - 50) / 100;
		}
		static unsigned int levelup_required(unsigned int experience, unsigned int level) {
			return calculate_level(experience) > level;
		}
		static unsigned int levelup_required(body_component const& body) {
			return levelup_required(body.experience(), body.level());
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

		trait const* operator[](unsigned int trait_id) {
			auto it = by_id.find(trait_id);
			if (it == by_id.end()) return nullptr;
			return it->second;
		}

	private:
		void add_trait(trait const& feature) {
			lib.push_back(feature);

			// vector can be reallocated
			for (auto const& t : lib) {
				by_tag[t.tag] = &t;
				by_id[t.index] = &t;
			}
		}

		void fill() {
			add_trait({ 1, "class_warrior", "Strongest gnome under ground", "" });
			add_trait({ 2, "class_archer", "Stealth Archery", "" });
			add_trait({ 3, "class_mage", "Mage", "" });
		}
		void give_trait(trait const& feature, body_component & body) const {
			if (auto character = body.linked<character_component>()) {
				character->add_trait(feature.tag);
			}
			apply_stats(feature.index, body);
		}

		void apply_stats(unsigned int trait_id, body_component & /*body*/) const {
			switch (trait_id) {
			case 1:
				break;
			}
		}


	private:
		std::vector<trait>						lib;
		std::map<std::string, trait const*>		by_tag;
		std::map<unsigned int, trait const*>	by_id;
	};
}