// name: matrix_dynamic.hpp
// type: c++ header

#pragma once

#include <px/common/matrix_base.hpp>

#include <algorithm>
#include <vector>
#include <stdexcept>

namespace px {

	// matrix with varying size

	template <typename Element>
	class matrix2<Element>
	{
	public:
		typedef Element element_type;

	public:
		size_t width() const noexcept
		{
			return m_width;
		}
		size_t height() const noexcept
		{
			return m_height;
		}
		size_t size() const noexcept
		{
			return m_width * m_height;
		}

		void resize(size_t w, size_t h)
		{
			m_width = w;
			m_height = h;
			m_data.resize(w * h);
		}
		template <typename Pair>
		void resize(Pair && range)
		{
			resize(range.get<0>(), range.get<1>());
		}

		void swap(matrix2 & that)
		{
			std::swap(m_height, that.m_height);
			std::swap(m_width, that.m_width);
			std::swap(m_data, that.m_data);
		}

		bool empty() const noexcept
		{
			return m_width == 0 || m_height == 0;
		}

		bool contains(size_t x, size_t y) const noexcept
		{
			return x < m_width && y < m_height;
		}
		template <typename Pair>
		bool contains(Pair && position) const
		{
			return contains(position.get<0>(), position.get<1>());
		}

		void fill(element_type const& element_value)
		{
			std::fill(m_data.begin(), m_data.end(), element_value);
		}
		template <typename Generator>
		void generate(Generator && op)
		{
			size_t index = 0;
			for (size_t j = 0; j != m_height; ++j) {
				for (size_t i = 0; i != m_width; ++i) {
					m_data[index] = op(i, j);
					++index;
				}
			}
		}
		template <typename Operator>
		void enumerate(Operator && op)
		{
			size_t index = 0;
			for (size_t j = 0; j != m_height; ++j) {
				for (size_t i = 0; i != m_width; ++i) {
					op(i, j, m_data[index]);
					++index;
				}
			}
		}
		template <typename Operator>
		void enumerate(Operator && op) const
		{
			size_t index = 0;
			for (size_t j = 0; j != m_height; ++j) {
				for (size_t i = 0; i != m_width; ++i) {
					op(i, j, m_data[index]);
					++index;
				}
			}
		}

		// querry functions: operator[] not throws, at() throws, select returns default (outer) if out of range
		// specialized point2 acessors for easy querry with bracket-initialized points

		element_type const& get(size_t x, size_t y) const
		{
			return m_data[y * m_width + x];
		}
		element_type & get(size_t x, size_t y)
		{
			return m_data[y * m_width + x];
		}
		template <typename Pair>
		element_type const& get(Pair && key) const
		{
			return m_data[key.get<1>() * m_width + key.get<0>()];
		}
		template <typename Pair>
		element_type & get(Pair && key)
		{
			return m_data[key.get<1>() * m_width + key.get<0>()];
		}
		template <size_t X, size_t Y>
		element_type const& get() const
		{
			return m_data[Y * m_width + X];
		}
		template <size_t X, size_t Y>
		element_type & get()
		{
			return m_data[Y * m_width + X];
		}

		template <typename Pair>
		element_type const& operator[](Pair && key) const
		{
			return m_data[key.get<1>() * m_width + key.get<0>()];
		}
		template <typename Pair>
		element_type & operator[](Pair && key)
		{
			return m_data[key.get<1>() * m_width + key.get<0>()];
		}

		template <typename Pair>
		element_type const& at(Pair && key) const
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e>::at() - out of range");
			return (*this)[key];
		}
		template <typename Pair>
		element_type & at(Pair && key)
		{
			if (!contains(key)) throw std::runtime_error("px::matrix<e>::at() - out of range");
			return (*this)[key];
		}
		element_type const& at(size_t x, size_t y) const
		{
			if (!contains(x, y)) throw std::runtime_error("px::matrix<e>::at(x, y) - out of range");
			return m_data[y * m_width + x];
		}
		element_type & at(size_t x, size_t y)
		{
			if (!contains(x, y)) throw std::runtime_error("px::matrix<e>::at(x, y) - out of range");
			return m_data[y * m_width + x];
		}

		element_type const& get_or(element_type const& outer, size_t x, size_t y) const
		{
			return contains(x, y) ? m_data[y * m_width + x] : outer;
		}
		element_type & get_or(element_type & outer, size_t x, size_t y)
		{
			return contains(x, y) ? m_data[y * m_width + x] : outer;
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
			: m_width(0)
			, m_height(0)
		{
		}
		matrix2(size_t w, size_t h)
			: m_width(w), m_height(h)
		{
			m_data.resize(w * h);
		}
		matrix2(size_t w, size_t h, element_type const& initial)
			: m_width(w)
			, m_height(h)
		{
			m_data.assign(w * h, initial);
		}
		template <typename Pair>
		matrix2(Pair && range)
			: matrix2(range.get<0>(), range.get<1>())
		{
		}
		template <typename Generator>
		matrix2(size_t w, size_t h, Generator && op, int /*execution_option*/)
			: matrix2(w, h)
		{
			generate(std::forward<Generator>(op));
		}
		matrix2(matrix2 const&) = delete;
		matrix2 & operator=(matrix2 const&) = delete;
		matrix2(matrix2 && that) = default;
		matrix2 & operator=(matrix2 && that) = default;

	private:
		std::vector<element_type>	m_data;
		size_t						m_width;
		size_t						m_height;
	};
}