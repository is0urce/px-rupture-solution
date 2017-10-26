// name: builder.hpp
// type: c++ header
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>

#include <string>

namespace px {

	class animator_component;
	class body_component;
	class core;
	class character_component;
	class composite_component;
	class container_component;
	class deposite_component;
	class door_component;
	class light_component;
	class npc_component;
	class player_component;
	class sprite_component;
	class storage_component;
	class transform_component;
	class workshop_component;

	class builder final
	{
	public:
		animator_component *		add_animator(std::string const& name);
		body_component *			add_body();
		character_component *		add_character();
		container_component *		add_container();
		deposite_component *		add_deposite();
		door_component *			add_door();
		light_component *			add_light();
		npc_component *				add_npc();
		player_component *			add_player();
		sprite_component *			add_sprite(std::string const& name);
		storage_component *			add_storage();
		transform_component *		add_transform(point2 const& location);
		workshop_component *		add_workshop();
		void						begin();
		void						begin(uq_ptr<composite_component> strip);
		void						remove_body();
		void						remove_character();
		void						remove_container();
		void						remove_deposite();
		void						remove_light();
		void						remove_npc();
		void						remove_player();
		void						remove_sprite();
		void						remove_transform();
		uq_ptr<composite_component>	request();

	public:
		~builder();
		builder(core * production_factory);

	private:
		void						link_components();

	private:
		core *						factory;
		uq_ptr<composite_component>	unit;
		animator_component *		animator;
		body_component *			body;
		character_component *		character;
		container_component *		container;
		deposite_component *		deposite;
		door_component *			door;
		light_component *			light;
		npc_component *				npc;
		player_component *			player;
		sprite_component *			sprite;
		storage_component *			storage;
		transform_component *		transform;
		workshop_component *		workshop;
	};
}