#pragma once

#include <px/rl/entity.hpp>

#include "status.hpp"

namespace px::rl {

	class item
		: public entity
		, public status
	{
	public:
		template <typename Archive>
		void serialize(Archive & archive) {
			archive(static_cast<rl::entity &>(*this));
			archive(static_cast<rl::status &>(*this));
		}

	public:
		item()
		{
		}
	};
}