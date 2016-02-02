#pragma once

#include <functional>
#include <utility>


namespace Hypodermic
{
namespace Utils
{

    namespace Details
    {

        template <class TCallable>
        class ScopeExitFunction
        {
        public:
            ScopeExitFunction(TCallable&& func) throw()
                : m_func(std::forward< TCallable >(func))
                , m_empty(false)
            {
            }

            ScopeExitFunction(ScopeExitFunction&& moveFrom) throw()
                : m_func(std::move(moveFrom.m_func))
                , m_empty(moveFrom.m_empty)
            {
                moveFrom.m_empty = true;
            }

            ScopeExitFunction() throw()
                : m_func()
                , m_empty(true)
            {
            }

            ~ScopeExitFunction() throw()
            {
                if (!m_empty)
                    m_func();
            }

            ScopeExitFunction& operator=(ScopeExitFunction&& rhs)
            {
                m_func = std::move(rhs.m_func);
                m_empty = rhs.m_empty;

                rhs.m_empty = true;

                return *this;
            }

        private:
            ScopeExitFunction& operator=(const ScopeExitFunction&) = delete;

            TCallable m_func;
            bool m_empty;
        };

    } // namespace Details

    template <class TCallable>
    Details::ScopeExitFunction< TCallable > atScopeExit(TCallable&& func)
    {
        return Details::ScopeExitFunction< TCallable >(std::forward< TCallable >(func));
    }

} // namespace Utils
} // namespace Hypodermic