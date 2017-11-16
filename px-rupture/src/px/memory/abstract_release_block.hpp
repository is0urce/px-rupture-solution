// name: abstract_release_block.hpp
// type: c++ header
// auth: is0urce
// desc: abstract class for release blocks used for memory management blocks

#pragma once

// abstract base class for management of memory release for erased deleters
// polymorphic function used in destructor, so it has noexcept specifier

namespace px {

	class abstract_release_block abstract {
	public:
		void release() noexcept {
			release_block();
		}

	public:
		virtual ~abstract_release_block() = default;
		abstract_release_block() noexcept = default;

	protected:
		virtual void release_block() noexcept {
		}
	};
}