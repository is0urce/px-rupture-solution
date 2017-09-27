#pragma once

namespace px {

	template <typename I>
	class coordinate_transform
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

		I operator-() const
		{
			I result(*static_cast<I const*>(this));
			result.negate();
			return negated;
		}
	};
}