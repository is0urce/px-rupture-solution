// name: traverse_options.hpp
// type: c++ header
// desc: template class
// auth: is0urce

// class for typed traverse options bitset

#pragma once

#include <bitset>

namespace px::rl
{
	template <typename E, size_t Size = static_cast<size_t>(E::max_value)>
	class traverse_options
	{
	public:
		typedef E enumeration_type;
		typedef std::bitset<Size> bitset_type;

	public:
		static const size_t traverse_layers = Size;

	public:

		// traverable getters

		constexpr bool is_traversable() const { return bits[0]; }
		constexpr bool is_traversable(traverse_options const& opts) const { return (bits & opts.bits).any(); }
		constexpr bool is_traversable(E layer) const { return bits[static_cast<size_t>(layer)]; }

		// traversable setters

		void make_blocking()
		{
			bits.reset();
		}
		void make_blocking(E layer)
		{
			bits.reset(static_cast<size_t>(layer));
		}
		void make_traversable()
		{
			bits.set();
		}
		void make_traversable(bool val)
		{
			if (val) {
				bits.set();
			}
			else {
				bits.reset();
			}
		}
		void make_traversable(E layer)
		{
			bits[static_cast<size_t>(layer)] = true;
		}
		void make_traversable(E layer, bool val)
		{
			bits[static_cast<size_t>(layer)] = val;
		}
		void make_traversable(traverse_options const& opts)
		{
			bits |= opts.bits;
		}
		void make_traversable(bitset_type const& layers)
		{
			bits |= layers;
		}
		void set_traversable(traverse_options const& opts)
		{
			bits = opts.bits;
		}
		void set_traversable(bitset_type const& layers)
		{
			bits = layers;
		}

		// templates

		template <E Layer>
		void make_traversable()
		{
			bits[static_cast<size_t>(Layer)] = true;
		}
		template <E Layer>
		void make_blocking()
		{
			bits[static_cast<size_t>(Layer)] = false;
		}
		template <E Layer>
		constexpr bool is_traversable() const
		{
			return bits[static_cast<size_t>(Layer)];
		}

		// serialization

		template <typename Archive>
		void serialize(Archive & archive)
		{
			archive(bits);
		}

	public:
		constexpr traverse_options() noexcept = default;
		constexpr traverse_options(bitset_type layers) noexcept
			: bits(layers)
		{
		}

	private:
		bitset_type bits;
	};
}