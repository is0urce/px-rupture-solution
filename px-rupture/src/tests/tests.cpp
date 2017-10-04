// name: tests.cpp

#define PX_RUN_TESTS 1

#if PX_RUN_TESTS

#include <px/dev/test.hpp>

#include "case/pool_test.hpp"
#include "case/coordinate_test.hpp"
#include "case/qtree_test.hpp"
#include "case/link_test.hpp"

#include "case/rl/traverse_test.hpp"

unsigned int px::test::total = 0;
unsigned int px::test::fails = 0;
const char * px::test::lastmsg = "not specified";

void run_tests()
{
	test_coordinate();
	test_pool();
	test_qtree();
	test_link();

	test_traverse();
	px::test::print();
}

#endif // TESTS