// name: shuffle.hpp
// type: c++ header
// desc: class
// auth: is0urce

#pragma once

#include <px/algorithm/random.hpp>

#include <algorithm>
#include <ctime>
#include <stdexcept>
#include <random>
#include <vector>
#include <utility>

namespace px {

    template <typename Value>
    class shuffle {
    public:
        using value_type = Value;
    private:
        using container_type = std::vector<value_type>;
        using engine_type = std::mt19937;
    public:
        using position_type = typename container_type::size_type;

    public:
        void enqueue(value_type element) {
            container.push_back(element);
            ++size;

            shuffle_last(0);
        }

        void enqueue_front(value_type element) {
            container.push_back(element);
            ++size;

            std::swap(container[0], container[size - 1]);
        }

        void set_threshold(position_type new_threshold) {
            threshold = new_threshold;
        }

        bool is_empty() const noexcept {
            return size == 0;
        }

        void clear() {
            container.clear();
            size = 0;
        }

        value_type & next() {
            if (size > 0) {
                std::swap(container[0], container[size - 1]);
                shuffle_last(threshold);
                return container[0];
            }
            else {
                throw std::runtime_error("px::shuffle::next() - no elements");
            }
        }

        value_type * get_next() {
            if (size > 0) {
                std::swap(container[0], contaner[size - 1]);
                shuffle_last(threshold);
                return &container[0];
            }
            else {
                return nullptr;
            }
        }

    public:
        shuffle() noexcept
            : size(0)
            , threshold(1)
            , rng(random::create_timed<engine_type>(0)) {
        }

    private:
        void shuffle_last(position_type pos) {
            if (size > 0) {
                auto const max = size - 1;
                auto const min = std::min(pos, max);
                auto const new_pos = std::uniform_int_distribution<position_type>{ min, max }(rng);
                std::swap(container[new_pos], container[size - 1]);
            }
        }

        container_type              container;
        position_type               size;
        position_type               threshold;
        engine_type                 rng;
    };
}