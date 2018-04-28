// enhancement_collection.hpp

#pragma once

#include <px/rl/enhancement.hpp>

#include <vector>

namespace px::rl {

    template <typename Effect, typename Integral = int, typename Real = double>
    class enhancement_collection {
    public:
        using enhancement_type = enhancement<Effect, Integral, Real>;
        using integral_type = Integral;
        using real_type = Real;

    public:
        enhancement_type accumulate(enhancement_type accumulator) const {
            for (auto const& element : container) {
                if (element.main == accumulator.main) {
                    accumulator += element;
                    accumulator.sub = element.sub;
                }
            }
            return accumulator;
        }

        template <Effect MainType>
        enhancement_type accumulate() {
            enhancement_type accumulator{ MainType };
            for (auto const& element : container) {
                if (element.main == MainType) {
                    accumulator += element;
                    accumulator.sub = element.sub;
                }
            }
            return accumulator;
        }

        bool has_effect(Effect efx) const {
            for (enhancement_type const& element : container) {
                if (element.main == efx) {
                    return true;
                }
            }
            return false;
        }

        bool has_effect(Effect efx, Integral sub) const {
            for (enhancement_type const& element : container) {
                if (element.main == efx && element.sub == sub) {
                    return true;
                }
            }
            return false;
        }

        Integral find_subtype(Effect efx, Integral or_else) const {
            for (enhancement_type const& element : container) {
                if (element.main == efx) {
                    return element.sub;
                }
            }
            return or_else;
        }

        // returns first enhancement by maintype, or prototype if it's not there
        enhancement_type find(enhancement_type or_else) const {
            for (enhancement_type const& element : container) {
                if (element.main == or_else.main) {
                    return element;
                }
            }
            return or_else;
        }

        void add(enhancement_type val) {
            container.push_back(val);
        }

        size_t size() const noexcept {
            return container.size();
        }

        template <typename Archive>
        void serialize(Archive & archive) {
            archive(container);
        }

    private:
        std::vector<enhancement<Effect, Integral, Real>> container;
    };
}