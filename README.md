Hypodermic
==========

Hypodermic is an IoC container for C++. It provides dependency injection to your existing design.

[![Build status](https://ci.appveyor.com/api/projects/status/20csygnj7ww04tyw?svg=true&pendingText=Windows%20build%20pending&failingText=Windows%20build%20failed&passingText=Windows%20build%20passing)](https://ci.appveyor.com/project/ybainier/hypodermic)
[![Build Status](https://travis-ci.org/ybainier/Hypodermic.svg?branch=master)](https://travis-ci.org/ybainier/Hypodermic)

### Ackowledgment

Hypodermic was started with the will to mimic the famous .NET Autofac http://autofac.org/. Although its design evolved, both its behavior and its expressiveness tend to remain the same.

# Register components

Configure the container by registering types, instances, and instance factories.
```cpp
ContainerBuilder builder;

builder.registerInstance(std::make_shared< BusConfiguration >());

builder.registerInstanceFactory([](Container&)
{
    return std::make_shared< ThreadPool >(std::thread::hardware_concurrency());
});

builder.registerType< MessageDispatcher >()
       .as< IMessageDispatcher >();

builder.registerType< MessageSerializer >();

builder.registerType< Transport >()
       .with< IMessageSerializer, MessageSerializer >()
       .as< ITransport >();
       
builder.registerType< Bus >()
       .as< IBus >()
       .singleInstance();
       
auto container = builder.build();
```

# Express dependencies

Hypodermic automatically inject your constructor dependencies.
```cpp
class Service
{
public:
    explicit Service(const std::shared_ptr< INotificationSender >& notificationSender)
        : m_notificationSender(notificationSender)
    { }
  
private:
    std::shared_ptr< INotificationSender > m_notificationSender;
};
```

# Resolve types

![Sample class hierarchy](../master/resources/home_page_simple_diagram.png?raw=true "Sample class hierarchy")

In your application, you may write code like this:
```cpp
ContainerBuilder builder;

builder.registerType< SmsNotificationSender >().as< INotificationSender >();

auto container = builder.build();

auto service = container->resolve< Service >();
service->start();
```
Now, in a unit test:
```cpp
// Arrange
auto mock = std::make_shared< NotificationSenderMock >();
Service service(mock);

// Act & Assert
EXPECT_CALL(*mock, send(_)).Times(1);

BOOST_CHECK_NO_THROW(service.start());
```

# Wiring the internal logging mechanism

You may want to trace Hypodermic's activity by wiring your own logging mechanism to it. You have to implement `ILoggerSink` and configure the singleton of `Logger` like so:
```cpp
// You will intercept every messages but at no 0 cost. Warn might be a little less aggressive.
Logger::configureLogLevel(LogLevels::Debug);

Logger::configureSink(std::make_shared< MyVeryOwnLog4cxxLoggerSink >());
```
