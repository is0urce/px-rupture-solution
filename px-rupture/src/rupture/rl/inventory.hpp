#pragma once

#include "item.hpp"

#include <px/memory/uq_ptr.hpp>

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
		bool remove(size_t idx) {
			if (idx >= items.size()) return false;
			items[idx] = std::move(items.back());
			items.pop_back();
			return true;
		}
		void take(inventory & l) {
			for (item_ptr & i : l.items) {
				items.push_back(std::move(i));
			}
			l.items.clear();
		}
		size_t size() {
			return items.size();
		}
		template <typename Op>
		void enumerate(Op && fn)
		{
			for (item_ptr & ptr : items) {
				fn(*ptr);
			}
		}
		template <typename Op>
		void enumerate(Op && fn) const
		{
			for (item_ptr const& prt : items) {
				fn(*ptr);
			}
		}

	public:
		inventory()
		{
		}

	private:
		std::vector<uq_ptr<item>> items;
	};
}