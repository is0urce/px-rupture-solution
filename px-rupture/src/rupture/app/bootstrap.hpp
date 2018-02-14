// name: bootstrap.hpp
// type: c++
// auth: is0urce
// desc: macro setup

#pragma once

#include "macro.hpp"

#if PX_RUN_TESTS

namespace px {

	void run_tests();

	inline void bootstrap() {
		run_tests();
	}
}

#else

namespace px {

	inline void bootstrap() {
	}
}

#endif