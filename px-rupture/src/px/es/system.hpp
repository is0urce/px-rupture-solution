// name: system.hpp
// type: c++ header
// desc: template class
// auth: is0urce

#pragma once

// abstract base class for system

namespace px {

	template <typename Delta>
	class system abstract
	{
	public:
		typedef Delta delta_type;

	public:
		void update(delta_type const& delta)
		{
			update_system(delta);
		}

		void fixed_update(delta_type const& delta)
		{
			fixed_update_system(delta);
		}

		void turn_update(delta_type const& delta)
		{
			turn_update_system(delta);
		}

	public:
		virtual ~system() noexcept
		{
		}

	protected:
		virtual void update_system(delta_type const& /* delta */)
		{
		}
		virtual void fixed_update_system(delta_type const& /* delta */)
		{
		}
		virtual void turn_update_system(delta_type const& /* delta */)
		{
		}
	};

}