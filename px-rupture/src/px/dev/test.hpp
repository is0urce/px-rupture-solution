// name: test.hpp

#pragma once

#include <iostream>
#include <string>

namespace px {

	class test final {
	public:
		static void require(bool result) {
			++total;
			if (!result) {
				write_fail();
			}
		}

		template <typename Operation>
		static void require_throw(Operation && fn) {
			try {
				fn();
				write_fail();
			}
			catch (...) {
				++total;
			}
		}

		static void section(const char* section_name) {
			lastmsg = section_name;
		}

		static void print() {
			unsigned int success = total - fails;
			if (fails == 0) {
				std::cout << "ALL " << std::to_string(success) << " TESTS PASS ";
			}
			
			if (fails > 0) {
				std::cout << "TEST FAILS " << std::to_string(fails) << ", " << std::to_string(success) << "/" << std::to_string(total) << " TESTS PASS ";
			}
			std::cout << std::endl;
		}

	private:
		static void write_fail() {
			++fails;
			if (lastmsg != nullptr) {
				std::cout << "TEST FAIL, section " << std::string(lastmsg) << std::endl;
			}
		}

	public:
		static unsigned int total;
		static unsigned int fails;
		static const char * lastmsg;
	};
}