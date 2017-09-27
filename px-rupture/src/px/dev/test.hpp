// name: test.hpp

#pragma once

#include <iostream>
#include <string>

namespace px {

	class test final
	{
	public:
		static void require(bool result)
		{
			++total;
			if (!result) ++fails;
		}
		static void print()
		{
			unsigned int success = total - fails;
			if (fails == 0) {
				std::cout << "ALL " << std::to_string(success) << " TESTS PASS ";
			}
			
			if (fails > 0) {
				std::cout << "TEST FAILS " << std::to_string(fails) << ", " << std::to_string(success) << " TESTS PASS ";
			}
			std::cout << std::endl;
		}

	public:
		static unsigned int total;
		static unsigned int fails;
	};
}