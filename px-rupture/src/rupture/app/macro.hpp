// name: macro.hpp
// type: c++
// auth: is0urce
// desc: macro setup

#pragma once

#ifdef NDEBUG

#define PX_RUN_TESTS 0
#define PX_INGAME_PERFORMANCE_TEST 0
#define PX_INGAME_EDITOR 1              // ::DEPLOY:: set this to zero also

#else

#define PX_RUN_TESTS 1
#define PX_INGAME_PERFORMANCE_TEST 1    // fps
#define PX_INGAME_EDITOR 1

#endif // NDEBUG