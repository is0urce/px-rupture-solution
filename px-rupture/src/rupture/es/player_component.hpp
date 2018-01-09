// name: player_component.hpp

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include <cstdint>

namespace px {

	class player_component
		: public component
		, public link_dispatcher<player_component>
	{
	public:
		std::uint32_t get_seed() const noexcept {
			return seed;
		}

		template <typename Archive>
		void serialize(Archive & archive) {
			archive(seed);
		}

	public:
		virtual ~player_component() = default;

	private:
		std::uint32_t	seed;
	};
}