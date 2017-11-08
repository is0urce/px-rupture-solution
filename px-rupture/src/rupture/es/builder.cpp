// name: builder.hpp
// type: c++
// auth: is0urce
// desc: class methods implementation

#include "builder.hpp"

#include "rupture/core.hpp"

#include "animator_component.hpp"
#include "body_component.hpp"
#include "composite_component.hpp"
#include "character_component.hpp"
#include "container_component.hpp"
#include "deposit_component.hpp"
#include "door_component.hpp"
#include "light_component.hpp"
#include "npc_component.hpp"
#include "player_component.hpp"
#include "sprite_component.hpp"
#include "storage_component.hpp"
#include "transform_component.hpp"
#include "workshop_component.hpp"

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

	// methods

	sprite_component * builder::add_sprite(std::string const& name)
	{
		auto part = factory->sprites.make(name);
		sprite = part.get();
		if (sprite) {
			unit->add(std::move(part));
		}

		return sprite;
	}
	animator_component * builder::add_animator(std::string const& name)
	{
		auto part = factory->animators.make(name);
		animator = part.get();
		if (animator) {
			unit->add(std::move(part));
		}

		return animator;
	}
	transform_component * builder::add_transform(point2 const& location)
	{
		auto part = factory->transforms.make();
		transform = part.get();
		part->place(location);
		part->store();
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
	npc_component * builder::add_npc()
	{
		auto part = factory->npcs.make();
		npc = part.get();
		unit->add(std::move(part));

		return npc;
	}
	character_component * builder::add_character()
	{
		auto part = factory->characters.make();
		character = part.get();
		unit->add(std::move(part));

		return character;
	}
	container_component * builder::add_container()
	{
		auto part = make_uq<container_component>();
		container = part.get();
		unit->add(std::move(part));

		return container;
	}
	player_component * builder::add_player()
	{
		auto part = make_uq<player_component>();
		player = part.get();
		unit->add(std::move(part));

		return player;
	}
	door_component * builder::add_door()
	{
		auto part = make_uq<door_component>();
		door = part.get();
		unit->add(std::move(part));

		return door;
	}
	deposite_component * builder::add_deposite()
	{
		auto part = make_uq<deposite_component>();
		deposite = part.get();
		unit->add(std::move(part));

		return deposite;
	}
	storage_component * builder::add_storage()
	{
		auto part = make_uq<storage_component>();
		storage = part.get();
		unit->add(std::move(part));

		return storage;
	}
	workshop_component * builder::add_workshop()
	{
		auto part = make_uq<workshop_component>();
		workshop = part.get();
		unit->add(std::move(part));

		return workshop;
	}

	void builder::remove_animator()
	{
		unit->remove<animator_component>();
		animator = nullptr;
	}
	void builder::remove_transform()
	{
		unit->remove<transform_component>();
		transform = nullptr;
	}
	void builder::remove_sprite()
	{
		unit->remove<sprite_component>();
		sprite = nullptr;
	}

	void builder::remove_body()
	{
		unit->remove<body_component>();
		body = nullptr;
	}
	void builder::remove_character()
	{
		unit->remove<character_component>();
		character = nullptr;
	}
	void builder::remove_container()
	{
		unit->remove<container_component>();
		container = nullptr;
	}
	void builder::remove_deposite()
	{
		unit->remove<deposite_component>();
		deposite = nullptr;
	}
	void builder::remove_door() {
		unit->remove<door_component>();
		door = nullptr;
	}
	void builder::remove_light()
	{
		unit->remove<light_component>();
		light = nullptr;
	}
	void builder::remove_npc()
	{
		unit->remove<npc_component>();
		npc = nullptr;
	}
	void builder::remove_player() {
		unit->remove<player_component>();
		player = nullptr;
	}
	void builder::remove_workshop() {
		unit->remove<workshop_component>();
		workshop = nullptr;
	}

	void builder::link_components() {

		// presentation

		if (transform) {
			transform->connect(body);
		}
		if (sprite) {
			sprite->connect(transform);
		}
		if (animator) {
			animator->connect(sprite);
		}
		if (light) {
			light->connect(transform);
		}

		// rpg

		if (body) {
			body->connect(transform);
			body->connect(character);
			body->connect(container);
			body->connect(unit.get());

			if (deposite) body->assign_useable(deposite);
			if (door) body->assign_useable(door);
			if (storage) body->assign_useable(storage);
			if (workshop) body->assign_useable(workshop);
		}

		// useables

		if (deposite) {
			deposite->connect(container);
			deposite->connect(unit.get());
		}
		if (door) {
			door->connect(animator);
			door->connect(body);
		}

		// controls

		if (npc) {
			npc->connect(transform);
		}

		unit->connect(transform);
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
		animator = nullptr;
		body = nullptr;
		character = nullptr;
		container = nullptr;
		deposite = nullptr;
		door = nullptr;
		light = nullptr;
		npc = nullptr;
		player = nullptr;
		sprite = nullptr;
		storage = nullptr;
		transform = nullptr;
		workshop = nullptr;

		unit = make_uq<composite_component>();
	}
	void builder::begin(uq_ptr<composite_component> strip)
	{
		unit = std::move(strip);

		if (unit) {
			animator = unit->query<animator_component>();
			body = unit->query<body_component>();
			character = unit->query<character_component>();
			container = unit->query<container_component>();
			deposite = unit->query<deposite_component>();
			door = unit->query<door_component>();
			light = unit->query<light_component>();
			npc = unit->query<npc_component>();
			player = unit->query<player_component>();
			sprite = unit->query<sprite_component>();
			storage = unit->query<storage_component>();
			transform = unit->query<transform_component>();
			workshop = unit->query<workshop_component>();
		}
		else {
			begin();
		}
	}
}