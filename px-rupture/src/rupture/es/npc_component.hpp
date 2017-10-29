// name: npc_component.hpp

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include <px/common/point.hpp>
#include <px/rl/ai_state.hpp>

#include <cstdint>

namespace px {

	class transform_component;

	class npc_component
		: public component
		, public link<transform_component>
		, public link_dispatcher<npc_component>
	{
	public:
		rl::ai_state get_state() const noexcept
		{
			return state;
		}
		std::uint32_t get_range() const noexcept
		{
			return perception_range;
		}
		void set_state(rl::ai_state current) noexcept
		{
			state = current;
		}
		void set_range(std::uint32_t range) noexcept
		{
			perception_range = range;
		}
		point2 & destination()
		{
			return waypoint;
		}

		template <typename Archive>
		void serialize(Archive & archive) {
			archive(state, perception_range, waypoint);
		}

	public:
		virtual ~npc_component() = default;
		npc_component()
			: state(rl::ai_state::idle)
			, perception_range(0)
			, waypoint{ 0, 0 }
		{
		}
		npc_component(npc_component const&) = delete;
		npc_component & operator=(npc_component const&) = delete;

	private:
		rl::ai_state	state;
		std::uint32_t	perception_range;
		point2			waypoint;
	};
}