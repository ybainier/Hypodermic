#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numeric>
#include <vector>


namespace Hypodermic
{
namespace Testing
{
namespace Utils
{

    template <class TDuration>
    class PercentileCalculator
    {
    public:
        std::int64_t count() const
        {
            return static_cast< std::int64_t >(m_latencies.size());
        }

        TDuration computeAverage() const
        {
            return !m_latencies.empty()
                ? TDuration(std::accumulate(std::begin(m_latencies), std::end(m_latencies), TDuration(0)) / TDuration(m_latencies.size()))
                : TDuration(0);
        }

        TDuration computeMax() const
        {
            return !m_latencies.empty()
                ? *std::minmax_element(std::begin(m_latencies), std::end(m_latencies)).second
                : TDuration(0);
        }

        TDuration computeMin() const
        {
            return !m_latencies.empty()
                ? *std::minmax_element(std::begin(m_latencies), std::end(m_latencies)).first
                : TDuration(0);
        }

        TDuration computePercentile(double percentile)
        {
            if (m_latencies.empty())
                return TDuration(0);

            std::sort(std::begin(m_latencies), std::end(m_latencies));

            auto index = static_cast< int >(std::floor(static_cast< double >(m_latencies.size()) * (100.0 - percentile) / 100.0)) + 1;
            return m_latencies[static_cast< int >(m_latencies.size()) - index];
        }

        void pushLatency(const TDuration& latency)
        {
            m_latencies.push_back(latency);
        }

    private:
        std::vector< TDuration > m_latencies;
    };
    
} // namespace Utils
} // namespace Testing
} // namespace Hypodermic
