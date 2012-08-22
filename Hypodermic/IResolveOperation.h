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
        virtual ~IResolveOperation() = 0;

		virtual std::shared_ptr< void > getOrCreateInstance(std::shared_ptr< ISharingLifetimeScope > currentOperationScope,
										                    std::shared_ptr< IComponentRegistration > registration) = 0;
	};

    inline IResolveOperation::~IResolveOperation() {}

} // namespace Hypodermic


#endif /* !HYPODERMIC_IRESOLVE_OPERATION_H_ */