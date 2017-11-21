// name: craft_result.hpp

#pragma once

#include "item.hpp"

#include <px/memory/uq_ptr.hpp>
#include <px/memory/memory.hpp>

#include <string>

namespace px::rl {

	class craft_result {
	public:
		using enhancement_type = rl::item::enhancement_type;
		using integer_type = enhancement_type::integer_type;
		using real_type = enhancement_type::real_type;

	public:
		static uq_ptr<rl::item> create_weapon(std::string tag, std::string name, integer_type essence, real_type raw_power, real_type enh_power) {
			auto result = make_uq<rl::item>();
			result->set_name(name);
			result->set_tag(tag);
			result->add(rl::item::enhancement_type::real(rl::effect::damage, 0, raw_power));
			result->add(rl::item::enhancement_type::real(rl::effect::critical, 0, enh_power));
			result->add(rl::item::enhancement_type::integral(rl::effect::essence, 0, essence));
			return result;
		}
	};
}