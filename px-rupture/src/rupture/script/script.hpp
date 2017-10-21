#pragma once

#include <px/memory/uq_ptr.hpp>

#include <functional>

#include <px/common/point.hpp>
#include <px/rl/skill/skill_functional.hpp>

namespace px {

	class body_component;
	class script_internal;
	class skill;

	class script
	{
	public:
		void run(std::string const& code);
		void execute(std::string const& file);
		skill & impact(std::string const& name);

	public:
		~script();
		script();

	private:
		uq_ptr<script_internal> works;
	};
}