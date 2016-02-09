Hypodermic
==========

Hypodermic is an IoC container for C++. It provides dependency injection to your existing design.


### Ackowledgment

Hypodermic was started with the will to mimic the famous .NET Autofac http://autofac.org/. Although its design evolved, both its behavior and its expressiveness tend to remain the same.

# Register components

Components are _registered_ in a `ContainerBuilder`.
``` cpp
ContainerBuilder builder;

builder.registerInstance(std::make_shared< BusConfiguration >());

builder.registerType< MessageDispatcher >()
       .as< IMessageDispatcher >();

builder.registerType< MessageSerializer >();

builder.registerType< Transport >()
       .usingConstructor< Transport(BusConfiguration*, IMessageSerializer*) >()
       .with< IMessageSerializer, MessageSerializer >()
       .with
       .as< ITransport >();
       
builder.registerType< Bus >()
       .as< IBus >()
       .singleInstance();
       
auto container = builder.build();
```

# Express dependencies

It might be magic to some of you; at last, it cannot scan types by itself and deduce how to build them if you don't provide some information. This is C++ we're using, there is no reflection — yet.
``` cpp
class Service
{
public:
    typedef AutowiredConstructor< Service(INotificationSender*) > AutowiredSignature;

    Service(std::shared_ptr< INotificationSender > notificationSender)
        : m_notificationSender(notificationSender)
    { }
  
private:
    std::shared_ptr< INotificationSender > m_notificationSender;
};
```
With the above _AutowiredSignature typedef_:
``` cpp
ContainerBuilder builder;

builder.registerType< Service >();
```
Without:
``` cpp
ContainerBuilder builder;

builder.registerType< Service >()
       .usingConstructor< Service(INotificationSender*) >();
```

# Resolve types

![Sample class hierarchy](../master/resources/home_page_simple_diagram.png?raw=true "Sample class hierarchy")

In your application, you may write code like this:
``` cpp
ContainerBuilder builder;

builder.registerType< Service >();
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
``` cpp
// You will intercept every messages but at no 0 cost. Warn might be a little less aggressive.
Logger::configureLogLevel(LogLevels::Debug);

Logger::configureSink(std::make_shared< MyVeryOwnLog4cxxLoggerSink >());
```
