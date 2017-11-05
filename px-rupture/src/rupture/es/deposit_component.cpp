// name: deposit_component.cpp

#include "deposit_component.hpp"

#include "rupture/environment.hpp"
#include "body_component.hpp"
#include "container_component.hpp"
#include "composite_component.hpp"
#include "transform_component.hpp"

namespace px {

	deposite_component::~deposite_component()
	{
	}
	deposite_component::deposite_component()
		: dissolve(true)
		, use_duration(1)
	{
	}

	// virtual

	bool deposite_component::can_use_useable(body_component * user, environment * environment) const {
		if (user && environment) {
			auto dest = user->linked<container_component>();
			auto source = linked<container_component>();
			return source && dest;
		}
		return false;
	}
	void deposite_component::use_useable(body_component * user, environment * environment) {
		if (user && environment) {
			auto dest = user->linked<container_component>();
			auto source = linked<container_component>();
			if (source && dest) {

				// transfer items with popups
				auto pawn = user->linked<transform_component>();
				dest->take(*source, [&](rl::item & item) {
					if (pawn) {
						environment->popup(std::string("+ ") + item.name(), 0xffffff, pawn->position());
					}
				});

				// destroy deposit
				if (dissolve) {
					if (auto composite = linked<composite_component>()) {
						composite->destroy(0);
					}
				}

				if (use_duration != 0) {
					environment->end_turn(use_duration);
				}
			}
		}
	}

	// methods

	void deposite_component::set_dissolve(bool should_destroy) noexcept
	{
		dissolve = should_destroy;
	}
	bool deposite_component::dissolving() const noexcept
	{
		return dissolve;
	}
}