#pragma once

namespace px {

	template <typename Delta>
	class system abstract
	{
	public:
		typedef Delta delta_type;

	public:
		void update(delta_type delta)
		{
			update_system(delta);
		}
		void fixed_update(delta_type delta)
		{
			fixed_update_system(delta);
		}
		void tick_update(delta_type delta)
		{
			tick_update_system(delta);
		}

	public:
		virtual ~system()
		{
		}

	protected:
		virtual void update_system(delta_type /* delta */)
		{
		}
		virtual void fixed_update_system(delta_type /* delta */)
		{
		}
		virtual void tick_update_system(delta_type /* delta */)
		{
		}
	};

}