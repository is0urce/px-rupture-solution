// name: constitution.hpp

#pragma once

#include "bar.hpp"

#include <optional>

namespace px::rl {

	class constitution
	{
	public:
		typedef std::optional<bar<int>> resource_type;

	public:
		bool is_dead() const
		{
			return health.has_value() && health->empty();
		}
		bool is_alive() const
		{
			return health.has_value() && health->empty();
		}

		bool has_health() const
		{
			return health.has_value();
		}
		bool has_energy() const
		{
			return health.has_value();
		}

		resource_type & hp()
		{
			return health;
		}
		resource_type & mp()
		{
			return health;
		}

		template <typename Archive>
		void serialize(Archive & archive)
		{
			bool has_hp = health.has_value();
			bool has_mp = health.has_value();
			archive(has_hp, has_mp);
			if (has_hp) archive(*health);
			if (has_mp) archive(*energy);
		}

	public:
		constitution() noexcept
		{
		}

	private:
		resource_type health;
		resource_type energy;
	};
}