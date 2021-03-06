// name: tile_library.hpp

#pragma once

#include "tile.hpp"
#include "tile_prototype.hpp"

#include "../app/settings.hpp"
#include "../app/document.hpp"

#include "../es/sprite_system.hpp"
#include "../es/sprite_component.hpp"

#include <map>

namespace px {

    class tile_library final {
    public:
        void add_prototype(std::uint32_t block_id, std::string const& name, rl::mass<rl::traverse> const& mass) {
            library[block_id] = { block_id, name, mass };
        }

        void assign_sprites(sprite_system * system) noexcept {
            sprites = system;
        }

        void setup(tile & block, std::uint32_t block_id) {
            tile_prototype const& prototype = library[block_id];

            block.block_id = prototype.block_id;
            block.mass = prototype.mass;
            block.sprite = sprites->make(prototype.name);
            if (block.sprite) {
                block.sprite->connect(&block.transform);
                block.sprite->activate();
            }
        }

    public:
        tile_library()
            : sprites(nullptr) {
            load_prototypes();
        }

    private:
        void load_prototypes() {
            auto doc = document::load_document(settings::tiles_path);
            auto tile_list = doc["tiles"];
            for (auto const& tile_node : tile_list) {
                unsigned int const block_id = tile_node["block_id"];
                std::string const name = tile_node["name"];
                bool const transparent = tile_node["transparent"];
                unsigned int const traverse_mask = tile_node["traversable"];

                rl::mass<rl::traverse>::bitset_type const blocking_bits{ traverse_mask };

                add_prototype(block_id, name, { transparent, ~blocking_bits });
            }
        }

    private:
        std::map<uint32_t, tile_prototype>  library;
        sprite_system *                     sprites;
    };
}