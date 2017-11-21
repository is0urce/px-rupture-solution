// name: craft_task

#pragma once

#include "item.hpp"

#include <px/memory/uq_ptr.hpp>

#include <vector>

namespace px::rl {

	class craft_task {
	public:
		void erase() {
			slots.clear();
		}
		void reset(size_t n) {
			erase();
			slots.resize(n);
		}
		size_t reagent_count() const noexcept {
			return slots.size();
		}
		void sort() {
			std::sort(slots.begin(), slots.end(), [](auto const& a, auto const& b) { return a.get() > b.get(); });
		}
		bool add(uq_ptr<rl::item> itm) {
			for (size_t idx = 0, size = slots.size(); idx != size; ++idx) {
				if (!slots[idx]) {
					slots[idx] = std::move(itm);
					sort();
					return true;
				}
			}
			return false;
		}
		uq_ptr<rl::item> remove(size_t idx) {
			uq_ptr<rl::item> result;
			if (idx < slots.size()) {
				result = std::move(slots[idx]);
				slots[idx] = std::move(slots.back());
				slots.pop_back();
				sort();
			}
			return result;
		}
		uq_ptr<rl::item> remove() {
			return remove(0);
		}
		bool is_empty() const {
			for (size_t idx = 0, size = slots.size(); idx != size; ++idx) {
				if (slots[idx]) return false;
			}
			return true;
		}
		bool is_complete() const {
			for (size_t idx = 0, size = slots.size(); idx != size; ++idx) {
				if (!slots[idx]) return false;
			}
			return true;
		}
		auto calculate_essence() const {
			auto denominator = static_cast<rl::item::enhancement_type::integer_type>(1);

			for (size_t idx = 0, size = slots.size(); idx != size; ++idx) {
				if (slots[idx]) {
					auto essence = slots[idx]->find_subtype(rl::effect::essence, 0);
					if (denominator % essence != 0) {
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
				function(ptr.get());
			}
		}

		rl::item const* operator[](size_t idx) const {
			return slots[idx].get();
		}

	private:
		std::vector<uq_ptr<rl::item>> slots;
	};
}