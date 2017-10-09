// name: lightmap.hpp
// type: c++ struct

#pragma once

namespace px {

	struct lightmap
	{
		int x_offset;
		int y_offset;
		size_t width;
		size_t height;
		double * data;
	};
}