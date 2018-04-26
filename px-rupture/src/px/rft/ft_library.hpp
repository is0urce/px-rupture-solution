// name: ft_library.hpp

#pragma once

#include "ft_options.hpp"

#include <stdexcept>

namespace px {

    class ft_library {
    public:
        operator FT_Library() const noexcept {
            return library;
        }

        void swap(ft_library & that) noexcept {
            std::swap(library, that.library);
        }

    public:
        ~ft_library() {
            FT_Done_FreeType(library);
        }

        ft_library() {
            auto error = FT_Init_FreeType(&library);
            if (error) throw std::runtime_error("px::rft::ft_library::ctor() - .. an error occurred during library initialization ...");
        }

        ft_library(ft_library const&) = delete;
        ft_library& operator=(ft_library const&) = delete;

    private:
        FT_Library library;
    };
}