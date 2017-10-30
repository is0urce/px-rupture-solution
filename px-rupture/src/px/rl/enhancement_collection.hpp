#pragma once

#include <px/rl/enhancement.hpp>

#include <vector>

namespace px::rl {

	template <typename Effect, typename Integral = int, typename Real = double>
	class enhancement_collection
		: public std::vector<enhancement<Effect, Integral, Real>>
	{
	public:
		typedef enhancement<Effect, Integral, Real> enhancement_type;

	public:
		enhancement_type accumulate(enhancement_type accumulator) const {
			for (enhancement_type const& element : *this) {
				if (element.main == accumulator.main) {
					accumulator += element;
					accumulator.sub = element.sub;
				}
			}
			return accumulator;
		}
		bool has_effect(Effect efx) const {
			for (enhancement_type const& element : *this) {
				if (element.main == efx) {
					return true;
				}
			}
			return false;
		}
		int find_subtype(Effect efx, int or_else) const {
			for (enhancement_type const& element : *this) {
				if (element.main == efx) {
					return element.sub;
				}
			}
			return or_else;
		}

		template <typename Archive>
		void serialize(Archive & archive) {
			archive(static_cast<std::vector<enhancement<Effect, Integral, Real>> &>(*this));
		}
	};
}