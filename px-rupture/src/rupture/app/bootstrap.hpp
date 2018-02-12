// name: bootstrap.hpp

#pragma once

#ifdef NDEBUG
#define PX_RUN_TESTS 0
#else
#define PX_RUN_TESTS 1
#endif // NDEBUG


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