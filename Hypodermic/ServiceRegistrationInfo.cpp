#include "ServiceRegistrationInfo.h"

#include <stdexcept>

#include "IRegistrationSource.h"
#include "NullptrWorkaround.h"
#include "Service.h"


namespace Hypodermic
{

    ServiceRegistrationInfo::ServiceRegistrationInfo(std::shared_ptr< Service > service)
        : service_(service)
        , isInitialized_(false)
    {
        if (service == nullptr)
            throw std::invalid_argument("service");
    }

    std::vector< std::shared_ptr< IComponentRegistration > >& ServiceRegistrationInfo::implementations()
    {
        return implementations_;
    }

    void ServiceRegistrationInfo::addImplementation(std::shared_ptr< IComponentRegistration > registration)
    {
        implementations_.push_back(registration);
    }

    std::shared_ptr< IComponentRegistration > ServiceRegistrationInfo::getRegistration()
    {
        if (implementations_.size() > 0)
            return implementations_[implementations_.size() - 1];
        return std::shared_ptr< IComponentRegistration >();
    }

    bool ServiceRegistrationInfo::isRegistered()
    {
        return implementations_.size() > 0 && implementations_[0] != nullptr;
    }

    void ServiceRegistrationInfo::include(std::shared_ptr< IRegistrationSource > source)
    {
        if (isInitialized())
        {
            std::deque< std::shared_ptr< IRegistrationSource > > sources;
            sources.push_back(source);

            beginInitialization(sources);
        }
        else if (isInitializing())
            sourcesToQuery_.push_back(source);
    }

    void ServiceRegistrationInfo::completeInitialization()
    {
        isInitialized_ = true;
        sourcesToQuery_.clear();
    }

    bool ServiceRegistrationInfo::isInitialized() const
    {
        return isInitialized_;
    }

    bool ServiceRegistrationInfo::isInitializing() const
    {
        return !isInitialized_ && !sourcesToQuery_.empty();
    }

    void ServiceRegistrationInfo::beginInitialization(const std::deque< std::shared_ptr< IRegistrationSource > >& sources)
    {
        isInitialized_ = false;
        sourcesToQuery_ = sources;
    }

    bool ServiceRegistrationInfo::hasSourcesToQuery() const
    {
        return isInitializing() && !sourcesToQuery_.empty();
    }

    std::shared_ptr< IRegistrationSource > ServiceRegistrationInfo::dequeueNextSource()
    {
        if (!isInitializing())
            throw std::logic_error("Not during initialization");

        auto source = sourcesToQuery_.front();
        sourcesToQuery_.pop_front();

        return source;
    }

    void ServiceRegistrationInfo::skipSource(std::shared_ptr< IRegistrationSource > source)
    {
        if (!isInitializing())
            throw std::logic_error("Not during initialization");

        auto&& tmp = sourcesToQuery_;
        sourcesToQuery_.clear();

        std::copy_if(tmp.begin(), tmp.end(), std::back_inserter(sourcesToQuery_),
            [source](std::shared_ptr< IRegistrationSource > rs) { return rs != source; });
    }

} // namespace Hypodermic