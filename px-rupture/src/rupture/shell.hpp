// name: shell.hpp

#pragma once

#include "environment.hpp"
#include "key.hpp"

#include "es/render_system.hpp"
#include "es/ui_system.hpp"

#include <px/es/engine.hpp>
#include <px/es/delta.hpp>

namespace px {

	class renderer;

	class shell final
		: public environment
	{
	public:
		void frame(double timer);
		void text(unsigned int codepoint);
		void click(int mouse_button);
		void hover(int x, int y);
		void scroll(double horisontal, double vertical);
		void press(key action_index);
		void resize(int w, int h);

	public:
		virtual ~shell();
		shell(unsigned int start_widht, unsigned int start_height);

	private:
		void connect_managers();
		void load_data();
		void register_systems();
		void add_texture(const char * name);
		void add_atlas(const char * name, unsigned int texture_id);

	private:
		delta				time;
		engine<delta>		engine;

		render_system		renderer;
		ui_system			ui;
	};
}