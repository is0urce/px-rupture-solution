// name: character_component.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include "rupture/rl/skill.hpp"
#include <px/rl/skill/skill_set.hpp>

#include <algorithm>	// remove
#include <string>		// key
#include <vector>		// container

namespace px {

	class character_component
		: public component
		, public link_dispatcher<character_component>
		, public rl::skill_set<skill>
	{
	public:
		void add_trait(std::string trait) {
			traits.push_back(trait);
		}

		void remove_trait(size_t idx) {
			if (idx < traits.size()) {
				traits[idx] = std::move(traits.back());
				traits.pop_back();
			}
		}

		void remove_trait(std::string const& tag) {
			std::remove(traits.begin(), traits.end(), tag);
		}

		void clear_traits() {
			traits.clear();
		}

		bool has_trait(std::string const& match) const noexcept {
			for (std::string const& trait : traits) {
				if (trait == match) return true;
			}
			return false;
		}

		template <typename Archive>
		void serialize(Archive & archive) {
			archive(static_cast<rl::skill_set<skill> &>(*this));
			archive(traits);
		}

	public:
		virtual ~character_component() = default;
		character_component() = default;
		character_component(character_component const&) = delete;
		character_component const& operator=(character_component const&) = delete;

	private:
		std::vector<std::string> traits;
	};
}