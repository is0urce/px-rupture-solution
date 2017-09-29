// name: coordinate_operation.hpp
// type: c++ template class
// desc: operators traits for coordinates
// auth: is0urce

#pragma once

namespace px {

	template <typename I>
	class coordinate_operation
	{
	public:
		I moved(I delta) const
		{
			delta.move(*static_cast<I const*>(this));
			return delta;
		}
		I multiplied(I stretch) const
		{
			stretch.multiply(*static_cast<I const*>(this));
			return stretch;
		}
		I divided(I const& divisor) const
		{
			I result(*static_cast<I const*>(this));
			result.divide(divisor);
			return result;
		}
		I negated() const
		{
			I result(*static_cast<I const*>(this));
			result.negate();
			return result;
		}
		I clamped(I const& min, I const& max) const
		{
			I result(*static_cast<I const*>(this));
			result.clamp(min, max);
			return result;
		}
		template <size_t Axis, typename Component>
		I moved_axis(Component s) const
		{
			I result(*static_cast<I const*>(this));
			result.move_axis<Axis>(s);
			return result;
		}

		// unary minus
		I operator-() const
		{
			I result(*static_cast<I const*>(this));
			result.negate();
			return negated;
		}

		I & operator+=(I const& rhs)
		{
			static_cast<I*>(this)->move(rhs);
			return *static_cast<I*>(this);
		}
		I & operator-=(I const& rhs)
		{
			static_cast<I*>(this)->reverse(rhs);
			return *static_cast<I*>(this);
		}
		I & operator*=(I const& rhs)
		{
			static_cast<I*>(this)->multiply(rhs);
			return *static_cast<I*>(this);
		}
		I & operator/=(I const& rhs)
		{
			static_cast<I*>(this)->divide(rhs);
			return *static_cast<I*>(this);
		}
	};
}