#ifndef		HYPODERMIC_IRESOLVE_OPERATION_H_
# define	HYPODERMIC_IRESOLVE_OPERATION_H_


namespace Hypodermic
{
    class IComponentRegistration;
    class ISharingLifetimeScope;


	class IResolveOperation
	{
    public:
		virtual void* getOrCreateInstance(ISharingLifetimeScope* currentOperationScope,
										  IComponentRegistration* registration) = 0;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_IRESOLVE_OPERATION_H_ */