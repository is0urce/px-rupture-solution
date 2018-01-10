// name: light.hpp
// type: c++ header
// desc: class
// auth: is0urce

#pragma once

#include <px/common/color.hpp>

#include <px/es/light_source.hpp>

namespace px {

	class light {
	public:
		color			tint;
		double			elevation;
		bool			is_on;
		light_source	source;

	public:
		template <typename Archive>
		void serialize(Archive & archive) {
			archive(tint, elevation, is_on, source);
		}

	public:
		light() noexcept
			: tint(0, 0, 0, 0)
			, elevation(0)
			, is_on(true)
			, source(light_source::point)
		{
		}
	};
}