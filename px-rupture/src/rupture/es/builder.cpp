#include "builder.hpp"

#include "rupture/core.hpp"

#include "animator_component.hpp"
#include "composite_component.hpp"
#include "body_component.hpp"
#include "sprite_component.hpp"
#include "transform_component.hpp"
#include "light_component.hpp"

#include <px/memory/memory.hpp>

namespace px {

	builder::~builder()
	{

	}
	builder::builder(core * production_factory)
		: factory(production_factory)
	{
		begin();
	}

	sprite_component * builder::add_sprite(std::string const& name)
	{
		auto part = factory->sprites.make(name);
		sprite = part.get();
		unit->add(std::move(part));
		return sprite;
	}
	animator_component * builder::add_animator(std::string const& name)
	{
		auto part = factory->animators.make(name);
		animator = part.get();
		unit->add(std::move(part));

		return animator;
	}
	transform_component * builder::add_transform(point2 const& location)
	{
		auto part = factory->transforms.make();
		part->place(location);
		part->store();
		transform = part.get();
		unit->add(std::move(part));
		return transform;
	}
	light_component * builder::add_light()
	{
		auto part = factory->lights.make();
		light = part.get();
		unit->add(std::move(part));

		return light;
	}
	body_component * builder::add_body()
	{
		auto part = factory->bodies.make();
		body = part.get();
		unit->add(std::move(part));

		return body;
	}

	void builder::link_components()
	{
		if (transform && sprite) sprite->connect(transform);
		if (transform && light) light->connect(transform);
		if (transform && body) transform->connect(body);
	}

	uq_ptr<composite_component> builder::request()
	{
		link_components();
		auto result = std::move(unit);
		begin();
		return result;
	}
	void builder::begin()
	{
		transform = nullptr;
		sprite = nullptr;
		light = nullptr;
		body = nullptr;
		animator = nullptr;
		unit = make_uq<composite_component>();
	}
}