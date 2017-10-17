#include "body_system.hpp"

#include "body_works.hpp"

#include <px/memory/memory.hpp>

namespace px {

	// ctor & dtor

	body_system::~body_system()
	{

	}
	body_system::body_system()
		: works(make_uq<body_works>())
	{

	}

	// methods

	uq_ptr<body_component> body_system::make()
	{
		return works->make();
	}
}