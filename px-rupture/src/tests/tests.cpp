// name: tests.cpp

#define PX_RUN_TESTS 1

#if PX_RUN_TESTS

#include <px/dev/test.hpp>

#include "case/common/pool_test.hpp"
#include "case/common/coordinate_test.hpp"
#include "case/common/qtree_test.hpp"
#include "case/common/matrix_test.hpp"
#include "case/common/memory_test.hpp"

#include "case/rl/traverse_test.hpp"

#include "case/link_test.hpp"

unsigned int px::test::total = 0;
unsigned int px::test::fails = 0;
const char * px::test::lastmsg = "not specified";

void run_tests()
{
	test_coordinate();
	test_pool();
	test_qtree();
	test_link();
	test_matrix();

	test_traverse();
	px::test::print();
}

#endif // TESTS