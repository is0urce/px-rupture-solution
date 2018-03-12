// name: environment-io.cpp
// type: c++
// auth: is0urce
// desc: class methods implementation

#include "environment.hpp"

#include "app/settings.hpp"

#include "io/blueprint.hpp"
#include "io/repository.hpp"
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

        stage.unload();

        save_main();

        // copy directory
        repository out_directory(std::string(settings::save_path) + name);
        out_directory.clear();
        current->save(out_directory);

        load(name);
        return true;
    }

    bool environment::load(std::string const& name) {

        end();

        // copy directory
        repository in_directory(std::string(settings::save_path) + name);
        if (!in_directory.has_main()) return false;
        current->clear();
        current->load(in_directory);

        load_main();

        return_turn();

        return true;
    }

    void environment::save_main() {
        auto ostream = output_stream(current->depot_main());
        SAVE_OUTPUT_ARCHIVE archive(ostream);

        // save units
        size_t count = stage.size();
        archive(count);
        stage.enumerate([&](composite_component & unit) {
            blueprint::store(archive, unit);
        });
    }

    void environment::load_main() {
        auto istream = input_stream(current->depot_main());
        SAVE_INPUT_ARCHIVE archive(istream);

        builder factory(this);

        stage.clear_units();

        size_t count;
        archive(count);
        for (size_t i = 0; i != count; ++i) {
            auto & unit = stage.spawn(blueprint::assemble(archive, factory));
            unit->activate();
        }

        incarnate(factory.created_player());
    }

    void environment::save_scene(point2 const& cell) {

        auto ostream = output_stream(current->depot_scene(cell));
        SAVE_OUTPUT_ARCHIVE archive(ostream);

        std::vector<uq_ptr<composite_component>> tmp;
        stage.pull(cell, [&](uq_ptr<composite_component> unit) {
            tmp.push_back(std::move(unit));
        });

        // save units
        size_t count = tmp.size();
        archive(count);
        for (auto & ptr : tmp) {
            blueprint::store(archive, *ptr);
        }
    }

    void environment::load_scene(point2 const& cell) {
        auto name = current->depot_scene(cell);
        if (!current->has_scene(cell)) return;

        auto istream = input_stream(name);
        SAVE_INPUT_ARCHIVE archive(istream);

        builder factory(this);

        size_t count;
        archive(count);
        for (size_t i = 0; i != count; ++i) {
            auto & unit = stage.spawn(blueprint::assemble(archive, factory));
            unit->activate();
        }
    }

    void environment::update_blueprints(std::string const& /*out_path*/, std::string const& /*blueprint*/) {
    }
}