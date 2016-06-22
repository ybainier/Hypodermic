#pragma once

#include <functional>
#include <memory>


namespace Hypodermic
{

    class Container;

    
    template <class T>
    class FactoryBuilder
    {
    public:
        explicit FactoryBuilder(const std::weak_ptr< Container >& container)
            : m_container(container)
        {
        }

        std::function< std::shared_ptr< T >() > build() const
        {
            auto weakContainer = m_container;

            return [weakContainer]() -> std::shared_ptr< T >
            {
                auto&& c = weakContainer.lock();
                if (c == nullptr)
                    return nullptr;

                return c->template resolve< T >();
            };
        }

    private:
        std::weak_ptr< Container > m_container;
    };

} // namespace Hypodermic