// name: poly_release_block.hpp
// type: c++ header

#pragma once

// abstract base class for management of memory release for erased deleters
// polymorphic function used in destructor, so it has noexcept specifier

namespace px {

	class abstract_release_block abstract
	{
	public:
		void release() noexcept
		{
			release_block();
		}

	public:
		virtual ~abstract_release_block()
		{
		}
		abstract_release_block() noexcept
		{
		}

	protected:
		virtual void release_block() noexcept
		{
		}
	};
}