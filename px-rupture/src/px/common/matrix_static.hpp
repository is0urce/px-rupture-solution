// name: matrix_static.hpp
// type: c++ header
// desc: base template class for points and vectors

#pragma once

#include <px/common/matrix_base.hpp>

#include <algorithm>
#include <array>
#include <stdexcept>

namespace px {

	template <typename Element, size_t W, size_t H>
	class matrix2<Element, W, H> {
	public:
		using element_type = Element;

	public:
		static constexpr size_t width() noexcept
		{
			return W;
		}
		static constexpr size_t height() noexcept
		{
			return H;
		}
		static constexpr size_t size() noexcept
		{
			return W * H;
		}

		void swap(matrix2 & that)
		{
			std::swap(m_data, that.m_data);
		}
		void copy(matrix2 const& that)
		{
			std::copy(that.m_data[0], that.m_data[W * H], m_data[0]);
		}

		static constexpr bool contains(size_t x, size_t y) noexcept
		{
			return x < W && y < H;
		}
		template <typename Pair>
		static constexpr bool contains(Pair && key)
		{
			return contains(key.get<0>(), key.get<1>());
		}

		void fill(element_type const& e)
		{
			m_data.fill(e);
		}
		template <typename Generator>
		void generate(Generator && op)
		{
			size_t index = 0;
			for (size_t j = 0; j != H; ++j) {
				for (size_t i = 0; i != W; ++i) {
					m_data[index] = op(i, j);
					++index;
				}
			}
		}

		template <typename Operator>
		void enumerate(Operator && op)
		{
			size_t index = 0;
			for (size_t j = 0; j != H; ++j) {
				for (size_t i = 0; i != W; ++i) {
					op(i, j, m_data[index]);
					++index;
				}
			}
		}
		template <typename Operator>
		void enumerate(Operator && op) const
		{
			size_t index = 0;
			for (size_t j = 0; j != H; ++j) {
				for (size_t i = 0; i != W; ++i) {
					op(i, j, m_data[index]);
					++index;
				}
			}
		}

		// querry functions: operator[] not throws, at() throws, get_or returns default (outer) if out of range
		// template acessors for easy querry with tuple-likes

		element_type const& get(size_t x, size_t y) const
		{
			return m_data[y * W + x];
		}
		element_type & get(size_t x, size_t y)
		{
			return m_data[y * W + x];
		}
		template <typename Pair>
		element_type const& get(Pair && key) const
		{
			return m_data[key.get<1>() * W + key.get<0>()];
		}
		template <typename Pair>
		element_type & get(Pair && key)
		{
			return m_data[key.get<1>() * W + key.get<0>()];
		}
		template <size_t X, size_t Y>
		element_type const& get() const
		{
			static_assert(contains(X, Y));
			return m_data[Y * W + X];
		}
		template <size_t X, size_t Y>
		element_type & get()
		{
			static_assert(contains(X, Y));
			return m_data[Y * W + X];
		}

		template <typename Pair>
		element_type const& operator[](Pair && key) const
		{
			return m_data[key.get<1>() * W + key.get<0>()];
		}
		template <typename Pair>
		element_type & operator[](Pair && key)
		{
			return m_data[key.get<1>() * W + key.get<0>()];
		}

		template <typename Pair>
		element_type const& at(Pair && key) const
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e,w,h>::at<pair>(key) const - key out of range");
			return operator[](key);
		}
		template <typename Pair>
		element_type & at(Pair && key)
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e,w,h>::at<pair>(key) - key out of range");
			return operator[](key);
		}
		element_type const& at(size_t x, size_t y) const
		{
			if (!contains(x, y)) throw std::runtime_error("px::matrix<e,w,h>::at() - out of range");
			return m_data[y * W + x];
		}
		element_type & at(size_t x, size_t y)
		{
			if (!contains(x, y)) throw std::runtime_error("px::matrix<e,w,h>::at() - out of range");
			return m_data[y * W + x];
		}

		element_type const& get_or(element_type const& outer, size_t x, size_t y) const
		{
			return contains(x, y) ? m_data[y * W + x] : outer;
		}
		element_type & get_or(element_type & outer, size_t x, size_t y)
		{
			return contains(x, y) ? m_data[y * W + x] : outer;
		}
		template <typename Pair>
		element_type const& get_or(element_type const& outer, Pair && key) const
		{
			return contains(key) ? operator[](key) : outer;
		}
		template <typename Pair>
		element_type & get_or(element_type & outer, Pair && key)
		{
			return contains(key) ? operator[](key) : outer;
		}

	public:
		matrix2()
		{
		}
		matrix2(element_type const& initial)
		{
			fill(initial);
		}
		template <typename Generator>
		matrix2(Generator && op, int /*execution_option*/)
		{
			generate(std::forward<Generator>(op));
		}
		matrix2(matrix2 const&) = delete;
		matrix2 & operator=(matrix2 const&) = delete;
		//matrix2(matrix2 &&) = default;
		//matrix2 & operator=(matrix2 &&) = default;

	private:
		std::array<element_type, W * H> m_data;
	};
}