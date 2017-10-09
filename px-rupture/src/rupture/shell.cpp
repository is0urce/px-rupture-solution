// name: shell.cpp

#include "shell.hpp"

#include "app/document.hpp"
#include "app/settings.hpp"

#include <px/memory/memory.hpp>

#include "draw/renderer.hpp"

#include <lodepng.h>

#include <string>

namespace px {

	shell::~shell()
	{
	}
	shell::shell(unsigned int start_widht, unsigned int start_height)
		: render(make_uq<renderer>(start_widht, start_height))
	{
		connect_managers();
		load_data();
		register_systems();
		start();
	}

	void shell::resize(int width, int height)
	{
		render->resize(width, height);
	}

	void shell::connect_managers()
	{
		render->assign_sprite_data(sprites.data());
		terrain.assign_sprites(&sprites);
	}
	void shell::load_data()
	{
		auto document = document::load_document(settings::texture_path);
		auto textures = document["textures"];
		unsigned int texture_id = 0;
		for (auto const& texture : textures) {
			std::string atlas = texture["atlas"];
			std::string bitmap = texture["texture"];

			add_texture(bitmap.c_str());
			add_atlas(atlas.c_str(), texture_id);
			++texture_id;
		}
	}

	void shell::register_systems()
	{
		engine.add(&sprites);
		engine.add(&transforms);
	}

	void shell::add_texture(const char * name)
	{
		std::vector<unsigned char> bits;
		unsigned int texture_width;
		unsigned int texture_height;
		auto error = lodepng::decode(bits, texture_width, texture_height, name);
		if (error) throw std::runtime_error("add_texture error while loading image, path=" + std::string(name));
		render->add_texture(texture_width, texture_height, bits.data());
	}

	void shell::add_atlas(const char * name, unsigned int texture_index)
	{
		auto document = document::load_document(name);
		sprites.load_atlas(document, texture_index, true);
	}

	void shell::frame(double timer)
	{
		if (run) {
			time.advance(timer);
			engine.update(time);

			++time.tick_index;
			engine.tick_update(time);
			render->run();
		}
	}
	void shell::text(unsigned int /*codepoint*/)
	{

	}
	void shell::click(int /*mouse_button*/)
	{

	}
	void shell::hover(int /*x*/, int /*y*/)
	{

	}
	void shell::scroll(double horisontal, double vertical)
	{
		double total = horisontal + vertical;
		render->zoom(total > 0);
	}
	void shell::press(key action_index)
	{
		switch (action_index) {
		case key::move_east: step({ 1, 0 }); break;
		case key::move_west: step({ -1, 0 }); break;
		case key::move_north: step({ 0, 1 }); break;
		case key::move_south: step({ 0, -1 }); break;
		}
	}
}