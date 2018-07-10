#pragma once

#include <ctime>
#include <random>

namespace px {

    class random {
    public:
        template <typename Engine>
        static void seed_noize(Engine & rng, unsigned int extra_seed) {
            using engine_type = Engine;
            using result_type = engine_type::result_type;

            result_type noize[rng.state_size];
            std::random_device device;
            std::generate(std::begin(noize), std::end(noize), [&]() { return device() ^ extra_seed; });
            std::seed_seq seq(std::begin(noize), std::end(noize));
            rng = engine_type(seq);
        }

        template <typename Engine>
        static void seed_time(Engine & rng, unsigned int extra_seed) {
            using engine_type = Engine;

            std::time_t time;
            rng = engine_type(std::time(&time) ^ extra_seed);
        }

        template <typename Engine>
        static Engine create_timed(unsigned int extra_seed) {
            using engine_type = Engine;
            using result_type = engine_type::result_type;

            std::time_t time;
            return Engine(static_cast<result_type>(std::time(&time) ^ extra_seed));
        }
    };
}