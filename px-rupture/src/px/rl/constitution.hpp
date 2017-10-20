// name: constitution.hpp

#pragma once

#include "bar.hpp"

#include <optional>

namespace px::rl {

	template <typename I>
	class constitution
	{
	public:
		typedef std::optional<bar<I>> resource_type;

	public:
		bool is_dead() const
		{
			return hp.has_value() && hp->empty();
		}
		bool is_alive() const
		{
			return hp.has_value() && !hp->empty();
		}

		bool has_health() const
		{
			return hp.has_value();
		}
		bool has_energy() const
		{
			return mp.has_value();
		}

		resource_type & health()
		{
			return hp;
		}
		resource_type & energy()
		{
			return mp;
		}
		resource_type const& health() const
		{
			return hp;
		}
		resource_type const& energy() const
		{
			return mp;
		}

		template <typename Archive>
		void serialize(Archive & archive)
		{
			bool has_hp = hp.has_value();
			bool has_mp = mp.has_value();
			archive(has_hp, has_mp);
			if (has_hp) archive(*hp);
			if (has_mp) archive(*mp);
		}

	public:
		constitution() noexcept
		{
		}

	private:
		resource_type hp;
		resource_type mp;
	};
}