#define BOOST_TEST_MODULE Hypodermic

#include <boost/test/results_reporter.hpp>
#include <boost/test/unit_test.hpp>

#include <boost/config.hpp>

#if defined(BOOST_MSVC)
# pragma warning (disable: 4231) // nonstandard extension used : 'extern' before template explicit instantiation
#endif

#include "Hypodermic/Logger.h"


struct GlobalFixture
{
    GlobalFixture()
    {
        boost::unit_test::results_reporter::set_level(boost::unit_test::SHORT_REPORT);

        Hypodermic::Logger::configureLogLevel(Hypodermic::LogLevels::Warn);
    }
};


BOOST_GLOBAL_FIXTURE(GlobalFixture);


//
// Examples:

// Types used:

//struct IFoo
//{
//    virtual ~IFoo() {}
//};
//
//struct Foo : IFoo
//{
//    Foo()
//    {
//    }
//};
//
//struct IBar
//{
//    virtual ~IBar() {}
//};
//
//struct Bar : IBar
//{
//    Bar()
//    {
//    }
//};
//
//struct FooBar : IFoo, IBar
//{
//    FooBar()
//    {
//    }
//};
//
//struct Baz
//{
//    explicit Baz(const std::shared_ptr< IBar >& bar)
//        : m_bar(bar)
//    {
//        if (m_bar == nullptr)
//            throw std::exception("m_bar cannot be null");
//    }
//
//private:
//    std::shared_ptr< IBar > m_bar;
//};
//
//struct Bazoo
//{
//    typedef Hypodermic::AutowiredConstructor< Bazoo(IFoo*) > AutowiredSignature;
//
//    explicit Bazoo(const std::shared_ptr< IFoo >& foo)
//        : m_foo(foo)
//    {
//        if (m_foo == nullptr)
//            throw std::exception("m_foo cannot be null");
//    }
//
//private:
//    std::shared_ptr< IFoo > m_foo;
//};
//
//struct DepB;
//
//struct IDepA
//{
//    virtual ~IDepA() {}
//};
//
//struct DepA : IDepA
//{
//    typedef Hypodermic::AutowiredConstructor< DepA(DepB*) > AutowiredSignature;
//
//    explicit DepA(const std::shared_ptr< DepB >& depB)
//    {
//        if (depB == nullptr)
//            throw std::exception("depB cannot be null");
//    }
//};
//
//struct DepC;
//
//struct DepB
//{
//    typedef Hypodermic::AutowiredConstructor< DepB(DepC*) > AutowiredSignature;
//
//    explicit DepB(const std::shared_ptr< DepC >& depA)
//    {
//        if (depA == nullptr)
//            throw std::exception("depA cannot be null");
//    }
//};
//
//struct DepC
//{
//    typedef Hypodermic::AutowiredConstructor< DepC(IDepA*) > AutowiredSignature;
//
//    explicit DepC(const std::shared_ptr< IDepA >& depA)
//    {
//        if (depA == nullptr)
//            throw std::exception("depA cannot be null");
//    }
//};
//
//
//struct CustomBar : IBar
//{
//    typedef Hypodermic::AutowiredConstructor< CustomBar(IFoo*) > AutowiredSignature;
//
//    explicit CustomBar(const std::shared_ptr< IFoo >& foo)
//        : foo(foo)
//    {
//        if (foo == nullptr)
//            throw std::exception("foo cannot be null");
//    }
//
//    std::shared_ptr< IFoo > foo;
//};
//
//
//struct ManyBar
//{
//    typedef Hypodermic::AutowiredConstructor< ManyBar(std::vector< IBar* >) > AutowiredSignature;
//
//    explicit ManyBar(const std::vector< std::shared_ptr< IBar > >& bars)
//        : bars(bars)
//    {
//    }
//
//    std::vector< std::shared_ptr< IBar > > bars;
//};
//
//
//struct BarFactory
//{
//    typedef Hypodermic::AutowiredConstructor< BarFactory(std::function< IBar*() >) > AutowiredSignature;
//
//    explicit BarFactory(const std::function< std::shared_ptr< IBar >() >& createBarFunc)
//        : createBarFunc(createBarFunc)
//    {
//    }
//
//    std::function< std::shared_ptr< IBar >() > createBarFunc;
//};



// Configuration / Resolution:


//{
//    Hypodermic::ContainerBuilder builder;
//
//    //builder.registerInstance(fooBar).as< IFoo >().as< IBar >();
//
//    //builder.registerType< Foo >().as< IFoo >().asSelf().singleInstance();
//    builder.registerType< Bar >().as< IBar >();
//
//    ////builder.registerType< Baz >();
//    //builder.registerType< Baz >().usingConstructor< Baz(IBar*) >().asSelf().singleInstance();
//    //builder.registerType< Baz >().usingConstructor< Baz(Bar*) >().asSelf().singleInstance();
//    //builder.registerType< Baz >().usingConstructor< Baz(IBar*) >().asSelf().with< IBar >(bar).singleInstance();
//    //builder.registerType< Baz >().usingConstructor< Baz(IBar*) >().asSelf().with< IBar, Bar >().singleInstance();
//    builder.registerType< Baz >().usingConstructor< Baz(IBar*) >().asSelf().with< IBar >([](Hypodermic::Container& c) { return c.resolve< IBar >(); });
//    builder.registerType< Baz >().usingConstructor< Baz(IBar*) >().with< IBar >(fooBar);
//
//    builder.registerType< FooBar >().as< IFoo >().as< IBar >().singleInstance();
//
//    //builder.registerType< Bazoo >().usingConstructor< Bazoo(IFoo*) >(); // automatically autowired => usingConstructor no longer available
//    builder.registerType< Bazoo >();
//
//    builder.registerType< CustomBar >().with< IFoo, FooBar >().as< IBar >();
//    builder.registerType< ManyBar >();
//
//    builder.registerType< DepA >().as< IDepA >().asSelf();
//    builder.registerType< DepB >();
//    builder.registerType< DepC >();
//
//    builder.registerType< BarFactory >();
//
//    container = builder.build();
//}
//
//auto manyBar = container->resolve< ManyBar >();
//
//auto barFactory = container->resolve< BarFactory >();
//auto createdBar = barFactory->createBarFunc();
//
//auto bazoo = container->resolve< Bazoo >();
//
//{
//    Hypodermic::ContainerBuilder builder;
//    builder.registerType< FooBar >().as< IFoo >().as< IBar >().singleInstance();
//
//    auto nestedContainer = builder.buildNestedContainerFrom(*container);
//
//    bazoo = nestedContainer->resolve< Bazoo >();
//    foo = nestedContainer->resolve< IFoo >();
//}
//
//bazoo = container->resolve< Bazoo >();
//
////auto cfoo = container->resolve< IFoo >();
////auto cfoo2 = container->resolve< IFoo >();
//
////auto cbar = container->resolve< IBar >();
////auto cbar2 = container->resolve< IBar >();
//
//auto&& bars = container->resolveAll< IBar >();
//
//auto baz = container->resolve< Baz >();
////auto baz2 = container->resolve< Baz >();
////auto bazoo = container->resolve< Bazoo >();
////auto bazoo2 = container->resolve< Bazoo >();
//
//auto depA = container->resolve< DepA >();
//auto depB = container->resolve< DepB >();
