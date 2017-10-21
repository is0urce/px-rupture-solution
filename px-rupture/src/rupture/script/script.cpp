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
		works->run("print(3)");
	}

	void script::run(std::string const& code)
	{
		works->run(code);
	}

	void script::execute(std::string const& /*name*/)
	{

	}
}