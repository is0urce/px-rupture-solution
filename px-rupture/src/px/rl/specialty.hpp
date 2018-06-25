// name: specialty.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

// trait aggregator

#include <algorithm>    // remove
#include <string>       // key
#include <vector>       // container

namespace px {

    class specialty {
    public:
        bool add_trait(std::string const& new_trait) {
            bool insert = true;

            // check if trait already in array
            for (std::string const& old_trait : traits) {
                if (new_trait == old_trait) {
                    insert = false;
                    break;
                }
            }

            if (insert) {
                traits.push_back(new_trait);
            }

            return insert;
        }

        void remove_trait(size_t idx) {
            if (idx < traits.size()) {
                traits[idx] = std::move(traits.back());
                traits.pop_back();
            }
        }

        void remove_trait(std::string const& tag) {
            std::remove(traits.begin(), traits.end(), tag);
        }

        void clear_traits() {
            traits.clear();
        }

        bool has_trait(std::string const& match) const {
            for (std::string const& trait : traits) {
                if (trait == match) return true;
            }
            return false;
        }

        std::string get_trait(size_t idx) const {
            return traits[idx];
        }

        // get number of traits
        size_t count_traits() const noexcept {
            return traits.size();
        }

        template <typename Archive>
        void serialize(Archive & archive) {
            archive(traits);
        }

    private:
        std::vector<std::string> traits;
    };
}