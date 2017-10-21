#pragma once

#include <px/memory/uq_ptr.hpp>

#include <functional>

namespace px {

	class script_internal;

	class script
	{
	public:
		//std::function<void()> action() { return nullptr; }
		//std::function<bool()> conditiona() { return nullptr; }
		void run(std::string const& code);
		void execute(std::string const& file);

	public:
		~script();
		script();

	private:
		uq_ptr<script_internal> works;
	};
}