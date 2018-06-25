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
            const static char *const application_name;
            const static char *const bindings_path;
            const static char *const configuration_path;
            const static char *const texture_path;
            const static char *const tiles_path;
            const static char *const schemata_path;
            const static char *const blueprints_path;
            const static char *const scripts_path;
            const static char *const skills_path;
            const static char *const log_path;
            const static char *const save_path;
            const static char *const terrrain_path;
        };

        const char *const settings::application_name = "px-rupture";
        const char *const settings::bindings_path = "data/controls.json";
        const char *const settings::configuration_path = "data/config.json";
        const char *const settings::texture_path = "data/textures.json";
        const char *const settings::tiles_path = "data/tiles.json";
        const char *const settings::skills_path = "data/skills.json";
        const char *const settings::log_path = "log.txt";
        const char *const settings::schemata_path = "data/schemata/";
        const char *const settings::blueprints_path = "data/blueprints/";
        const char *const settings::scripts_path = "data/scripts/";
        const char *const settings::save_path = "data/save/";
        const char *const settings::terrrain_path = "data/terrain/";
    }
}