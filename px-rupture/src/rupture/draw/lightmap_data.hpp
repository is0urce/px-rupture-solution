// name: lightmap_data.hpp
// type: c++ struct

#pragma once

namespace px {

	class lightmap_data
	{
	public:
		int				ox;
		int				oy;
		size_t			width;
		size_t			height;
		float *			raw;
		unsigned int	version;
		
	public:
		lightmap_data()
			: ox(0)
			, oy(0)
			, width(0)
			, height(0)
			, raw(nullptr)
			, version(0)
		{
		}
	};
}