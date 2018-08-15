// name: main.cpp
// type: c++
// auth: is0urce
// desc: programm entry point

#include "app/bootstrap.hpp"
#include "app/document.hpp"
#include "app/settings.hpp"
#include "draw/glew_options.hpp"
#include "key.hpp"
#include "shell.hpp"

#include <px/app/cfg.hpp>
#include <px/common/bindings.hpp>
#include <px/common/timer.hpp>
#include <px/dev/logger.hpp>
#include <px/dev/test.hpp>
#include <px/rglfw/rglfw.hpp>

#include <stdexcept>

namespace px {

    // enable context and load opengl extensions
    void create_context(glfw_window const& win, int swap_interval) {
        glfwMakeContextCurrent(win);        // context
        glfwSwapInterval(swap_interval);
        glewInit();                         // OpenGL extensions, after context
    }

    // prepare and start main loop
    void process() {

        // load window manager

        glfw_instance glfw;
        auto const monitor = glfwGetPrimaryMonitor();
        auto const video_mode = glfwGetVideoMode(monitor);

        // load settings

        auto const binds = bindings<int, key>::from_document(document::load_document(settings::bindings_path));
        cfg configuration(settings::configuration_path);

        // create windows

        bool const is_fullscreen = configuration["screen.mode"] == "fullscreen";
        bool const is_border = configuration["screen.mode"] == "window";
        int const window_width = is_fullscreen ? video_mode->width : configuration["screen.width"];
        int const window_height = is_fullscreen ? video_mode->height : configuration["screen.height"];

        if (!is_border) {
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        }
        if (is_fullscreen) {
            glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, video_mode->refreshRate);
        }

        glfw_window win(glfwCreateWindow(window_width, window_height, settings::application_name, is_fullscreen ? monitor : nullptr, nullptr));
        create_context(win, configuration["screen.vsync"]);

        // create environment & register events

        shell game(window_width, window_height, &configuration);

        glfw_callback evt(win);
        evt.on_resize([&](int width, int height) {
            game.resize(width, height);
            // store size in configuration file
            configuration["screen.width"] = width;
            configuration["screen.height"] = height;
        });
        evt.on_click([&](int mouse_button, int action, int /* mods */) {
            game.click(mouse_button, action == GLFW_PRESS);
        });
        evt.on_hover([&](double x, double y) {
            game.hover(x, y);
        });
        evt.on_scroll([&](double horisontal, double vertical) {
            game.scroll(horisontal, vertical);
        });
        evt.on_text([&](unsigned int codepoint) {
            game.text(codepoint);
        });
        evt.on_key([&](int os_code, int /* scancode */, int action, int /* mods */) {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                game.press(binds.value(os_code, key::not_valid));
            }
        });

        // main loop

        timer<glfw_time> time;
        game.seed(static_cast<unsigned int>(time.raw()));
        while (game.is_running() && win.process()) {
            game.frame(time);
        }

        glfwMakeContextCurrent(nullptr);
    }

    int run_application() {
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

int main() {
    px::bootstrap();
    return px::run_application();
}