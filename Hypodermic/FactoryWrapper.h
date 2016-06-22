#pragma once

#include <functional>
#include <memory>


namespace Hypodermic
{

    template <class T>
    class FactoryWrapper
    {
    public:
        explicit FactoryWrapper(const std::function< std::shared_ptr< T >() >& factory)
            : m_factory(factory)
        {
        }

        const std::function< std::shared_ptr< T >() >& getFactory() const
        {
            return m_factory;
        }

    private:
        std::function< std::shared_ptr< T >() > m_factory;
    };

} // namespace Hypodermic