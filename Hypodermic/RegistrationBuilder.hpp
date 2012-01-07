#ifdef      HYPODERMIC_REGISTRATION_BUILDER_H_
# ifndef    HYPODERMIC_REGISTRATION_BUILDER_HPP_
#  define   HYPODERMIC_REGISTRATION_BUILDER_HPP_

# include <typeinfo>

# include <Hypodermic/IInstanceActivator.h>
# include <Hypodermic/RegistrationData.h>
# include <Hypodermic/Service.h>
# include <Hypodermic/TypeCaster.h>
# include <Hypodermic/TypedService.h>


namespace Hypodermic
{

    template <class T, class RegistrationStyleT>
    RegistrationBuilder< T, RegistrationStyleT >::RegistrationBuilder(Service* defaultService,
                                                                      IInstanceActivator* activator,
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
    IInstanceActivator* RegistrationBuilder< T, RegistrationStyleT >::activator()
    {
        return activator_;
    }

    template <class T, class RegistrationStyleT>
    RegistrationStyleT& RegistrationBuilder< T, RegistrationStyleT >::registrationStyle()
    {
        return registrationStyle_;
    }

    template <class T, class RegistrationStyleT>
    const std::vector< ITypeCaster* >& RegistrationBuilder< T, RegistrationStyleT >::typeCasters() const
    {
        return typeCasters_;
    }

    template <class T, class RegistrationStyleT>
    IRegistrationBuilder< T, RegistrationStyleT >* RegistrationBuilder< T, RegistrationStyleT >::singleInstance()
    {
        auto& rd = registrationData();
        rd.sharing(InstanceSharing::Shared);
        rd.lifetime(new RootScopeLifetime);
        return this;
    }

    template <class T, class RegistrationStyleT>
    IRegistrationBuilder< T, RegistrationStyleT >* RegistrationBuilder< T, RegistrationStyleT >::instancePerLifetimeScope()
    {
        auto& rd = registrationData();
        rd.sharing(InstanceSharing::Shared);
        rd.lifetime(new CurrentLifetimeScope);
        return this;
    }

    template <class T, class RegistrationStyleT>
    IRegistrationBuilder< T, RegistrationStyleT >* RegistrationBuilder< T, RegistrationStyleT >::instancePerDependency()
    {
        auto& rd = registrationData();
        rd.sharing(InstanceSharing::None);
        rd.lifetime(new CurrentLifetimeScope);
        return this;
    }

    template <class T, class RegistrationStyleT>
    template <class ServiceT>
    IRegistrationBuilder< T, RegistrationStyleT >* RegistrationBuilder< T, RegistrationStyleT >::as()
    {
        const std::type_info& serviceTypeInfo = typeid(ServiceT);
        
        registrationData_.addService(new TypedService(serviceTypeInfo));
        typeCasters_.push_back(new TypeCaster< T, ServiceT >(serviceTypeInfo));

        return this;
    }

} // namespace Hypodermic


# endif /* !HYPODERMIC_REGISTRATION_BUILDER_HPP_ */
#endif /* HYPODERMIC_REGISTRATION_BUILDER_H_ */