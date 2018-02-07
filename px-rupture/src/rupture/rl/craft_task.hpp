// name: craft_task

#pragma once

#include "item.hpp"

#include <px/memory/uq_ptr.hpp>

#include <algorithm>
#include <vector>

namespace px::rl {

	class craft_task {
	public:
		using item_type = rl::item;
		using ptr = uq_ptr<item>;

	public:
		void close() {
			slots.clear();
		}
		void reset(size_t n) {
			slots.resize(n);
		}
		size_t size() const noexcept {
			return slots.size();
		}

		// sort empty slots
		void sort() {
			std::sort(slots.begin(), slots.end(), [](auto const& a, auto const& b) { return !b.get() && a.get(); });
		}

		bool add(ptr reagent) {
			for (size_t idx = 0, size = slots.size(); idx != size; ++idx) {
				if (!slots[idx]) {
					slots[idx] = std::move(reagent);
					sort();
					return true;
				}
			}
			return false;
		}
		ptr remove(size_t idx) {
			ptr result;
			if (idx < slots.size()) {
				result = std::move(slots[idx]);
				slots[idx] = std::move(slots.back());
				sort();
			}
			return result;
		}
		template <size_t Index>
		ptr remove() {
			ptr result;
			if (Index < slots.size()) {
				result = std::move(slots[Index]);
				slots[Index] = std::move(slots.back());
				sort();
			}
			return result;
		}
		ptr remove() {
			return remove<0>();
		}
		bool is_empty() const noexcept {
			for (size_t idx = 0, size = slots.size(); idx != size; ++idx) {
				if (slots[idx]) return false;
			}
			return true;
		}
		bool is_complete() const noexcept {
			for (size_t idx = 0, size = slots.size(); idx != size; ++idx) {
				if (!slots[idx]) return false;
			}
			return true;
		}
		auto calculate_essence() const {
			auto denominator = static_cast<rl::item::enhancement_type::integer_type>(1);

			for (size_t idx = 0, size = slots.size(); idx != size; ++idx) {
				if (slots[idx]) {
					auto enhancement = slots[idx]->find(rl::item::enhancement_type::zero(rl::effect::essence));
					auto essence = enhancement.value0;
					if (essence != 0 && denominator % essence != 0) {
						denominator *= essence;
					}
				}
			}
			return denominator;
		}
		auto calculate_power() const {
			auto power = rl::item::enhancement_type::zero(rl::effect::ingredient_power);

			for (size_t idx = 0, size = slots.size(); idx != size; ++idx) {
				if (slots[idx]) {
					power = slots[idx]->accumulate(power);
				}
			}
			return power;
		}

		template <typename Operator>
		void enumerate(Operator && function) const {
			for (auto const& ptr : slots) {
				if (ptr) {
					function(*ptr);
				}
			}
		}

		item_type const* operator[](size_t idx) const {
			return slots[idx].get();
		}

	private:
		std::vector<ptr> slots;
	};
}