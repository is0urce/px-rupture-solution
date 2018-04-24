// name: configuration.hpp
// type: c++
// auth: is0urce
// desc: configuration

#pragma once

#include <stdexcept>

namespace px {

    class configuration {
    public:
        template <typename Document>
        void load(Document && document) {
            try {
                width = document["window"]["width"];
                height = document["window"]["height"];
                vsync = document["window"]["vsync"];
                border = document["window"]["border"];
                fullscreen = document["window"]["fullscreen"];
            }
            catch (std::exception const& exc) {
                throw std::runtime_error("px::configuration::load(document) - error while loading configuration, what=" + std::string(exc.what()));
            }
            catch (...) {
                throw std::runtime_error("px::configuration::load(document) - unknown error");
            }
        }

        template <typename Document>
        static configuration from_document(Document && document) {
            configuration result;
            result.load(std::forward<Document>(document));
            return result;
        }

        static configuration from_default() {
            configuration result;
            result.width = 800;
            result.height = 600;
            result.vsync = 1;
            result.border = true;
            result.fullscreen = false;
            return result;
        }

    public:
        unsigned int    width;
        unsigned int    height;
        int             vsync;
        bool            border;
        bool            fullscreen;
    };
}