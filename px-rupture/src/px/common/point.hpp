// name: coordinate.hpp
// type: c++ header
// desc: class for 2d point with integer components

#pragma once

#include "coordinate.hpp"
#include "coordinate_transform.hpp"

namespace px {

	class point2
		: public coordinate<int, 2>
		, public coordinate_transform<point2>
	{
	public:
		constexpr component x() const noexcept
		{
			return m_array[0];
		}
		constexpr component y() const noexcept
		{
			return m_array[1];
		}

		point2 & operator+=(point2 const& rhs) { move(rhs); return *this; }
		point2 & operator-=(point2 const& rhs) { reverse(rhs); return *this; }
		point2 & operator*=(point2 const& rhs) { multiply(rhs); return *this; }
		point2 & operator/=(point2 const& rhs) { divide(rhs); return *this; }

		point2 & operator*=(component c) { multiply(c); return *this; };
		point2 & operator/=(component c) { divide(c); return *this; };

	public:
		// default constructor is for uninitialized state
		// use point2(0, 0) for constexpr
		point2() noexcept
		{
		}
		constexpr point2(component x, component y) noexcept
			: coordinate(x, y)
		{
		}
		constexpr point2(point2 const&) noexcept = default;
	};

	namespace
	{
		point2 operator+(point2 lhs, point2 const& rhs) { lhs += rhs; return lhs; }
		point2 operator-(point2 lhs, point2 const& rhs) { lhs -= rhs; return lhs; }
		point2 operator*(point2 lhs, point2 const& rhs) { lhs *= rhs; return lhs; }
		point2 operator/(point2 lhs, point2 const& rhs) { lhs /= rhs; return lhs; }
		point2 operator*(point2 lhs, point2::component c) { lhs *= c; return lhs; }
		point2 operator/(point2 lhs, point2::component c) { lhs /= c; return lhs; }
	}
}