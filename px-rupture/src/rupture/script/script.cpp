// name: script.cpp

#include "script.hpp"

#include "script_internal.hpp"

#include <px/memory/memory.hpp>

namespace px {
	script::~script()
	{

	}
	script::script()
		: works(make_uq<script_internal>())
	{
	}

	// methods

	void script::assign_environment(environment * game)
	{
		works->assign_environment(game);
	}

	void script::run(std::string const& code)
	{
		works->run(code);
	}

	void script::execute(std::string const& name)
	{
		works->execute(name);
	}

	rl::skill_functional<body_component *, body_component *, point2> script::impact(std::string const& name)
	{
		return works->impact(name);
	}
}