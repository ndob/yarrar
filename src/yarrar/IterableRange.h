#pragma once

namespace yarrar
{

template <typename Iterator>
class IterableRange
{
public:
    IterableRange(Iterator first, Iterator last)
        : m_first(first)
        , m_last(last)
    {
    }

    Iterator begin() const
    {
        return m_first;
    }

    Iterator end() const
    {
        return m_last;
    }

private:
    Iterator m_first;
    Iterator m_last;
};
}
