// name: animator_component.cpp

#include "animator_component.hpp"

namespace px {

	animator_component::~animator_component()
	{
	}
	animator_component::animator_component() noexcept
		: name(nullptr)
		, playing(false)
		, current_index(0)
	{
	}

	bool animator_component::play(size_t animation_id)
	{
		if (animation_id <= animations.size()) {
			current_index = animation_id;
			playing = true;
			return true;
		}
		return false;
	}
	void animator_component::stop()
	{
		playing = false;
	}
	void animator_component::resume()
	{
		playing = true;
	}
	bool animator_component::is_playing() const noexcept
	{
		return playing;
	}

	char const * const animator_component::get_id() const noexcept
	{
		return name;
	}

	void animator_component::set_id(char const * const id) noexcept
	{
		name = id;
	}

	void animator_component::clear()
	{
		animations.clear();
		stop();
	}
	void animator_component::push_animation(animation const* ptr)
	{
		animations.push_back(ptr);
	}

	animation const* animator_component::current() const
	{
		return animations[current_index];
	}

	size_t animator_component::size() const
	{
		return animations.size();
	}
}