// name: lightmap_control.hpp
// type: c++ struct

#pragma once

#include "lightmap_data.hpp"

namespace px {

	class lightmap_control
	{
	public:
		bool is_dirty() const noexcept
		{
			return current && current->version != prev_version;
		}
		void cashed()
		{
			prev_version = current ? current->version : ((unsigned int)-1);
		}
		size_t width()
		{
			return current ? current->width : 0;
		}
		size_t height()
		{
			return current ? current->height : 0;
		}
	public:
		lightmap_data const* current;
		lightmap_data const* last;
		unsigned int prev_version;

	public:
		lightmap_control()
			: current(nullptr)
			, last(0)
			, prev_version((unsigned int)-1)
		{
		}
	};
}