#ifndef		HYPODERMIC_IRESOLVE_OPERATION_H_
# define	HYPODERMIC_IRESOLVE_OPERATION_H_

# include <memory>


namespace Hypodermic
{
    class IComponentRegistration;
    class ISharingLifetimeScope;


	class IResolveOperation
	{
    public:
        virtual ~IResolveOperation() {}

		virtual std::shared_ptr< void > getOrCreateInstance(const std::shared_ptr< ISharingLifetimeScope >& currentOperationScope,
										                    const std::shared_ptr< IComponentRegistration >& registration) = 0;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_IRESOLVE_OPERATION_H_ */