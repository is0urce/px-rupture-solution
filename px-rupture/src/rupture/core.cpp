// name: core.cpp

#include "core.hpp"

#include "app/document.hpp"
#include "app/settings.hpp"

#include <lodepng.h>

namespace px {

	core::~core()
	{
	}
	core::core(unsigned int start_widht, unsigned int start_height)
		: width(start_widht)
		, height(start_height)
		, render(start_widht, start_height)
	{
		connect_managers();
		load_data();
	}

	void core::resize(int screen_width, int screen_height)
	{
		width = screen_width;
		height = screen_height;
		render.resize(width, height);
	}

	void core::connect_managers()
	{
		render.assign_sprite_data(sprites.data());
		terrain.assign_sprites(&sprites);
	}
	void core::load_data()
	{
		add_texture("data/img/monsters.png");
		add_atlas("data/img/monsters.json", 0);
	}

	void core::add_texture(std::string const& name)
	{
		std::vector<unsigned char> bits;
		unsigned int texture_width;
		unsigned int texture_height;
		auto error = lodepng::decode(bits, texture_width, texture_height, name);
		if (error) throw std::runtime_error("add_texture error while loading image, path=" + name);
		render.add_texture(texture_width, texture_height, bits.data());
	}

	void core::add_atlas(std::string const& path, unsigned int texture_index)
	{
		auto document = document::load_document(path);
		sprites.load_atlas(document, texture_index, true);
	}
}