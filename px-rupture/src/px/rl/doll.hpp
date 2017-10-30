#pragma once

#include <px/memory/memory.hpp>

#include <map>
#include <utility>

namespace px::rl {
	template <typename Equipment, typename Item>
	class doll {
	public:
		typedef Item item_type;
		typedef uq_ptr<item_type> item_ptr;

	public:
		item_ptr equip(Equipment slot, item_ptr && item) {
			item_ptr old = std::move(container[slot]);
			container[slot] = std::forward<item_ptr>(item);
			return old;
		}
		item_type * operator[](Equipment slot) const {
			auto it = container.find(slot);
			if (it == container.end()) return nullptr;
			return it->second.get();
		}
		item_ptr remove(Equipment slot) {
			item_ptr result = std::move(container[slot]);
			container.erase(slot);
			return result;
		}
		bool is_equipped(Equipment slot) const {
			return container.find(slot) != container.end();
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