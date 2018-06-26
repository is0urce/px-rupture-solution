// name: tests.cpp

#include "rupture/app/bootstrap.hpp"

#if PX_RUN_TESTS

#include <px/dev/test.hpp>

#include "case/common/bigint_test.hpp"
#include "case/common/pool_test.hpp"
#include "case/common/coordinate_test.hpp"
#include "case/common/qtree_test.hpp"
#include "case/common/matrix_test.hpp"
#include "case/common/memory_test.hpp"
#include "case/common/label_test.hpp"
#include "case/common/row_test.hpp"

#include "case/link_test.hpp"

#include "case/rl/traverse_test.hpp"
#include "case/rl/craft_test.hpp"

#include <px/dev/benchmark.hpp>

namespace px {

    unsigned int test::total = 0;
    unsigned int test::fails = 0;
    const char * test::lastmsg = "not specified";

    void run_tests() {
        test_coordinate();
        test_pool();
        test_qtree();
        test_link();
        test_matrix();
        test_memory();
        test_bigint();
        test_row();
        test_label();

        test_traverse();
        test_craft();
        test::print();
    }
}

#endif // TESTS