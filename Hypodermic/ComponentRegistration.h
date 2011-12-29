#ifndef		HYPODERMIC_COMPONENT_REGISTRATION_H_
# define	HYPODERMIC_COMPONENT_REGISTRATION_H_

# include <vector>

# include <boost/assert.hpp>
# include <boost/uuid/uuid.hpp>

# include <Hypodermic/IComponentLifetime.h>
# include <Hypodermic/IComponentRegistration.h>
# include <Hypodermic/IInstanceActivator.h>
# include <Hypodermic/InstanceOwnership.h>
# include <Hypodermic/InstanceSharing.h>
# include <Hypodermic/Service.h>


namespace Hypodermic
{

	class ComponentRegistration : public IComponentRegistration
	{
	public:
        ComponentRegistration(const boost::uuids::uuid& id,
                              IInstanceActivator* activator,
                              IComponentLifetime* lifetime,
                              InstanceSharing::Mode sharing,
                              InstanceOwnership::Mode ownership,
                              const std::vector< Service* >& services)
                              
            : id_(id)
            , activator_(activator)
            , sharing_(sharing)
            , ownership_(ownership)
            , lifetime_(lifetime)
            , services_(services)
            , target_(nullptr)
        {
            BOOST_ASSERT(activator != nullptr);
            BOOST_ASSERT(lifetime != nullptr);
        }

        ComponentRegistration(const boost::uuids::uuid& id,
                              IInstanceActivator* activator,
                              IComponentLifetime* lifetime,
                              InstanceSharing::Mode sharing,
                              InstanceOwnership::Mode ownership,
                              const std::vector< Service* >& services,
                              IComponentRegistration* target)
            : id_(id)
            , activator_(activator)
            , sharing_(sharing)
            , ownership_(ownership)
            , lifetime_(lifetime)
            , services_(services)
            , target_(target)
        {
            BOOST_ASSERT(activator != nullptr);
            BOOST_ASSERT(lifetime != nullptr);
            BOOST_ASSERT(target != nullptr);
        }

        IComponentRegistration* target()
        {
            return target_ != nullptr ? target_ : this;
        }

        const boost::uuids::uuid& id()
        {
            return id_;
        }

        IInstanceActivator* activator()
        {
            return activator_;
        }

        IComponentLifetime* lifetime()
        {
            return lifetime_;
        }

        InstanceSharing::Mode sharing()
        {
            return sharing_;
        }

        InstanceOwnership::Mode ownership()
        {
            return ownership_;
        }

        std::vector< Service* >& services()
        {
            return services_;
        }

        ///// <summary>
        ///// Fired when a new instance is required. The instance can be
        ///// provided in order to skip the regular activator, by setting the Instance property in
        ///// the provided event arguments.
        ///// </summary>
        //public event EventHandler<PreparingEventArgs> Preparing = (s, e) => { };

        void raisePreparing(IComponentContext* context)
        {
            //var args = new PreparingEventArgs(context, this, parameters);
            //Preparing(this, args);
            //parameters = args.Parameters;
        }

        ///// <summary>
        ///// Fired when a new instance is being activated. The instance can be
        ///// wrapped or switched at this time by setting the Instance property in
        ///// the provided event arguments.
        ///// </summary>
        //public event EventHandler<ActivatingEventArgs<object>> Activating = (s, e) => { };

        ///// <summary>
        ///// Called by the container once an instance has been constructed.
        ///// </summary>
        ///// <param name="context">The context in which the instance was activated.</param>
        ///// <param name="parameters">The parameters supplied to the activator.</param>
        ///// <param name="instance">The instance.</param>
        void raiseActivating(IComponentContext* context)
        {
            //var args = new ActivatingEventArgs<object>(context, this, parameters, instance);
            //Activating(this, args);
            //instance = args.Instance;
        }

        ///// <summary>
        ///// Fired when the activation process for a new instance is complete.
        ///// </summary>
        //public event EventHandler<ActivatedEventArgs<object>> Activated = (s, e) => { };

        ///// <summary>
        ///// Called by the container once an instance has been fully constructed, including
        ///// any requested objects that depend on the instance.
        ///// </summary>
        ///// <param name="context">The context in which the instance was activated.</param>
        ///// <param name="parameters">The parameters supplied to the activator.</param>
        ///// <param name="instance">The instance.</param>
        void raiseActivated(IComponentContext* context)
        {
            //Activated(this, new ActivatedEventArgs<object>(context, this, parameters, instance));
        }


        std::string toString()
        {
            return std::string("Component Registration");
            //    // Activator = {0}, Services = [{1}], Lifetime = {2}, Sharing = {3}, Ownership = {4}
            //    return string.Format(CultureInfo.CurrentCulture,
            //        ComponentRegistrationResources.ToStringFormat,
            //        Activator,
            //        Services.Select(s => s.Description).JoinWith(", "),
            //        Lifetime,
            //        Sharing,
            //        Ownership);
        }

	private:
        boost::uuids::uuid id_;
        IInstanceActivator* activator_;
        InstanceSharing::Mode sharing_;
        InstanceOwnership::Mode ownership_;
        IComponentLifetime* lifetime_;
        std::vector< Service* > services_;
        IComponentRegistration* target_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_COMPONENT_REGISTRATION_H_ */