// name: bigint.hpp
// type: c++ header
// desc: class with implementation
// auth: is0urce

// unsigned integer of any size

#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace px {

    class b_uint {
    public:
        using I = std::uint8_t;
        using D = std::uint16_t;
        using container_t = std::vector<I>;

    public:
        b_uint static zero() {
            return b_uint(0);
        }

        b_uint static one() {
            return b_uint(1);
        }

        void increment() {
            increment(0);
        }

        void decrement() {
            decrement(0);
        }

        void add(b_uint const& n) {
            const auto size = std::max(raw.size(), n.raw.size());
            raw.resize(size, 0);

            I carry = 0;
            for (size_t i = 0; i != size; ++i) {
                D summ = carry;
                summ += query(i);
                summ += n.query(i);
                raw[i] = summ % 256;
                carry = summ / 256 == 0 ? 0 : 1;
            }

            if (carry != 0) {
                raw.push_back(carry);
            }
        }

        void sub(b_uint const& rh) {
            I carry = 0;
            const size_t size = rh.raw.size(); // less or equal size required!
            for (size_t i = 0; i != size; ++i) {
                D acc = query(i);
                acc -= carry;
                acc -= rh.query(i);
                raw[i] = acc % 256;
                carry = acc / 256 == 0 ? 0 : 1;
            }

            trim();

            // if (carry != 0) throw exception
        }

        void multiply(b_uint const& n) {
            b_uint result;

            const size_t l_size = raw.size();
            const size_t r_size = n.raw.size();

            result.raw.resize(l_size + r_size);
            for (size_t j = 0; j != l_size; ++j) {
                for (size_t i = 0; i != r_size; ++i) {
                    D product = result.raw[i + j] + raw[j] * n.raw[i];
                    result.raw[i + j] = product % 256;
                    result.increment(i + j + 1, static_cast<I>(product / 256));
                }
            }

            result.trim();

            *this = std::move(result);
        }
        //void divide(b_uint const divisor, b_uint & quotient) {
        //	auto l_size = raw.size();
        //	auto r_size = divisor.raw.size();
        //	quotient.raw.assign(l_size, 0);

        //	//if (raw[])
        //	for (size_t i = 0; i != l_size; ++i) {
        //		size_t cursor_start = i;
        //		size_t cursor_len = r_size;
        //		if (raw)
        //	}
        //}

        bool less(b_uint const& rh) const {
            return raw.size() <= rh.raw.size() && std::lexicographical_compare(raw.cbegin(), raw.cend(), rh.raw.cbegin(), rh.raw.cend());
        }

        bool is_zero() const noexcept {
            return raw.empty();
        }

        container_t const& container() const {
            return raw;
        }

    private:
        void increment(size_t index) {
            const size_t size = raw.size();
            for (size_t i = index; i != size; ++i) {
                if (++raw[i] != 0) return;
            }
            raw.push_back(1);
        }

        void decrement(size_t index) {
            const auto size = raw.size();
            for (size_t i = index; i != size; ++i) {
                if (--raw[i] != static_cast<I>(-1)) {
                    if (i + 1 == size) raw.pop_back();
                    return;
                };
            }
            //raw.push_back(static_cast<I>(-1));
        }

        void increment(size_t index, I carry) {
            D acc = raw[index];
            acc += carry;
            raw[index] = acc % 256;

            if (acc / 256 != 0) {
                increment(index + 1);
            }
        }

        void decrement(size_t index, I carry) {
            D acc = raw[index];
            acc -= carry;
            raw[index] = acc % 256;
            if (acc / 256 != 0) {
                decrement(index + 1);
            }
        }

        I query(size_t index) const {
            return index < raw.size() ? raw[index] : 0;
        }

        void trim() {
            if (raw.empty()) return;
            if (raw.back() != 0) return;

            raw.pop_back();
            trim();
        }

    public:
        b_uint & operator+=(b_uint const& rh) {
            add(rh);
            return *this;
        }

        b_uint & operator*=(b_uint const& rh) {
            multiply(rh);
            return *this;
        }

        b_uint operator+(b_uint rh) const {
            rh.add(*this);
            return rh;
        }

        b_uint operator*(b_uint rh) const {
            rh.multiply(*this);
            return rh;
        }

    public:
        b_uint() = default;

        b_uint(unsigned long long start) {
            while (start != 0) {
                raw.push_back(start % 256);
                start /= 256;
            }
        }

        b_uint(b_uint && that)
            : raw(std::move(that.raw)) {
        }

        b_uint(b_uint const& that)
            : raw(that.raw) {
        }

        b_uint & operator=(b_uint const& lh) {
            raw = lh.raw;
            return *this;
        }

        b_uint & operator=(b_uint && lh) {
            raw = std::move(lh.raw);
            return *this;
        }

    private:
        container_t raw;
    };

    bool operator==(b_uint const& lhs, b_uint const& rhs) {
        return lhs.container() == rhs.container();
    }
    bool operator!=(b_uint const& lhs, b_uint const& rhs) {
        return lhs.container() != rhs.container();
    }
}