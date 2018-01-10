// name: doll.hpp

#pragma once

#include <px/memory/memory.hpp>

#include <map>
#include <utility>

namespace px::rl {

	template <typename Equipment, typename Item>
	class doll {
	public:
		using item_type = Item;
		using item_ptr = uq_ptr<item_type>;
		using enhancement_type = typename item_type::enhancement_type;

	public:
		// returns swapped item
		item_ptr equip(Equipment slot, item_ptr && item) {
			item_ptr old = std::move(container[slot]);
			container[slot] = std::forward<item_ptr>(item);
			return old;
		}

		// returns nullptr if no equipment
		item_type * operator[](Equipment slot) const {
			auto it = container.find(slot);
			if (it == container.end()) return nullptr;
			return it->second.get();
		}

		// returns removed item (or nullptr if nothing to remove)
		item_ptr remove(Equipment slot) {
			item_ptr result = std::move(container[slot]);
			container.erase(slot);
			return result;
		}

		bool is_equipped(Equipment slot) const {
			return container.find(slot) != container.end();
		}

		enhancement_type accumulate(enhancement_type accumulator) const {
			for (auto const& element : container) {
				accumulator = element.second->accumulate(accumulator);
			}
			return accumulator;
		}

		template <typename Archive>
		void save(Archive & archive) const{
			size_t size = container.size();
			archive(size);
			for (auto const& kv_pair : container) {
				archive(kv_pair.first, *kv_pair.second);
			}
		}
		template <typename Archive>
		void load(Archive & archive) {
			size_t size;
			archive(size);
			for (size_t i = 0; i != size; ++i) {
				Equipment slot;
				item_ptr item = make_uq<item_type>();
				archive(slot, *item);
				container[slot] = std::move(item);
			}
		}

	private:
		std::map<Equipment, item_ptr> container;
	};
}