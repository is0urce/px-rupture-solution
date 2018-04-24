// name: craft_test.hpp

#pragma once

#include <px/dev/test.hpp>

#include <rupture/rl/craft.hpp>

using namespace px;

using namespace px::rl;

#include <array>
#include <vector>

void test_craft() {

    test::section("craft test");
    {
        test::section("hashmod dispersion");
        {
            constexpr unsigned int dispersion_size = 8;
            std::array<unsigned int, 10> primes{ 3, 5, 7, 11, 13, 17, 19, 23, 29, 31 };
            std::array<unsigned int, dispersion_size> dispersion{};
            constexpr auto max_bits = 1 << primes.size();

            for (unsigned int bits = 0; bits != max_bits; ++bits) {
                unsigned int essence = 1;
                for (unsigned int bit = 0; bit != 8; ++bit) {
                    unsigned int mask = 1 << bit;
                    if ((bits & mask) != 0) {
                        essence *= primes[bit];
                    }
                }

                auto hashmod = craft::hash_mod(essence, dispersion_size);
                ++dispersion[hashmod];
                //std::cout << hashmod << "\n";
            }

            for (unsigned int i = 0; i != dispersion.size(); ++i) {
                //std::cout << dispersion[i] << "   " << std::to_string(float(dispersion[i]) / max_bits) << "\n";
            }
        }
    }
}