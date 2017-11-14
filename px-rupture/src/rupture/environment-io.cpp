// name: environment-io.cpp
// type: c++
// auth: is0urce
// desc: class methods implementation

#include "environment.hpp"

#include "app/settings.hpp"

#include "io/blueprint.hpp"
#include "io/serialization.hpp"

namespace px {

	uq_ptr<composite_component>	& environment::spawn(std::string const& blueprint_name, point2 const& position) {
		auto input = input_stream("data/blueprints/" + blueprint_name + ".dat");
		SAVE_INPUT_ARCHIVE archive(input);

		builder factory(this);
		auto unit = blueprint::assemble(archive, factory);
		auto transform = unit->query<transform_component>();
		transform->place(position);
		transform->store();
		return spawn(std::move(unit));
	}

	bool environment::save(std::string const& name) {
		save_main();

		// copy directory
		repository save_directory(std::string(settings::save_path) + name);
		directory.save(save_directory);

		return true;
	}

	bool environment::load(std::string const& name) {

		// copy directory
		repository save_directory(std::string(settings::save_path) + name);
		if (!save_directory.has_main()) return false;
		directory.load(save_directory);

		load_main();

		return true;
	}

	void environment::save_main() {
		auto ostream = output_stream(directory.depot_main());
		SAVE_OUTPUT_ARCHIVE archive(ostream);

		// save units
		size_t count = stage.size();
		archive(count);
		stage.enumerate([&](composite_component & unit) {
			blueprint::store(archive, unit);
		});
	}
	void environment::load_main() {
		auto istream = input_stream(directory.depot_main());
		SAVE_INPUT_ARCHIVE archive(istream);

		builder factory(this);

		stage.clear();

		size_t count;
		archive(count);
		for (size_t i = 0; i != count; ++i) {
			auto & unit = stage.spawn(blueprint::assemble(archive, factory));
			unit->activate();
		}

		incarnate(factory.created_player());
	}
}