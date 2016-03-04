#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"


namespace Testing
{

    class INotificationSender
    {
    public:
        virtual ~INotificationSender() {}

        virtual void notify(const std::string& message) = 0;
    };

    class SmsNotificationSender : public INotificationSender
    {
    public:
        void notify(const std::string&) override
        {
            // Send notification...
        }
    };

    class NotificationSenderTest : public INotificationSender
    {
    public:
        void notify(const std::string& message) override
        {
            notifications.push_back(message);
        }

        std::vector< std::string > notifications;
    };


    class Service
    {
    public:
        explicit Service(const std::shared_ptr< INotificationSender >& notificationSender)
            : m_notificationSender(notificationSender)
        {
            if (m_notificationSender == nullptr)
                throw std::runtime_error("m_notificationSender cannot be null");
        }

        void start()
        {
            m_notificationSender->notify("Service started");
        }

    private:
        std::shared_ptr< INotificationSender > m_notificationSender;
    };

} // namespace Testing



using namespace Testing;
using namespace Hypodermic;


BOOST_AUTO_TEST_SUITE(GithubTests)

BOOST_AUTO_TEST_CASE(should_resolve_service_with_sms_notification_sender_configured)
{
    // Arrange
    ContainerBuilder builder;

    builder.registerType< SmsNotificationSender >().as< INotificationSender >();

    auto container = builder.build();

    // Act
    std::shared_ptr< Service > service;
    BOOST_REQUIRE_NO_THROW(service = container->resolve< Service >());

    // Assert
    BOOST_REQUIRE(service != nullptr);
}

BOOST_AUTO_TEST_CASE(should_send_one_notification_when_service_is_started)
{
    // Arrange
    ContainerBuilder builder;

    auto notificationSender = std::make_shared< NotificationSenderTest >();
    builder.registerInstance(notificationSender).as< INotificationSender >();

    auto container = builder.build();

    // Act
    auto service = container->resolve< Service >();
    BOOST_REQUIRE_NO_THROW(service->start());

    // Assert
    BOOST_REQUIRE_EQUAL(notificationSender->notifications.size(), 1);
    BOOST_CHECK_EQUAL(notificationSender->notifications[0], "Service started");
}

BOOST_AUTO_TEST_SUITE_END()