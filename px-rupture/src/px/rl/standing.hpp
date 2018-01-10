// name: standing.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#pragma once

namespace px::rl {

	template <typename Faction>
	class standing {
	public:
		using faction_type = Faction;
		// reputation

		Faction current_faction() const noexcept
		{
			return faction;
		}
		void join_faction(Faction faction_id) noexcept
		{
			faction = faction_id;
		}
		void quit_faction() noexcept
		{
			faction = 0;
		}

		int reputation(standing const& subject) const noexcept
		{
			return faction == subject.faction ? 100 : -100;
		}
		bool is_hostile(standing const& subject) const noexcept
		{
			return reputation(subject) < 0;
		}
		bool is_ally(standing const& subject) const noexcept
		{
			return reputation(subject) > 0;
		}
		bool is_neutral(standing const& subject) const noexcept
		{
			return reputation(subject) == 0;
		}

		template <typename Archive>
		void serialize(Archive & archive)
		{
			archive(faction);
		}

	public:
		constexpr standing(Faction faction_id) noexcept
			: faction(faction_id)
		{
		}
		constexpr standing() noexcept
			: faction(0)
		{
		}

	private:
		Faction faction;
	};
}