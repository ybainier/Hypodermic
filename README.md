Hypodermic
==========

Hypodermic is an IoC container for C++. It provides dependency injection to your existing design.


### Ackowledgment

Hypodermic is mostly inspired from the famous .NET Autofac http://autofac.org/. A thousand thanks to its creators.

# Adding components

Components are _registered_ in a `ContainerBuilder`.
```cpp
ContainerBuilder builder;
```

## Registering concrete types

Considering these classes:

![Sample class hierarchy](../blob/master/resources/home_page_simple_diagram.png?raw=true "Sample class hierarchy")


You can invoke `ContainerBuilder::registerType()` with a type as a template parameter.
```cpp
builder.registerType< Driver >();
```
Further readings: [Registering concrete types basics](https://github.com/ybainier/Hypodermic/wiki/Registering-concrete-types#basics)

## Expressing dependencies

```cpp
class Car : public ICar
{
public:
    Car(std::shared_ptr< IDriver > driver);
}
```
Note that `Car` is needing to be injected a `IDriver`. We need to tell the container that `Driver` is actually a `IDriver` by setting up this concrete type as an interface:
```cpp
builder.registerType< Driver >()->as< IDriver >();
```
Further readings: [Registering concrete types as interfaces](https://github.com/ybainier/Hypodermic/wiki/Registering-concrete-types#intermediate)

Now, we can setup `Car`:
```cpp
builder.registerType< Car >(CREATE(new Car(INJECT(IDriver))))->as< ICar >();
```


# Resolving types

Calling `ContainerBuilder::build()` creates a container:
```cpp
auto container = builder.build();
```
Now it is time to get our instance:
```cpp
auto car = container->resolve< ICar >();
```

That's all folks!
