#pragma once

#include <memory>


namespace Hypodermic
{

    class Behavior
    {
    private:
        struct BehaviorPrivateLock {};

    public:
        static Behavior& instance()
        {
            static auto instance(std::make_shared< Behavior >(BehaviorPrivateLock()));
            return *instance;
        }

        static bool isRuntimeRegistrationEnabled()
        {
            return instance().m_isRuntimeRegistrationEnabled;
        }

        static void configureRuntimeRegistration(bool isEnabled)
        {
            instance().m_isRuntimeRegistrationEnabled = isEnabled;
        }

        explicit Behavior(const BehaviorPrivateLock&)
            : m_isRuntimeRegistrationEnabled(true)
        {
        }

    private:
        bool m_isRuntimeRegistrationEnabled;
    };

} // namespace Hypodermic