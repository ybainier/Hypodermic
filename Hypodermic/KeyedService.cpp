#include <boost/functional/hash.hpp>
#include "NullptrWorkaround.h"
#include "KeyedService.h"


namespace Hypodermic
{

    KeyedService::KeyedService(const std::string& name, const std::type_info& typeInfo)
        : name_(name)
        , typeInfo_(typeInfo)
    {
    }

    const std::string& KeyedService::name() const
    {
        return name_;
    }

    const std::type_info& KeyedService::typeInfo() const
    {
        return typeInfo_;
    }

    bool KeyedService::operator==(const Service& rhs) const
    {
        const Service* self = this;
        if (self == &rhs)
            return true;

        auto rhsKey = dynamic_cast< const KeyedService* >(&rhs);
        if (rhsKey == nullptr)
            return false;

        return name_ == rhsKey->name_ && typeInfo_ == rhsKey->typeInfo_;
    }

    std::size_t KeyedService::hashValue() const
    {
        return boost::hash< std::string >()(name_) ^ boost::hash< const char* >()(typeInfo_.name());
    }

} // namespace Hypodermic