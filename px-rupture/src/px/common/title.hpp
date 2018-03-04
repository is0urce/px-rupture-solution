// name: label.hpp

#pragma once

// encoding-agnostic fixed-size string
// max size = len - 1

#include <cstring>

namespace px {

    template <size_t Len>
    class label final {
        template <size_t RLen> friend class label<RLen>;

    public:
        size_t length() const {
            return std::strlen(data);
        }
        const char * c_str() const noexcept {
            return data;
        }

    public:
        label() {
            static_assert(Len > 0, "label::ctor() - len == 0")
            std::memset(data, 0, Len);
        }

        template <size_t RLen>
        label & operator=(char const& rhdata[RLen]) {
            static_assert (RLen <= Len, "label::operator=() - rlen > len");
            std::memcpy(data, rhdata, RLen);
        }

        bool operator==(label const& rh) {
            return std::memcmp(data, rh.data, Len) == 0;
        }
        bool operator!=(label const& rh) {
            return !operator==(rh);
        }
        bool operator<(label const& rh) {
            return std::memcmp(data, rh.data, Len) < 0;
        }

        template <size_t RLen>
        label<Len + RLen> operator+(label<RLen> const& rh) {
            label<Len + RLen> result;
            std::memcpy(result.data, data, Len - 1);
            std::memcpy(result.data + Len - 1, rh.data, RLen);
            return result;
        }

    private:
        char data[8];
    };
}