// name: message_overlay.hpp

#pragma once

#include "message.hpp"
#include "popup.hpp"

#include <px/common/point.hpp>

#include <vector>
#include <map>

namespace px {

	class message_overlay
	{
	public:
		void add_message(message && msg, point2 const& location)
		{
			auto & grid = map[location];
			for (auto & prev : grid) {
				prev.lift += msg.magnitude;
			}
			grid.emplace_back(popup{ std::move(msg), location, 0.0 });
			++total;
		}
		void cls()
		{
			map.clear();
			total = 0;
		}
		void flatten()
		{
			flat.resize(total);
			unsigned int i = 0;
			for (auto const& kv : map) {
				for (auto const& msg : kv.second) {
					flat[i] = msg;
					++i;
				}
			}
		}
		std::vector<popup> const* data() const
		{
			return &flat;
		}

	public:
		message_overlay()
			: total(0)
		{
		}

	private:
		std::map<point2, std::vector<popup>, decltype(&point2::lex_less)> map;
		std::vector<popup> flat;
		unsigned int total;
	};
}