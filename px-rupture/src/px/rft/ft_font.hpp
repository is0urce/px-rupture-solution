// name: ft_font.hpp
// type: c++ header
// auth: is0urce
// desc: font class

#pragma once

#include "ft_options.hpp"
#include "ft_face.hpp"

#include <algorithm>
#include <cstring>
#include <map>
#include <stdexcept>
#include <tuple>
#include <vector>

namespace px
{
    class ft_font final {
    public:
        struct glyph final {
            unsigned int pixels_left;
            unsigned int pixels_top;
            unsigned int pixels_width;
            unsigned int pixels_height;

            double sx, sy, dx, dy;
            double left, top;

            long advance_h;
            long advance_v;
            long width;
            long height;
            long bearing_hx;
            long bearing_hy;
        };

    public:
        auto width() const noexcept {
            return m_size;
        }

        auto height() const noexcept {
            return m_size;
        }

        glyph const& operator[](unsigned int code) {
            auto it = m_map.find(code);
            return (it == m_map.end()) ? rasterize(code) : it->second;
        }

        glyph & rasterize(unsigned int code) {
            return rasterize(code, code);
        }

        void const* download(unsigned int & width, unsigned int & height) {
            void const * result;
            std::tie(result, width, height) = download();
            return result;
        }

        std::tuple<void const*, unsigned int, unsigned int> download() {
            m_dirty = false;
            return std::tuple<void const*, unsigned int, unsigned int>{ m_atlas.data(), m_width, m_height };
        }

        bool updated() const noexcept {
            return m_dirty;
        }

        auto kerning(unsigned int l1, unsigned int l2) const {
            return m_face.kerning(l1, l2) >> 6;
        }

    public:
        ft_font(const char* name, unsigned int size, unsigned int atlas_size)
            : m_face(m_lib, name)
            , m_size(size)
            , m_width(atlas_size)
            , m_height(atlas_size)
            , m_pen_x(0)
            , m_pen_y(0)
            , m_max_height(0)
            , m_dirty(true)
            , m_version(0) {
            m_face.pixel_size(size);
            m_atlas.assign(m_width * m_height, 0); // black
        }

    private:
        void expand_atlas() {
            m_width *= 2;
            m_height *= 2;
            m_atlas.assign(m_width * m_height, 0);
            m_pen_x = 0;
            m_pen_y = 0;
            m_max_height = 0;
            m_dirty = true;
            ++m_version;
        }

        glyph & rasterize(unsigned int code, unsigned int into) {
            // load data & create glyph
            auto error = FT_Load_Char(m_face, code, FT_LOAD_RENDER);
            if (error) throw std::runtime_error("px::font::rasterize() error");
            auto slot = m_face->glyph;
            glyph & result = m_map[into];

            // select area in atlas to rasterize into
            unsigned int x_stride = slot->bitmap.width + padding * 2;
            unsigned int y_stride = slot->bitmap.rows + padding * 2;
            if (m_pen_x + x_stride > m_width)
            {
                m_pen_x = 0;
                m_pen_y += m_max_height;
                m_max_height = 0;
            }

            // expand atlas
            if (m_pen_y + y_stride > m_height) {
                expand_atlas();
                return rasterize(code, into);
            }

            // copy bitmap
            for (unsigned int j = 0, h = slot->bitmap.rows; j != h; ++j) {
                std::memcpy(&m_atlas[(m_pen_y + j + padding) * m_width + m_pen_x + padding], &slot->bitmap.buffer[j * slot->bitmap.pitch], slot->bitmap.width);
            }

            // assign data
            result.advance_h = slot->metrics.horiAdvance;
            result.advance_v = slot->metrics.vertAdvance;
            result.width = slot->metrics.width;
            result.height = slot->metrics.height;
            result.bearing_hx = slot->metrics.horiBearingX;
            result.bearing_hy = slot->metrics.horiBearingY;

            result.sx = (m_pen_x + padding) / static_cast<double>(m_width);
            result.dx = (m_pen_x + padding + slot->bitmap.width) / static_cast<double>(m_width);
            result.dy = (m_pen_y + padding) / static_cast<double>(m_height);
            result.sy = (m_pen_y + padding + slot->bitmap.rows) / static_cast<double>(m_height);
            result.pixels_left = slot->bitmap_left;
            result.pixels_top = slot->bitmap_top;
            result.pixels_width = slot->bitmap.width;
            result.pixels_height = slot->bitmap.rows;
            result.left = slot->bitmap_left / static_cast<double>(m_height);
            result.top = slot->bitmap_top / static_cast<double>(m_height);

            // adjust pen
            m_pen_x += x_stride;
            m_max_height = std::max(m_max_height, y_stride);

            // register changes
            m_dirty = true;
            ++m_version;

            return result;
        }

    private:
        ft_library                      m_lib;
        ft_face                         m_face;
        std::map<unsigned int, glyph>   m_map;
        std::vector<unsigned char>      m_atlas;    // bitmap atlas
        unsigned int                    m_size;
        unsigned int                    m_width;
        unsigned int                    m_height;
        unsigned int                    m_pen_x;
        unsigned int                    m_pen_y;
        unsigned int                    m_max_height;
        bool                            m_dirty;
        unsigned int                    m_version;

    private:
        static const unsigned int       padding = 2;
    };
}