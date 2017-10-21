#pragma once

// name: character_works.hpp

#pragma once

#include "character_component.hpp"

#include <px/memory/memory.hpp>

#include <px/rl/skill/skill_functional.hpp>

#include "rupture/rl/skill.hpp"

#include <map>
#include <optional>
#include <string>
#include <tuple>

namespace px {

	class character_works
	{
	public:
		uq_ptr<character_component> make()
		{
			auto result = make_uq<character_component>();
			result->assign_book(&book);
			return result;
		}
		void turn()
		{

		}
		void add(std::string const& name, skill::state_type const& start, rl::skill_functional<body_component *, body_component *, point2> && impact)
		{
			auto & fn = lib[name] = std::move(impact);
			book[name].emplace(start, &fn);
		}

	private:
		std::map<std::string, rl::skill_functional<body_component *, body_component *, point2>> lib;
		std::map<std::string, std::optional<std::tuple<skill::state_type, skill::impact_type * >>> book;
	};
}