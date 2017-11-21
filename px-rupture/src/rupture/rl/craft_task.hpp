#pragma once

#include "item.hpp"

#include <px/memory/uq_ptr.hpp>

#include <vector>

namespace px {

	class craft_task {
	public:
		void erase() {
			slots.clear();
		}
		void reset(size_t n) {
			erase();
			slots.resize(n);
		}
		uq_ptr<rl::item> remove(size_t idx) {
			uq_ptr<rl::item> result = std::move(slots[idx]);
			slots[idx] = std::move(slots.back());
			slots.pop_back();
			return result;
		}
		bool is_full() {
			for (size_t idx = 0, size = slots.size(); idx != size; ++idx) {
				if (!slots[idx]) return false;
			}
			return true;
		}
		bool add(uq_ptr<rl::item> itm) {
			for (size_t idx = 0, size = slots.size(); idx != size; ++idx) {
				if (!slots[idx]) {
					slots[idx] = std::move(itm);
					return true;
				}
			}
			return false;
		}
		auto calculate_essence() const {
			rl::item::enhancement_type::integer_type denominator = 1;

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

	private:
		std::vector<uq_ptr<rl::item>> slots;
	};
}