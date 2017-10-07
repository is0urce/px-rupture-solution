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
			prev = current;
		}

		// space is not serizlized
		template <typename Archive>
		void serialize(Archive & archive)
		{
			archive(current, prev);
		}

	public:
		transform() noexcept = default;
		transform(point2 position) noexcept
			: current(position)
			, prev(position)
		{
		}
		transform(transform const& position) noexcept = default;
		transform & operator=(transform const& position) noexcept = default;

	protected:
		point2			current;
		point2			prev;
	};
}