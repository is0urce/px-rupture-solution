// name: results.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "panel.hpp"

#include "rupture/environment.hpp"
#include "rupture/es/transform_component.hpp"
#include "rupture/es/body_component.hpp"

#include <imgui/imgui.h>

namespace px::ui {

	class results final
		: public panel
	{
	public:
		virtual ~results() = default;

		results(environment * game)
			: context(game)
		{
		}

	protected:
		virtual void combine_panel() override {
			if (!context) return;
			transform_component * player = context->possessed();
			if (!player) return;

		}

	private:
		environment *	context;
	};
}