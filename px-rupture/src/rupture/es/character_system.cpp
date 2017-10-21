#include "character_system.hpp"
#include "character_works.hpp"

#include "rupture/script/script.hpp"

#include <px/memory/memory.hpp>

namespace px {

	character_system::~character_system()
	{
	}
	character_system::character_system()
		: works(make_uq<character_works>())
	{
	}

	uq_ptr<character_component> character_system::make()
	{
		return works->make();
	}
	void character_system::load(script * mashine)
	{
		std::string name = "hit";
		if (mashine) {
			works->add(name, {}, mashine->impact("data/scripts/" + name + ".lua"));
		}
		auto c = works->make();
		c->learn_skill("hit");
		c->at<0>().use(nullptr, nullptr);
		c->at<0>().try_use(nullptr, nullptr);
	}

	void character_system::turn_update_system(delta_type const& /*delta_time*/)
	{
		works->turn();
	}
}