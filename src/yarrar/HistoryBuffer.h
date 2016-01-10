#pragma once

#include "IterableRange.h"

#include <cstdlib>
#include <vector>

namespace yarrar
{

template <typename T>
class HistoryBuffer
{
    using ContainerType = std::vector<T>;
    using Range = IterableRange<typename ContainerType::const_iterator>;

public:
    HistoryBuffer(size_t bufferSize)
        : m_bufferSize(bufferSize)
    {
    }

    void add(const T& val)
    {
        // Remove last element, if buffer is full.
        if(m_data.size() == m_bufferSize)
        {
            m_data.erase(m_data.begin());
        }
        m_data.push_back(val);
    }

    Range get(size_t maxNumberValues)
    {
        if(m_data.size() < maxNumberValues)
        {
            maxNumberValues = m_data.size();
        }

        return {
            m_data.cbegin(),
            m_data.cbegin() + maxNumberValues
        };
    };

    size_t size()
    {
        return m_data.size();
    }

    void clear()
    {
        m_data.clear();
    }

private:
    size_t m_bufferSize;
    ContainerType m_data;
};
}
