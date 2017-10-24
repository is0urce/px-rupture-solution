// name: environment-io.cpp
// type: c++
// auth: is0urce
// desc: class methods implementation

#include "environment.hpp"

#include "io/blueprint.hpp"
#include "io/serialization.hpp"

namespace px {

	uq_ptr<composite_component>	& environment::spawn(std::string const& blueprint_name, point2 const& position)
	{
		auto input = input_stream("data/blueprints/" + blueprint_name + ".dat");
		SAVE_INPUT_ARCHIVE archive(input);

		builder factory(this);
		auto unit = blueprint::load(archive, factory);
		auto transform = unit->query<transform_component>();
		transform->place(position);
		transform->store();
		return spawn(std::move(unit), transform);
	}
}