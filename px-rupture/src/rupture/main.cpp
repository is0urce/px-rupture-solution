// name: main.cpp

#define GLEW_STATIC
#include <gl/glew.h>

#include "configuration.hpp"
#include "depot.hpp"
#include "settings.hpp"

#include <px/dev/logger.hpp>
#include <px/rglfw/rglfw.hpp>

#include <stdexcept>

namespace px {

	// create main window from configuration
	glfw_window create_window(configuration const& config, char const* name)
	{
		auto monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(monitor);

		int width = config.width;
		int height = config.height;

		glfwWindowHint(GLFW_DECORATED, config.border ? 1 : 0); // border
		if (config.fullscreen) {
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
			width = mode->width;
			height = mode->height;
		}

		return glfwCreateWindow(width, height, name, config.fullscreen ? monitor : nullptr, nullptr);
	}

	// enable context and load opengl extensions
	void create_context(glfw_window const& win, int swap_interval)
	{
		glfwMakeContextCurrent(win); // context
		glfwSwapInterval(swap_interval);
		glewInit();	// OpenGL extensions, after context
	}

	void process()
	{
		configuration config = configuration::from_document(depot::load_document(settings::configuration_path));

		glfw_instance glfw;
		glfw_window win = create_window(config, settings::application_name.c_str());
		create_context(win, config.vsync);

		glfw_callback event_callback(win);
		//event_callback.on_resize([&](int widht, int height) { std::cout << "Hello";	});
		//event_callback.on_key([&](int os_key, int /* scancode */, int action, int /* mods */) {
		//	if (action == GLFW_PRESS || action == GLFW_REPEAT) std::cout << "Hello";
		//});
		//event_callback.on_text([&](unsigned int codepoint) { std::cout << codepoint; });
		//event_callback.on_click([&](int button, int action, int /* mods */) {
		//	if (action == GLFW_PRESS) std::cout << "Hello";
		//});
		//event_callback.on_hover([&](double x, double y) {
		//	//game.hover(static_cast<int>(x), static_cast<int>(y));
		//});
		//event_callback.on_scroll([&](double horisontal, double vertical) {
		//	std::cout << "Hello";
		//});

		// main loop

		while (win.process()) {

		}
	}

	int run_application()
	{
		logger log(settings::log_path);

		try {
			process();
		}
		catch (std::exception const& exc) {
			log.message(exc.what());
			return -1;
		}
		catch (...) {
			log.message("unhandled exception");
			return -1;
		}

		return 0;
	}
}

int main()
{
	return px::run_application();
}