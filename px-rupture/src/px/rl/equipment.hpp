// name: equipment.hpp

#pragma once

namespace px::rl {
	enum class equipment : unsigned char {
		not_valid = 0,
		hand, head, chest, legs
	};
}