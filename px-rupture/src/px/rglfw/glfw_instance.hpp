// name: glfw_instance.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

// RAII glfw init and termination
// glfw state
// call for glfw init and sets an error callback
// do not create more than one

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>

namespace px
{
	class glfw_instance final
	{
	public:
		bool is_valid() const noexcept
		{
			return m_init;
		}

	public:
		~glfw_instance()
		{
			if (m_init) {
				glfwTerminate();
			}
		}
		glfw_instance(GLFWerrorfun error_callback)
			: m_init(false)
		{
			if (error_callback) {
				glfwSetErrorCallback(error_callback);
			}
			m_init = glfwInit();
		}
		glfw_instance()
			: glfw_instance(&error_callback)
		{
		}

	private:
		static void error_callback(int error, char const* description)
		{
			throw std::runtime_error(std::string("px::glfw error, code #" + std::to_string(error) + " message: " + std::string(description)));
		}

	private:
		bool m_init;
	};
}