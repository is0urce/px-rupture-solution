// name: engine.hpp
// type: c++ header
// desc: template class
// auth: is0urce

#pragma once

// updates dispatcher

#include <vector>

#include <px/es/system.hpp>

namespace px {

	template <typename Delta>
	class engine {
	public:
		using delta_type = Delta;
		using system_type = system<delta_type>;

	public:
		void add(system_type * system)
		{
			systems.push_back(system);
		}
		void clear()
		{
			systems.clear();
		}
		void update(delta_type const& delta)
		{
			for (system_type * system : systems) {
				system->update(delta);
			}
		}
		void fixed_update(delta_type const& delta)
		{
			for (system_type * system : systems) {
				system->fixed_update(delta);
			}
		}
		void turn_update(delta_type const& delta)
		{
			for (system_type * system : systems) {
				system->turn_update(delta);
			}
		}

	private:
		std::vector<system_type*> systems;
	};
}