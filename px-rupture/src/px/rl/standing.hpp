// name: reputation_subject.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#pragma once

namespace px::rl {

	class standing
	{
	public:
		// reputation

		unsigned int current_faction() const noexcept
		{
			return faction;
		}
		void join_faction(unsigned int faction_id) noexcept
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
		constexpr standing(unsigned int faction_id) noexcept
			: faction(faction_id)
		{
		}
		constexpr standing() noexcept
			: faction(0)
		{
		}

	private:
		unsigned int faction;
	};
}