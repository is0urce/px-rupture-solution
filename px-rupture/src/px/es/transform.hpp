// transform.hpp

#pragma once

#include <px/common/point.hpp>

namespace px {

	class transform
	{
	public:
		point2 const& position() const noexcept
		{
			return current;
		}
		point2 const& last() const noexcept
		{
			return prev;
		}
		void store()
		{
			current = prev;
		}

		// space is not serizlized
		template <typename Archive>
		void serialize(Archive & archive)
		{
			archive(current, prev);
		}

	public:
		transform() noexcept = default;

	protected:
		point2			current;
		point2			prev;
	};
}