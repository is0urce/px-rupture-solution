// name: bar.hpp
// type: c++ header
// desc: template struct
// auth: is0urce

// resource bar
// value that have maximal cap
// starting value is maximal

#pragma once

#include <algorithm>

namespace px::rl {

	template <typename T>
	class bar
	{
	public:
		using value_type = T;

	public:
		// querry

		T current() const noexcept {
			return m_current;
		}
		T maximum() const noexcept {
			return m_max;
		}
		bool empty() const noexcept {
			return m_current <= 0;
		}
		bool full() const noexcept {
			return m_current == m_max;
		}

		// restore value to maximum

		void restore() noexcept {
			m_current = m_max;
		}
		void restore(T magnitude) noexcept {
			m_current = cap(m_current + magnitude);
		}
		void modify(T magnitude) noexcept {
			m_current = cap(m_current + magnitude);
		}
		void damage(T magnitude) noexcept {
			m_current = cap(m_current - magnitude);
		}

		// mutation operators

		bar & operator=(T c) noexcept
		{
			set(c);
			return *this;
		}
		bar & operator+=(T c) noexcept
		{
			modify(c);
			return *this;
		}
		bar & operator-=(T c) noexcept
		{
			modify(-c);
			return *this;
		}

		// comparison operators

		bool operator<(T c) const noexcept
		{
			return m_current < c;
		}
		bool operator==(T c) const noexcept
		{
			return m_current == c;
		}
		bool operator<=(T c) const noexcept
		{
			return operator<(*this, c) || operator==(this*, c);
		}
		bool operator>=(T c) const noexcept
		{
			return !operator<(*this, c);
		}
		bool operator!=(T c) const noexcept
		{
			return !operator==(*this, c);
		}
		bool operator>(T c) const noexcept
		{
			return !operator<=(*this, c);
		}

		// value cast operators

		operator value_type() const noexcept
		{
			return m_current;
		}
		operator bool() const noexcept
		{
			return !empty();
		}

		void set_current(T current) noexcept
		{
			m_current = cap(current);
		}
		void set_maximum(T max) noexcept
		{
			m_max = max;
			m_current = cap(m_current);
		}
		void set(T current_and_max) noexcept
		{
			m_max = current_and_max;
			m_current = current_and_max;
		}
		void set(T current, T max) noexcept
		{
			m_max = max;
			m_current = cap(current);
		}

		// io
		template <typename Archive>
		void serialize(Archive & archive)
		{
			archive(m_current, m_max);
		}

		// ctor & dtor
	public:
		constexpr bar() noexcept
			: m_current(0)
			, m_max(0)
		{
		}
		constexpr bar(T max) noexcept
			: m_current(max)
			, m_max(max)
		{
		}
		constexpr bar(T current, T max) noexcept
			: m_current(current)
			, m_max(max)
		{
		}
		bar(bar const&) = default;
		bar & operator=(bar const&) noexcept = default;

	private:
		T cap(T val) const noexcept
		{
			return std::min(val, m_max);
		}

	private:
		T m_current;
		T m_max;
	};
}