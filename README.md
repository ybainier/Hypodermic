Hypodermic <img align="center" src="../master/resources/syringe_by_Andy_Selimov_from_thenounproject.png?raw=true" width="56">
==========

[![MIT license](http://img.shields.io/badge/license-MIT-brightgreen.svg)](http://opensource.org/licenses/MIT)
[![Build status](https://img.shields.io/appveyor/ci/ybainier/Hypodermic/master.svg?label=windows)](https://ci.appveyor.com/project/ybainier/hypodermic)
[![Build Status](https://img.shields.io/travis/ybainier/Hypodermic/master.svg?label=linux/osx)](https://travis-ci.org/ybainier/Hypodermic)

Hypodermic is a non-intrusive header only [IoC container](http://martinfowler.com/articles/injection.html) for C++. It provides dependency injection to your existing design by _managing the creation of your components and their dependencies_ in the right order, sparing you the trouble of writing and maintaining boiler plate code.

Used in production environments since 2012.


## First steps

Configure the container by registering your components
```cpp
ContainerBuilder builder;

builder.registerType< MessageDispatcher >();

auto container = builder.build();
```

... then ask the container to give you an instance of that type
```cpp
auto dispatcher = container->resolve< MessageDispatcher >();
```

Want to discover what happened here? Learn to use more features through the [beginner's guide](https://github.com/ybainier/Hypodermic/wiki/Getting-started) on the wiki.


## Benchmarks

Some comparative benchmarks are available on the wiki, [check them out](https://github.com/ybainier/Hypodermic/wiki/Benchmarks).


## Ackowledgment

Hypodermic was started with the will to mimic the famous .NET Autofac http://autofac.org/. Although its design evolved, both its behavior and its expressiveness tend to remain the same.

Constructor deduction has been made possible thanks to the work of [Krzysztof Jusiak](http://krzysztof.jusiak.net/) in http://boost-experimental.github.io/di/.
