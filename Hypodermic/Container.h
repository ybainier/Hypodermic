#pragma once

#include <memory>
#include <vector>

#include "Hypodermic/ComponentContext.h"
#include "Hypodermic/IRegistrationScope.h"
#include "Hypodermic/IRuntimeRegistrationBuilder.h"
#include "Hypodermic/NestedRegistrationScope.h"


namespace Hypodermic
{

    class Container : public std::enable_shared_from_this< Container >
    {
    private:
        struct PrivateKey {};

    public:
        Container(const PrivateKey&,
                  const std::shared_ptr< IRegistrationScope >& registrationScope,
                  const std::shared_ptr< IRuntimeRegistrationBuilder >& runtimeRegistrationBuilder)
            : m_registrationScope(registrationScope)
            , m_runtimeRegistrationBuilder(runtimeRegistrationBuilder)
        {
        }

        static std::shared_ptr< Container > create(const std::shared_ptr< IRegistrationScope >& registrationScope,
                                                   const std::shared_ptr< IRuntimeRegistrationBuilder >& runtimeRegistrationBuilder)
        {
            return std::make_shared< Container >(PrivateKey(), registrationScope, runtimeRegistrationBuilder);
        }

        /// <summary>
        /// Create a nested scope
        /// </summary>
        /// <returns>A shared pointer to a new IRegistrationScope</returns>
        std::shared_ptr< IRegistrationScope > createNestedScope() const
        {
            return std::make_shared< NestedRegistrationScope >(m_registrationScope);
        }

        /// <summary>
        /// Resolve an instance of type T
        /// </summary>
        /// <param name="T">The type to resolve (i.e. get an instance of T)</param>
        /// <returns>A shared pointer on an instance of type T</returns>
        template <class T>
        std::shared_ptr< T > resolve()
        {
            ComponentContext componentContext(shared_from_this(), m_registrationScope, m_runtimeRegistrationBuilder);
            return componentContext.resolve< T >();
        }

        /// <summary>
        /// Resolve all instances of type T
        /// </summary>
        /// <param name="T">The type to resolve (i.e. get all instances of T)</param>
        /// <returns>A vector of shared pointers on instances of type T</returns>
        template <class T>
        std::vector< std::shared_ptr< T > > resolveAll()
        {
            ComponentContext componentContext(shared_from_this(), m_registrationScope, m_runtimeRegistrationBuilder);
            return componentContext.resolveAll< T >();
        }

    private:
        std::shared_ptr< IRegistrationScope > m_registrationScope;
        std::shared_ptr< IRuntimeRegistrationBuilder > m_runtimeRegistrationBuilder;
    };

} // namespace Hypodermic