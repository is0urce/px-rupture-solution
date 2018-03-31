// name: timer.hpp
// type: c++ header
// desc: class declaration & implementation

// time track

#pragma once

namespace px {

    template <typename QueryOperator>
    class timer {
    private:
        QueryOperator m_query;
    public:
        using query_type = QueryOperator;
        using precision_type = decltype(m_query());

    public:
        timer()
            : m_query(QueryOperator{})
        {
            restart();
        }
        timer(QueryOperator const& query)
            : m_query(query)
        {
            restart();
        }

        void restart() {
            start = m_query();
        }
        void restart(precision_type time) {
            start = time;
        }

        // time in seconds
        auto measure() const {
            return m_query() - start;
        }

        auto measure() {
            return m_query() - start;
        }

        auto operator()() const {
            return measure();
        }

        auto operator()() {
            return measure();
        }

        operator typename timer::precision_type() const {
            return measure();
        }

    private:
        precision_type start;
    };
}