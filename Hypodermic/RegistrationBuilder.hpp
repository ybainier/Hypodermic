#ifdef      HYPODERMIC_REGISTRATION_BUILDER_H_
# ifndef    HYPODERMIC_REGISTRATION_BUILDER_HPP_
#  define   HYPODERMIC_REGISTRATION_BUILDER_HPP_

# include <Hypodermic/ActivatedData.h>
# include <Hypodermic/ActivatingData.h>
# include <Hypodermic/IComponentRegistration.h>
# include <Hypodermic/IInstanceActivator.h>
# include <Hypodermic/PreparingData.h>
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
        const auto& typeInfo = typeid(ServiceT);
        return as(std::make_shared< TypedService >(typeid(ServiceT)), std::make_shared< TypeCaster< T, ServiceT > >(typeInfo));
    }

    template <class T, class RegistrationStyleT>
    std::shared_ptr< typename RegistrationBuilder< T, RegistrationStyleT >::ParentType >
    RegistrationBuilder< T, RegistrationStyleT >::asSelf()
    {
        const auto& typeInfo = typeid(T);
        return as(std::make_shared< TypedService >(typeInfo), std::make_shared< TypeCaster< T, T > >(typeInfo));
    }

    template <class T, class RegistrationStyleT>
    std::shared_ptr< typename RegistrationBuilder< T, RegistrationStyleT >::ParentType >
    RegistrationBuilder< T, RegistrationStyleT >::as(std::shared_ptr< Service > service, std::shared_ptr< ITypeCaster > typeCaster)
    {
        registrationData_.addService(service);
        typeCasters_.insert(std::make_pair(std::type_index(service->typeInfo()), typeCaster));

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
        const auto& typeInfo = typeid(ServiceT);
        return as(std::make_shared< KeyedService >(serviceName, serviceTypeInfo), std::make_shared< TypeCaster< T, ServiceT > >(typeInfo));
    }


    template <class T, class RegistrationStyleT>
    std::shared_ptr< typename RegistrationBuilder< T, RegistrationStyleT >::ParentType >
    RegistrationBuilder< T, RegistrationStyleT >::targeting(std::shared_ptr< IComponentRegistration > target)
    {
        if (target == nullptr)
            throw std::invalid_argument("target");

        registrationStyle().target(target);
        return this->shared_from_this();
    }

    template <class T, class RegistrationStyleT>
    std::shared_ptr< typename RegistrationBuilder< T, RegistrationStyleT >::ParentType >
    RegistrationBuilder< T, RegistrationStyleT >::onPreparing(std::function< void(IPreparingData&) > callback)
    {
        registrationData_.preparingCallbacks().push_back(
            [callback](PreparingData& preparingData) -> void
            {
                PreparingData data(preparingData.componentContext(), preparingData.componentRegistration());
                callback(data);
            }
        );

        return this->shared_from_this();
    }

    template <class T, class RegistrationStyleT>
    std::shared_ptr< typename RegistrationBuilder< T, RegistrationStyleT >::ParentType >
    RegistrationBuilder< T, RegistrationStyleT >::onActivating(std::function< void(IActivatingData< T >&) > callback)
    {
        registrationData_.activatingCallbacks().push_back(
            [callback](ActivatingData< void >& activatingData) -> void
            {
                ActivatingData< T > data(activatingData.componentContext(), activatingData.componentRegistration(), std::static_pointer_cast< T >(activatingData.instance()));
                callback(data);
                activatingData.instance(data.instance());
            }
        );

        return this->shared_from_this();
    }

    template <class T, class RegistrationStyleT>
    std::shared_ptr< typename RegistrationBuilder< T, RegistrationStyleT >::ParentType >
    RegistrationBuilder< T, RegistrationStyleT >::onActivated(std::function< void(IActivatedData< T >&) > callback)
    {
        registrationData_.activatedCallbacks().push_back(
            [callback](ActivatedData< void >& activatedData) -> void
            {
                ActivatedData< T > data(activatedData.componentContext(), activatedData.componentRegistration(), std::static_pointer_cast< T >(activatedData.instance()));
                callback(data);
            }
        );

        return this->shared_from_this();
    }

} // namespace Hypodermic


# endif /* !HYPODERMIC_REGISTRATION_BUILDER_HPP_ */
#endif /* HYPODERMIC_REGISTRATION_BUILDER_H_ */