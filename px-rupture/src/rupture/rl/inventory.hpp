// name: inventory.hpp

#pragma once

#include "item.hpp"

#include <px/memory/uq_ptr.hpp>
#include <px/memory/memory.hpp>

#include <vector>

namespace px::rl {

	class inventory
	{
	public:
		typedef uq_ptr<item> item_ptr;

	public:
		item_ptr & add(item_ptr item) {
			items.push_back(std::move(item));
			return items.back();
		}

		item * get(size_t idx) {
			return idx >= items.size() ? nullptr : items[idx].get();
		}

		item_ptr remove(size_t idx) {
			if (idx >= items.size()) return nullptr;
			item_ptr result = std::move(items[idx]);
			items[idx] = std::move(items.back());
			items.pop_back();
			return result;
		}

		void take(inventory & loot) {
			for (item_ptr & i : loot.items) {
				items.push_back(std::move(i));
			}
			loot.items.clear();
		}

		template <typename Op>
		void take(inventory & loot, Op && msg) {
			for (item_ptr & i : loot.items) {
				msg(*i);
				items.push_back(std::move(i));
			}
			loot.items.clear();
		}

		template <typename Op>
		void sort(Op && comparator) {
			std::sort(items.begin(), items.end(), std::forward<Op>(comparator));
		}

		size_t size() {
			return items.size();
		}

		template <typename Op>
		void enumerate(Op && fn) {
			for (item_ptr & ptr : items) {
				fn(*ptr);
			}
		}

		template <typename Op>
		void enumerate(Op && fn) const {
			for (item_ptr const& prt : items) {
				fn(*ptr);
			}
		}

		template <typename Archive>
		void serialize(Archive & archive) {
			size_t size = items.size();
			archive(size);
			items.resize(size);
			for (size_t i = 0; i != size; ++i) {
				if (!items[i]) items[i] = make_uq<rl::item>();
				uq_ptr<item> & r = items[i];
				item & it = *r;
				it;
				archive(it);
			}
		}

	public:
		inventory()	{
		}

	private:
		std::vector<uq_ptr<item>> items;
	};
}