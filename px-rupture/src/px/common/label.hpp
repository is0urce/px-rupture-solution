// name: label.hpp

#pragma once

// encoding-agnostic fixed-size string
// max size = len - 1

#include <cstring>

namespace px {

    template <size_t Len>
    class label final {
        template <size_t RLen>
        friend class label;

    public:
        size_t length() const {
            return std::strlen(data);
        }
        const char * c_str() const noexcept {
            return data;
        }

    public:
        label() {
            static_assert(Len > 0, "label::ctor() - len == 0");
            std::memset(data, 0, Len);
        }

        template <size_t RLen>
        label(const char(&arg)[RLen]) {
            static_assert(Len > 0, "label::ctor() - len == 0");
            static_assert(RLen <= Len, "label::ctor() - rlen > len");
            std::memcpy(data, arg, RLen);
            std::memset(data + RLen, 0, Len - RLen);
        }

    public:
        bool operator==(label const& rh) {
            return std::memcmp(data, rh.data, Len) == 0;
        }
        bool operator<(label const& rh) {
            return std::memcmp(data, rh.data, Len) < 0;
        }
        bool operator<=(label const& rh) {
            return std::memcmp(data, rh.data, Len) <= 0;
        }
        bool operator!=(label const& rh) {
            return !operator==(rh);
        }
        bool operator>(label const& rh) {
            return !operator<=(rh);
        }
        bool operator>=(label const& rh) {
            return !operator<(rh);
        }

        // concatenation
        // ::DOWENEEDITANYWAY?::
        template <size_t RLen>
        label<Len + RLen - 1> operator+(label<RLen> const& rh) {
            label<Len + RLen - 1> result;
            std::memcpy(result.data, data, Len - 1);
            size_t len = length();
            std::memcpy(result.data + len, rh.data, RLen);
            return result;
        }

    private:
        char data[Len];
    };
}