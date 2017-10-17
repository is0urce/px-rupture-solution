// name: useable.hpp

#pragma once

namespace px {
	template <typename User, typename Environment>
	class useable
	{
	public:
		bool can_use(User user, Environment environment) const
		{
			return can_use_useable(user, environment);
		}
		void use(User user, Environment environment)
		{
			use_useable(user, environment);
		}
		bool try_use(User user, Environment environment)
		{
			bool can = can_use(user, environment);
			if (can) {
				use(user, environment);
			}
			return can;
		}

	public:
		virtual bool can_use_useable(User user, Environment environment) const
		{
		}
		virtual void use_useable(User user, Environment environment)
		{
		}
	};
}