// name: player_component.hpp
// type: c++ header
// auth: is0urce

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
		void set_seed(std::uint32_t seed_value) {
			seed = seed_value;
		}

		void give_experience(std::uint32_t experience) {
			experience_pool += experience;
		}
		void set_experience(std::uint32_t experience) {
			experience_pool = experience;
		}

		template <typename Archive>
		void serialize(Archive & archive) {
			archive(seed, experience_pool);
		}

	public:
		virtual ~player_component() = default;
		player_component()
			: seed(0)
			, experience_pool(0)
		{
		}

	private:
		std::uint32_t	seed;
		std::uint32_t	experience_pool;
	};
}