// name: builder.hpp

#pragma once

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>

#include <string>

namespace px {

	class core;
	class composite_component;
	class sprite_component;
	class transform_component;
	class light_component;
	class body_component;
	class animator_component;

	class builder final
	{
	public:
		void						begin();
		uq_ptr<composite_component>	request();
		sprite_component *			add_sprite(std::string const& name);
		transform_component *		add_transform(point2 const& location);
		light_component *			add_light();
		body_component *			add_body();
		animator_component *		add_animator(std::string const& name);

	public:
		~builder();
		builder(core * production_factory);

	private:
		void link_components();

	private:
		core *						factory;
		uq_ptr<composite_component>	unit;
		transform_component *		transform;
		sprite_component *			sprite;
		light_component *			light;
		body_component *			body;
		animator_component *		animator;
	};
}