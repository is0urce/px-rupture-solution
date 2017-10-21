#pragma once

#define PX_RUN_TESTS 0

#if PX_RUN_TESTS

namespace px {
	void run_tests();
	inline void bootstrap()
	{
		run_tests();
	}
}

#else

namespace px {
	inline void bootstrap()
	{
	}
}

#endif