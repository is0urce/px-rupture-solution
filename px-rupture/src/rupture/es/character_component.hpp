#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

namespace px {

	class character_component
		: public component
		, public link_dispatcher<character_component>
	{
	public:
		virtual ~character_component() = default;
	};
	class container_component
		: public component
		, public link_dispatcher<container_component>
	{
	public:
		virtual ~container_component() = default;
	};

	class door_component
		: public component
		, public link_dispatcher<door_component>
	{
	public:
		virtual ~door_component() = default;
	};
	class storage_component
		: public component
		, public link_dispatcher<storage_component>
	{
	public:
		virtual ~storage_component() = default;
	};
	class deposite_component
		: public component
		, public link_dispatcher<deposite_component>
	{
	public:
		virtual ~deposite_component() = default;
	};
	class workshop_component
		: public component
		, public link_dispatcher<workshop_component>
	{
	public:
		virtual ~workshop_component() = default;
	};

	class player_component
		: public component
		, public link_dispatcher<player_component>
	{
	public:
		virtual ~player_component() = default;
	};
	class npc_component
		: public component
		, public link_dispatcher<npc_component>
	{
	public:
		virtual ~npc_component() = default;
	};
}