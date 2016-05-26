#pragma once

#include <functional>
#include <utility>


namespace Hypodermic
{
namespace Utils
{

    class InvokeAtScopeExit
    {
    public:
        explicit InvokeAtScopeExit(const std::function< void() >& func) throw()
            : m_func(func)
        {
        }

        ~InvokeAtScopeExit() throw()
        {
            if (m_func)
                m_func();
        }

    private:
        std::function< void() > m_func;
    };


    class InvokeAtScopeFailure
    {
    public:
        InvokeAtScopeFailure(const std::function< void() >& func) throw()
            : m_func(func)
        {
        }

        ~InvokeAtScopeFailure() throw()
        {
            if (std::uncaught_exception() && m_func)
                m_func();
        }

    private:
        std::function< void() > m_func;
    };

} // namespace Utils
} // namespace Hypodermic