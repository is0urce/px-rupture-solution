// name: settings.hpp
// auth: is0urce
// type: c++
// desc: constants

#pragma once

// set global setting values

#include <string>

namespace px {
    namespace {

        struct settings {
            static char const* const application_name;
            static char const* const bindings_path;
            static char const* const configuration_path;
            static char const* const texture_path;
            static char const* const tiles_path;
            static char const* const schemata_path;
            static char const* const blueprints_path;
            static char const* const scripts_path;
            static char const* const skills_path;
            static char const* const log_path;
            static char const* const save_path;
            static char const* const terrrain_path;
            static char const* const sound_path;
        };

        char const* const settings::application_name = "Gnomi";
        char const* const settings::bindings_path = "data/controls.json";
        char const* const settings::configuration_path = "data/config.json";
        char const* const settings::texture_path = "data/textures.json";
        char const* const settings::tiles_path = "data/tiles.json";
        char const* const settings::skills_path = "data/skills.json";
        char const* const settings::log_path = "log.txt";
        char const* const settings::schemata_path = "data/schemata/";
        char const* const settings::blueprints_path = "data/blueprints/";
        char const* const settings::scripts_path = "data/scripts/";
        char const* const settings::save_path = "data/save/";
        char const* const settings::terrrain_path = "data/terrain/";
        char const* const settings::sound_path = "data/snd/";
    }
}