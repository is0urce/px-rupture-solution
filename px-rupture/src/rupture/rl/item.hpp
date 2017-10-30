#pragma once

#include <px/rl/entity.hpp>
#include <px/rl/enhancement_collection.hpp>
#include <px/rl/effect.hpp>

#include <cstdint>

namespace px::rl {

	class item
		: public entity
		, public rl::enhancement_collection<rl::effect, std::int32_t, double>
	{
	public:
		template <typename Archive>
		void serialize(Archive & archive) {
			archive(static_cast<rl::entity &>(*this));
			archive(static_cast<rl::enhancement_collection<rl::effect, std::int32_t, double> &>(*this));
		}

	public:
		item()
		{
		}
	};
}