#include "builder.hpp"

#include "rupture/core.hpp"

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

	sprite_component * builder::add_sprite(const char* name)
	{
		auto s = factory->sprites.make(name);
		sprite = s.get();
		unit->add(std::move(s));
		return sprite;
	}
	transform_component * builder::add_transform(point2 const& location)
	{
		auto t = factory->transforms.make();
		t->place(location);
		t->store();
		transform = t.get();
		unit->add(std::move(t));
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
		unit = make_uq<composite_component>();
	}
}