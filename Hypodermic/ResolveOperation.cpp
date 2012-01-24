#include <boost/foreach.hpp>

#include "DependencyResolutionException.h"
#include "InstanceLookup.h"
#include "ISharingLifetimeScope.h"
#include "ResolveOperation.h"


namespace Hypodermic
{

    ResolveOperation::ResolveOperation(std::shared_ptr< ISharingLifetimeScope > mostNestedLifetimeScope)
        : mostNestedLifetimeScope_(mostNestedLifetimeScope)
        , callDepth_(0)
        , ended_(false)
    {
        if (mostNestedLifetimeScope == nullptr)
            throw std::invalid_argument("mostNestedLifetimeScope");
        resetSuccessfulActivations();
    }

    std::shared_ptr< IComponentRegistry > ResolveOperation::componentRegistry()
    {
        return mostNestedLifetimeScope_->componentRegistry();
    }

    std::shared_ptr< void > ResolveOperation::resolveComponent(std::shared_ptr< IComponentRegistration > registration)
    {
        return getOrCreateInstance(mostNestedLifetimeScope_, registration);
    }

    std::shared_ptr< void > ResolveOperation::execute(std::shared_ptr< IComponentRegistration > registration)
    {
        std::shared_ptr< void > result = nullptr;

        try
        {
            result = resolveComponent(registration);
        }
        catch (DependencyResolutionException& ex)
        {
            end(ex);
            throw;
        }
        catch (std::exception& ex)
        {
            end(ex);
            throw DependencyResolutionException(ex.what());
        }

        end();
        return result;
    }

    std::shared_ptr< void > ResolveOperation::getOrCreateInstance(std::shared_ptr< ISharingLifetimeScope > currentOperationScope,
                                                                  std::shared_ptr< IComponentRegistration > registration)
    {
        if (currentOperationScope == nullptr)
            throw std::invalid_argument("currentOperationScope");
        if (registration == nullptr)
            throw std::invalid_argument("registration");
        if (ended_)
            throw std::logic_error("Temporary context disposed");

        circularDependencyDetector_.checkForCircularDependency(registration, activationStack_, ++callDepth_);

        auto activation = std::make_shared< InstanceLookup >(registration, shared_from_this(), currentOperationScope);

        activationStack_.push_back(activation);

        auto instance = activation->execute();
        successfulActivations_.push_back(activation);

        activationStack_.pop_back();

        if (activationStack_.size() == 0)
            completeActivations();

        --callDepth_;

        return instance;
    }

    void ResolveOperation::completeActivations()
    {
        auto completedActivations = successfulActivations_;
        resetSuccessfulActivations();

        BOOST_FOREACH(auto activation, completedActivations)
            activation->complete();
    }

    void ResolveOperation::resetSuccessfulActivations()
    {
        successfulActivations_.clear();
    }

    void ResolveOperation::end(std::exception& exception)
    {
        if (!ended_)
        {
            ended_ = true;
        }
    }

    void ResolveOperation::end()
    {
        if (!ended_)
        {
            ended_ = true;
        }
    }

} // namespace Hypodermic