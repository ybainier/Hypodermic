#ifdef      HYPODERMIC_REGISTRATION_BUILDER_H_
# ifndef    HYPODERMIC_REGISTRATION_BUILDER_HPP_
#  define   HYPODERMIC_REGISTRATION_BUILDER_HPP_

# include <Hypodermic/IInstanceActivator.h>
# include <Hypodermic/RegistrationData.h>
# include <Hypodermic/Service.h>
# include <Hypodermic/TypeCaster.h>
# include <Hypodermic/TypedService.h>


namespace Hypodermic
{

    template <class T, class RegistrationStyleT>
    RegistrationBuilder< T, RegistrationStyleT >::RegistrationBuilder(std::shared_ptr< Service > defaultService,
                                                                      std::shared_ptr< IInstanceActivator > activator,
                                                                      const RegistrationStyleT& registrationStyle)
        : registrationData_(defaultService)
        , activator_(activator)
        , registrationStyle_(registrationStyle)
    {
    }

    template <class T, class RegistrationStyleT>
    RegistrationData& RegistrationBuilder< T, RegistrationStyleT >::registrationData()
    {
        return registrationData_;
    }

    template <class T, class RegistrationStyleT>
    std::shared_ptr< IInstanceActivator > RegistrationBuilder< T, RegistrationStyleT >::activator()
    {
        return activator_;
    }

    template <class T, class RegistrationStyleT>
    RegistrationStyleT& RegistrationBuilder< T, RegistrationStyleT >::registrationStyle()
    {
        return registrationStyle_;
    }

    template <class T, class RegistrationStyleT>
    const std::unordered_map< std::type_index, std::shared_ptr< ITypeCaster > >&
    RegistrationBuilder< T, RegistrationStyleT >::typeCasters() const
    {
        return typeCasters_;
    }

    template <class T, class RegistrationStyleT>
    std::shared_ptr< typename RegistrationBuilder< T, RegistrationStyleT >::ParentType >
    RegistrationBuilder< T, RegistrationStyleT >::singleInstance()
    {
        auto& rd = registrationData();
        rd.sharing(InstanceSharing::Shared);
        rd.lifetime(std::make_shared< RootScopeLifetime >());
        return this->shared_from_this();
    }

    template <class T, class RegistrationStyleT>
    std::shared_ptr< typename RegistrationBuilder< T, RegistrationStyleT >::ParentType >
    RegistrationBuilder< T, RegistrationStyleT >::instancePerLifetimeScope()
    {
        auto& rd = registrationData();
        rd.sharing(InstanceSharing::Shared);
        rd.lifetime(std::make_shared< CurrentLifetimeScope >());
        return this->shared_from_this();
    }

    template <class T, class RegistrationStyleT>
    std::shared_ptr< typename RegistrationBuilder< T, RegistrationStyleT >::ParentType >
    RegistrationBuilder< T, RegistrationStyleT >::instancePerDependency()
    {
        auto& rd = registrationData();
        rd.sharing(InstanceSharing::None);
        rd.lifetime(std::make_shared< CurrentLifetimeScope >());
        return this->shared_from_this();
    }

    template <class T, class RegistrationStyleT>
    template <class ServiceT>
    std::shared_ptr< typename RegistrationBuilder< T, RegistrationStyleT >::ParentType >
    RegistrationBuilder< T, RegistrationStyleT >::as()
    {
        return as< ServiceT >(std::make_shared< TypedService >(typeid(ServiceT)));
    }

    template <class T, class RegistrationStyleT>
    std::shared_ptr< typename RegistrationBuilder< T, RegistrationStyleT >::ParentType >
    RegistrationBuilder< T, RegistrationStyleT >::asSelf()
    {
        return as< T >(std::make_shared< TypedService >(typeid(T)));
    }

    template <class T, class RegistrationStyleT>
    template <class ServiceT>
    std::shared_ptr< typename RegistrationBuilder< T, RegistrationStyleT >::ParentType >
    RegistrationBuilder< T, RegistrationStyleT >::as(std::shared_ptr< Service > service)
    {
        registrationData_.addService(service);
        typeCasters_.insert(std::make_pair(std::type_index(service->typeInfo()), std::make_shared< TypeCaster< T, ServiceT > >(service->typeInfo())));

        return this->shared_from_this();
    }


    template <class T, class RegistrationStyleT>
    template <class ServiceT>
    std::shared_ptr< typename RegistrationBuilder< T, RegistrationStyleT >::ParentType >
    RegistrationBuilder< T, RegistrationStyleT >::named(const std::string& serviceName)
    {
        if (serviceName.empty())
            throw std::invalid_argument("serviceName");
        return named< ServiceT >(serviceName, typeid(ServiceT));
    }

    template <class T, class RegistrationStyleT>
    template <class ServiceT>
    std::shared_ptr< typename RegistrationBuilder< T, RegistrationStyleT >::ParentType >
    RegistrationBuilder< T, RegistrationStyleT >::named(const std::string& serviceName, const std::type_info& serviceTypeInfo)
    {
        return as< ServiceT >(std::make_shared< KeyedService >(serviceName, serviceTypeInfo));
    }

} // namespace Hypodermic


# endif /* !HYPODERMIC_REGISTRATION_BUILDER_HPP_ */
#endif /* HYPODERMIC_REGISTRATION_BUILDER_H_ */