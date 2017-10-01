// name: transform_system.hpp

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

namespace px {
	
	class transform_component;
	class transform_works;

	class transform_system final
		: public system<delta>
	{
	public:
		uq_ptr<transform_component> make();

	public:
		virtual ~transform_system();
		transform_system();

	private:
		uq_ptr<transform_works> works;
	};
}