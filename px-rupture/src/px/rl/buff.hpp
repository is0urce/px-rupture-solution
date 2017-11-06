// name: item.hpp

#pragma once

#include <px/rl/entity.hpp>
#include <px/rl/enhancement_collection.hpp>
#include <px/rl/effect.hpp>
#include <px/rl/stack.hpp>

#include <cstdint>

namespace px::rl {

	template <typename Effect, typename Integral, typename Real>
	class buff
		: public entity
		, public rl::enhancement_collection<Effect, Integral, Real>
	{
	public:
		bool is_expired() const noexcept {
			return duration <= 0;
		}
		void set_duration(Integral remaining) noexcept
		{
			duration = remaining;
		}
		void reduce_duration(Integral timespan) noexcept
		{
			duration -= timespan;
		}
		Integral timer() const noexcept
		{
			return duration;
		}

		template <typename Archive>
		void serialize(Archive & archive) {
			archive(static_cast<rl::entity &>(*this));
			archive(static_cast<rl::enhancement_collection<Effect, Integral, Real> &>(*this));
			archive(duration);
		}

	public:
		buff()
		{
		}

	private:
		Integral duration;
	};
}