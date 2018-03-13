// name: hit_result.hpp

#pragma once

namespace px::rl {
    enum class hit_result {
        miss = 0,
        connects = 1,
        critical = 2,
        failure = 3
    };
}