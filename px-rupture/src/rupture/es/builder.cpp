#include "builder.hpp"

#include "rupture/core.hpp"

#include "composite_component.hpp"
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
		, sprite(nullptr)
		, transform(nullptr)
		, light(nullptr)
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
		auto l = factory->lights.make();
		light = l.get();
		unit->add(std::move(l));

		return light;
	}

	void builder::link_components()
	{
		if (transform && sprite) sprite->connect(transform);
		if (transform && light) light->connect(transform);
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
		unit = make_uq<composite_component>();
	}
}