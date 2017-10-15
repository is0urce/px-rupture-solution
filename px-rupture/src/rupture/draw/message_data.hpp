// name: message_data.hpp

#pragma once

#include "popup.hpp"

#include <px/common/color.hpp>

#include <vector>

namespace px {

	class popup;

	class message_data
	{
	public:
		unsigned int		version;
		std::vector<popup>	messages;

	public:
		message_data()
			: version(0)
		{
		}
	};
}