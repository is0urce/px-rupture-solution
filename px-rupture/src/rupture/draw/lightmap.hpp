// name: lightmap.hpp
// type: c++ struct

#pragma once

namespace px {

	struct lightmap
	{
		int ox;
		int oy;
		size_t width;
		size_t height;
		double * data;
		unsigned int version;
	};
}