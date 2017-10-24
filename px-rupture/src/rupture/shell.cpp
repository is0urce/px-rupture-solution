// name: shell.cpp

#include "shell.hpp"

#include "app/document.hpp"
#include "app/settings.hpp"

#include <px/common/vector.hpp>

#include <lodepng.h>

#include <string>

namespace px {

	shell::~shell()
	{
	}
	shell::shell(unsigned int start_width, unsigned int start_height)
		: renderer(start_width, start_height)
		, ui(start_width, start_height, this)
		, width(start_width)
		, height(start_height)
	{
		connect_managers();
		load_data();
		register_systems();
		start();
	}

	// methods

	void shell::resize(unsigned int screen_width, unsigned int screen_height)
	{
		width = screen_width;
		height = screen_height;
		renderer.resize(width, height);
		ui.resize(width, height);
	}

	void shell::connect_managers()
	{
		renderer.assign_sprite_data(sprites.data());
		renderer.assign_lightmap_data(lights.current_data(), lights.last_data());
		renderer.assigm_message_data(messages.data());
		lights.assign_scene(&stage);
		stage.assign_sprites(&sprites);
	}
	void shell::load_data()
	{
		auto document = document::load_document(settings::texture_path);
		auto textures = document["textures"];
		unsigned int texture_id = 0;
		for (auto const& texture : textures) {
			std::string atlas = texture["atlas"];	// conversion constructors
			std::string bitmap = texture["texture"];

			add_texture(bitmap.c_str());
			add_atlas(atlas.c_str(), texture_id);
			++texture_id;
		}

		characters.load(&mashine);
		animators.load(&sprites);
	}

	void shell::register_systems()
	{
		engine.add(&animators);
		engine.add(&sprites);
		engine.add(&renderer);
		engine.add(&ui);

		// order is important
		engine.add(&transforms);
		engine.add(&characters);
		engine.add(&npcs);
		engine.add(&lights);
		engine.add(&messages);
	}

	void shell::frame(double timer)
	{
		if (is_running()) {
			time.advance(timer);
			if (turn_passed()) {
				time.advance_turn(current_turn());
				engine.turn_update(time);
				pass_turn();
			}
			engine.update(time);
		}
	}
	void shell::text(unsigned int codepoint)
	{
		if (!is_running() || turn_passed()) return;
		ui.text(codepoint);
	}
	void shell::click(int mouse_button, bool is_down)
	{
		if (!is_running() || turn_passed()) return;
		if (ui.click(mouse_button, is_down)) return;
		if (mouse_button == 0 && is_down) {
			advance();
		}
	}
	void shell::hover(int x, int y)
	{
		if (!is_running() || turn_passed()) return;
		if (ui.hover(x, y)) return;
		
		vector2 position(x, y);
		position /= vector2(width, height);			// (0, 1) screen space
		position += { -0.5, -0.5 };					// to screen center
		position *= { 2.0, -2.0 * height / width };	// account aspect
		position /= renderer.get_scale();
		position += { 0.5, 0.5 };					// tile center offset
		focus(position.floor());
	}
	void shell::scroll(double horisontal, double vertical)
	{
		if (!is_running() || turn_passed()) return;
		if (ui.scroll(horisontal, vertical)) return;
		renderer.zoom(horisontal + vertical > 0);
	}
	void shell::press(key action_index)
	{
		if (!is_running() || turn_passed()) return;
		if (ui.takes_input()) return;

		switch (action_index) {
		case key::move_east: step({ 1, 0 }); break;
		case key::move_west: step({ -1, 0 }); break;
		case key::move_north: step({ 0, 1 }); break;
		case key::move_south: step({ 0, -1 }); break;

		case key::action0: action(0); break;
		case key::action1: action(1); break;
		case key::action2: action(2); break;
		case key::action3: action(3); break;
		case key::action4: action(4); break;
		case key::action5: action(5); break;
		}
	}

	void shell::add_texture(const char * name)
	{
		std::vector<unsigned char> bits;
		unsigned int texture_width;
		unsigned int texture_height;
		auto error = lodepng::decode(bits, texture_width, texture_height, name);
		if (error) throw std::runtime_error("add_texture error while loading image, path=" + std::string(name));
		renderer.add_texture(texture_width, texture_height, bits.data());
	}

	void shell::add_atlas(const char * name, unsigned int texture_index)
	{
		auto document = document::load_document(name);
		sprites.load_atlas(document, texture_index, true);
	}
}