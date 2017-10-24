#pragma once

#ifndef NDEBUG

#include <string>
#include <iostream>

namespace px {

	void debug(std::string const& text)
	{
		std::cout << text << std::endl;
	}
}

#else

namespace px {

	void debug(std::string const& /* discarded */)
	{
	}
}

#endif