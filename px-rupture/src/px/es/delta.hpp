// name: basic_delta.hpp
// type: c++ header
// desc: pod struct
// auth: is0urce

#pragma once

namespace px {

	struct delta
	{
	public:
		double simulation_time;		// time from start of simulation in simulation world units
		double simulation_last;		// time of last simulation update
		double simulation_delta;	// time span from last simulation update in simulation world units
		double simulation_start;	// real time of simulation start

		double real_time;			// time from start, real time in seconds
		double real_delta;			// time from last update
		double real_last;			// last update time

		double fixed_delta;			// duration of fixed update span
		unsigned int tick_index;	// index of current tick or turn

	public:
		void advance(double new_real)
		{
			real_last = real_time;
			real_delta = new_real - real_time;
			real_time = new_real;

			simulation_last = simulation_time;
			simulation_delta = real_delta;
			simulation_time = new_real - simulation_start;
		}
		void restart_real(double start_time)
		{
			real_time = start_time;
			real_last = start_time;
			real_delta = 0;
		}
		void restart_real()
		{
			restart_real(0);
		}
		void restart_simulation(double start_time)
		{
			simulation_time = start_time;
			simulation_last = start_time;
			simulation_delta = 0;
			simulation_start = real_time;
		}
		void restart_simulation()
		{
			restart_simulation(0);
		}
		void restart_ticks(unsigned int start_tick)
		{
			tick_index = start_tick;
		}
		void restart_ticks()
		{
			restart_ticks(0);
		}
		void restart()
		{
			restart_real();
			restart_simulation();
			restart_ticks();
		}
	};
}