#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

namespace px {

	class body_component;
	class body_works;

	class body_system final
		: public system<delta>
	{
	public:
		uq_ptr<body_component> make();

	public:
		virtual ~body_system();
		body_system();

	private:
		uq_ptr<body_works> works;
	};
}