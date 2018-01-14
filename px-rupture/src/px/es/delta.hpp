// name: delta.hpp
// type: c++ header
// desc: pod struct
// auth: is0urce

#pragma once

namespace px {

	struct delta {
		double			real_current;
		double			real_last;
		double			real_delta;

		double			turn_start;
		double			turn_duration;
		unsigned int	turn_number;

	public:
		void advance(double current_time) noexcept {
			real_last = real_current;
			real_delta = current_time - real_current;
			real_current = current_time;

			turn_duration = real_current - turn_start;
		}
		void advance_turn(unsigned int current_turn) noexcept {
			turn_number = current_turn;
			turn_start = real_current;
			turn_duration = 0;
		}
	public:
		delta()
			: real_current(0)
			, real_last(0)
			, real_delta(0)
			, turn_start(0)
			, turn_duration(0)
			, turn_number()
		{
		}
	};
}