// name: color.hpp
// type: c++ header
// desc: class with implementation
// auth: is0urce

// color with real rgba components
// + and * operators change alpha components

#pragma once

#include <algorithm>    // max
#include <cmath>        // sin, cos for colorspace transforms

namespace px {

    class color {
    public:
        using component = double;

        // attributes
    public:
        component R;
        component G;
        component B;
        component A;

    public:
        // operators

        constexpr color operator-() const noexcept { return color(-R, -G, -B, A); };
        color & operator+=(color const& rh) noexcept { R += rh.R; G += rh.G; B += rh.B; A += rh.A; return *this; };
        color & operator-=(color const& rh) noexcept { R -= rh.R; G -= rh.G; B -= rh.B; A -= rh.A; return *this; };
        color & operator*=(color const& rh) noexcept { R *= rh.R, G *= rh.G, B *= rh.B, A *= rh.A; return *this; };

        constexpr bool operator==(color const& rh) const noexcept { return R == rh.R && G == rh.G && B == rh.B && A == rh.A; }
        constexpr bool operator!=(color const& rh) const noexcept { return !(*this == rh); }
        color operator+(color c) const noexcept { c += *this; return c; }
        color operator-(color c) const noexcept { c -= *this; return c; }
        color operator*(color c) const noexcept { c *= *this; return c; }

        color operator*(component c) const noexcept { return color(R * c, G * c, B * c, A * c); };
        color operator/(component c) const { return color(R / c, G / c, B / c, A / c); };
        color operator*=(component c) noexcept { *this = *this * c; return *this; };
        color operator/=(component c) { *this = *this / c; return *this; };

        void set_rgba(unsigned int r, unsigned int g, unsigned int b, unsigned int a) noexcept { R = r / 255.0; G = g / 255.0; B = b / 255.0; A = a / 255.0; }
        void set_rgb(unsigned int r, unsigned int g, unsigned int b) noexcept { R = r / 255.0; G = g / 255.0; B = b / 255.0; }
        void set_hex(unsigned int hex) noexcept { set_rgb(hex / 256 / 256 % 256, hex / 256 % 256, hex % 256); };
        void shift_hue(double angle) { *this = transform_hue(*this, angle); };
        void shift_brightness(double scale) { *this = transform_hsv(*this, 0, 1.0, scale); }
        void shift_hsv(double hue, double saturation, double v) { *this = transform_hsv(*this, hue, saturation, v); };
        color transform_hsv(double hue, double saturation, double v) const { return transform_hsv(*this, hue, saturation, v); };
        color average(color other) const noexcept { other += *this; other /= 2; return other; };
        component luminance() const noexcept { return 0.2125 * R + 0.7154 * G + 0.0721 * B; };
        color lerp(color b, component t) const noexcept { b *= t; b += *this * (t - 1); return b; }
        void multiple_rgb(component multiplier) noexcept { R *= multiplier; G *= multiplier; B *= multiplier; }
        color multiplied_rgb(component multiplier) const noexcept { auto result = *this; result.multiple_rgb(multiplier); return result; }
        color dissolved(component w) const noexcept { auto result = *this; result.A *= (1 - w); return result; }

        // i-o, we have dedicated repeat function arguments to compliment structures like solid-tinted polygons
        template <typename Memory>
        void write(Memory * memory) const {
            memory[0] = static_cast<Memory>(R);
            memory[1] = static_cast<Memory>(G);
            memory[2] = static_cast<Memory>(B);
            memory[3] = static_cast<Memory>(A);
        };

        template <typename Memory>
        void write(Memory * memory, size_t repeat) const {
            for (size_t i = 0; i < repeat; ++i) {
                write(memory);
                memory += sizeof(*this) / sizeof(component); // plus num of components
            }
        }

        void write(component * memory) const {
            memory[0] = R;
            memory[1] = G;
            memory[2] = B;
            memory[3] = A;
        }

        void write(component * memory, size_t repeat) const {
            for (size_t i = 0; i < repeat; ++i) {
                write(memory);
                memory += sizeof(*this) / sizeof(component); // plus num of components
            }
        }

        // space is not serizlized
        template <typename Archive>
        void serialize(Archive & archive) {
            archive(R, G, B, A);
        }

        // ctor & dtor
    public:
        constexpr color() noexcept
            : R{}
            , G{}
            , B{}
            , A(1) {
        }

        constexpr color(component r, component g, component b) noexcept
            : R(r)
            , G(g)
            , B(b)
            , A(1) {
        }

        constexpr color(component r, component g, component b, component a) noexcept
            : R(r)
            , G(g)
            , B(b)
            , A(a) {
        }

        constexpr color(unsigned int hex) noexcept
            : R(hex / 256 / 256 % 256 / 255.0)
            , G(hex / 256 % 256 / 255.0)
            , B(hex % 256 / 255.0)
            , A(1) {
        }

    public:
        // constructor functions

        static constexpr color rgb(unsigned char r, unsigned char g, unsigned char b) noexcept {
            return color(r / 255.0, g / 255.0, b / 255.0);
        }

        static constexpr color rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a) noexcept {
            return color(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
        }

        static constexpr color black() noexcept {
            return color(0, 0, 0, 1);
        };

        static constexpr color white() noexcept {
            return color(1, 1, 1, 1);
        };

        // aux

        static color average(color const& a, color const& b) { return (a + b) / 2; }
        static color average(color const& a, color const& b, color const& c) { return (a + b + c) / 3; }
        static color average(color const& a, color const& b, color const& c, color const& d) { return (a + b + c + d) / 4; }
        static color min(color const& a, color const& b) { return color(std::min(a.R, b.R), std::min(a.G, b.G), std::min(a.B, b.B), std::min(a.A, b.A)); }
        static color max(color const& a, color const& b) { return color(std::max(a.R, b.R), std::max(a.G, b.G), std::max(a.B, b.B), std::max(a.A, b.A)); }

        // hsv transformation
        // hue - hue shift (in degrees) in hardcoded 'default' colorspace preset
        // saturation - saturation multiplier (scalar), v - value multiplier (scalar)
        static color transform_hsv(color const& in, component hue, component saturation, component v) {
            const component vs_u = v * saturation * std::cos(hue);
            const component vs_w = v * saturation * std::sin(hue);

            color ret(in);
            ret.R = 0
                + (0.299 * v + 0.701 * vs_u + 0.168 * vs_w) * in.R
                + (0.587 * v - 0.587 * vs_u + 0.330 * vs_w) * in.G
                + (0.114 * v - 0.114 * vs_u - 0.497 * vs_w) * in.B;
            ret.G = 0
                + (0.299 * v - 0.299 * vs_u - 0.328 * vs_w) * in.R
                + (0.587 * v + 0.413 * vs_u + 0.035 * vs_w) * in.G
                + (0.114 * v - 0.114 * vs_u + 0.292 * vs_w) * in.B;
            ret.B = 0
                + (0.299 * v - 0.300 * vs_u + 1.250 * vs_w) * in.R
                + (0.587 * v - 0.588 * vs_u - 1.050 * vs_w) * in.G
                + (0.114 * v + 0.886 * vs_u - 0.203 * vs_w) * in.B;
            return ret;
        }

        static color transform_hue(color const& in, component angle) {
            const component u = std::cos(angle);
            const component w = std::sin(angle);

            color ret(in);
            ret.R = 0
                + (0.299 + 0.701 * u + 0.168 * w) * in.R
                + (0.587 - 0.587 * u + 0.330 * w) * in.G
                + (0.114 - 0.114 * u - 0.497 * w) * in.B;
            ret.G = 0
                + (0.299 - 0.299 * u - 0.328 * w) * in.R
                + (0.587 + 0.413 * u + 0.035 * w) * in.G
                + (0.114 - 0.114 * u + 0.292 * w) * in.B;
            ret.B = 0
                + (0.299 - 0.300 * u + 1.250 * w) * in.R
                + (0.587 - 0.588 * u - 1.050 * w) * in.G
                + (0.114 + 0.886 * u - 0.203 * w) * in.B;
            return ret;
        }
    };
}