// name: op_release_block
// type: c++ template class

#pragma once

// polymorphic release block for template operator

#include <px/memory/abstract_release_block.hpp>

namespace px {

	template <typename T, typename Operator>
	class op_release_block final
		: public abstract_release_block
	{
	public:
		virtual ~op_release_block()
		{
		}
		op_release_block(T * raw, Operator fn)
			: original(raw)
			, op(fn)
		{
		}
		op_release_block()
			: op_release_block(nullptr, Operator{})
		{
		}

	protected:
		virtual void release_block() noexcept override
		{
			op(original);
		}

	private:
		T * original;
		Operator op;
	};

}