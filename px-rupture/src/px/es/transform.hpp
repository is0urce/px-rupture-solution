// transform.hpp

#pragma once

#include <px/common/point.hpp>
#include <px/common/qtree.hpp>

namespace px {

	template <typename R>
	class transform
	{
	public:
		typedef qtree<R*> space_type;

	public:
		auto const& position() const noexcept
		{
			return current;
		}
		auto const& last() const noexcept
		{
			return prev;
		}
		void store()
		{
			current = prev;
		}

		// move by offset
		void move(point2 const& direction)
		{
			place(m_current + direction);
		}

		// place into specified location
		void place(point2 destination)
		{
			if (materialized && space) {
				space->move(current, static_cast<R*>(this), destination);
			}
			current = destination;
		}

		void incarnate(space_type * world)
		{
			if (materialized) {
				if (space) space->remove(current, static_cast<R*>(this));
				if (world) world->add(current, static_cast<R*>(this));
			}
			space = world;
		}
		void insert()
		{
			if (!inside && space) {
				space->add(current, static_cast<R*>(this));
			}
			inside = true;
		}
		void retract()
		{
			if (inside && space) {
				space->remove(current, static_cast<R*>(this));
			}
			inside = false;
		}

		space_type * world() const noexcept
		{
			return space;
		}

		// space is not serizlized
		template <typename Archive>
		void serialize(Archive & archive)
		{
			archive(current, prev);
		}

	public:
		transform() noexcept
			: space(nullptr)
			, inside(false)
		{
		}

	private:
		space_type *	space;
		point2			current;
		point2			prev;
		bool			inside;
	};
}