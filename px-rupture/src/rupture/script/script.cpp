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
		works->run(R"(print("lua print..."))");
	}

	// methods

	void script::run(std::string const& code)
	{
		works->run(code);
	}

	void script::execute(std::string const& name)
	{
		works->execute(name);
	}

	skill & script::impact(std::string const& name)
	{
		return works->impact(name);
	}
}