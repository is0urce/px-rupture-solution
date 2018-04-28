// name: buff.hpp
// type: c++ header
// auth: is0urce
// desc: buff template class

#pragma once

#include <px/rl/entity.hpp>
#include <px/rl/enhancement_collection.hpp>

namespace px::rl {

    template <typename Effect, typename Integral, typename Real>
    class buff
        : public entity
        , public rl::enhancement_collection<Effect, Integral, Real>
    {
    public:
        bool is_expired() const noexcept {
            return duration <= 0;
        }

        bool is_hidden() const noexcept {
            return hidden;
        }

        void set_duration(Integral remaining) noexcept {
            duration = remaining;
        }

        void set_hidden(bool flag) noexcept {
            hidden = flag;
        }

        void reduce_duration(Integral timespan) noexcept {
            duration -= timespan;
        }

        Integral timer() const noexcept {
            return duration;
        }

        template <typename Archive>
        void serialize(Archive & archive) {
            archive(static_cast<rl::entity &>(*this));
            archive(static_cast<rl::enhancement_collection<Effect, Integral, Real> &>(*this));
            archive(duration, hidden);
        }

    public:
        ~buff() = default;

        buff()
            : duration(0)
            , hidden(false) {
        }

    private:
        Integral    duration;
        bool        hidden;
    };
}