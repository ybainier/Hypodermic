#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "Benchmark.h"
#include "TestingTypes.h"


using namespace Hypodermic;

#if defined(_DEBUG)
# define BENCHMARK_ITERATION_COUNT 10000
# define BENCHMARK_WARM_UP_CYCLE_COUNT 1000
#else
# define BENCHMARK_ITERATION_COUNT 100000
# define BENCHMARK_WARM_UP_CYCLE_COUNT 10000
#endif


BOOST_AUTO_TEST_SUITE(PerformanceTests)

BOOST_AUTO_TEST_CASE(simple_resolve_benchmark)
{
    {
        std::vector< std::shared_ptr< Testing::DefaultConstructible1 > > instances((BENCHMARK_ITERATION_COUNT + BENCHMARK_WARM_UP_CYCLE_COUNT) * 2);
        std::cout << Testing::Utils::Benchmark::measure
        (
            "std::make_shared with default constructible",
            BENCHMARK_ITERATION_COUNT,
            [&]()
            {
                instances.push_back(std::make_shared< Testing::DefaultConstructible1 >());
            },
            BENCHMARK_WARM_UP_CYCLE_COUNT
        ) << std::endl;
    }

    {
        ContainerBuilder builder;
        auto container = builder.build();

        std::vector< std::shared_ptr< Testing::DefaultConstructible1 > > instances((BENCHMARK_ITERATION_COUNT + BENCHMARK_WARM_UP_CYCLE_COUNT) * 2);
        std::cout << Testing::Utils::Benchmark::measure
        (
            "resolve not registered transient",
            BENCHMARK_ITERATION_COUNT,
            [&]()
            {
                instances.push_back(container->resolve< Testing::DefaultConstructible1 >());
            },
            BENCHMARK_WARM_UP_CYCLE_COUNT
        ) << std::endl;
    }

    {
        ContainerBuilder builder;
        builder.registerType< Testing::DefaultConstructible1 >();

        auto container = builder.build();

        std::vector< std::shared_ptr< Testing::DefaultConstructible1 > > instances((BENCHMARK_ITERATION_COUNT + BENCHMARK_WARM_UP_CYCLE_COUNT) * 2);
        std::cout << Testing::Utils::Benchmark::measure
        (
            "resolve registered transient",
            BENCHMARK_ITERATION_COUNT,
            [&]()
            {
                instances.push_back(container->resolve< Testing::DefaultConstructible1 >());
            },
            BENCHMARK_WARM_UP_CYCLE_COUNT
        ) << std::endl;
    }

    {
        ContainerBuilder builder;
        builder.registerType< Testing::DefaultConstructible1 >().singleInstance();

        auto container = builder.build();

        std::vector< std::shared_ptr< Testing::DefaultConstructible1 > > instances((BENCHMARK_ITERATION_COUNT + BENCHMARK_WARM_UP_CYCLE_COUNT) * 2);
        std::cout << Testing::Utils::Benchmark::measure
        (
            "resolve registered single instance",
            BENCHMARK_ITERATION_COUNT,
            [&]()
            {
                instances.push_back(container->resolve< Testing::DefaultConstructible1 >());
            },
            BENCHMARK_WARM_UP_CYCLE_COUNT
        ) << std::endl;
    }

    {
        ContainerBuilder builder;
        builder.registerInstance(std::make_shared< Testing::DefaultConstructible1 >());

        auto container = builder.build();

        std::vector< std::shared_ptr< Testing::DefaultConstructible1 > > instances((BENCHMARK_ITERATION_COUNT + BENCHMARK_WARM_UP_CYCLE_COUNT) * 2);
        std::cout << Testing::Utils::Benchmark::measure
        (
            "resolve registered instance",
            BENCHMARK_ITERATION_COUNT,
            [&]()
            {
                instances.push_back(container->resolve< Testing::DefaultConstructible1 >());
            },
            BENCHMARK_WARM_UP_CYCLE_COUNT
        ) << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(resolve_with_dependencies_benchmark)
{
    {
        std::vector< std::shared_ptr< Testing::MissingConstructor > > instances((BENCHMARK_ITERATION_COUNT + BENCHMARK_WARM_UP_CYCLE_COUNT) * 2);
        std::cout << Testing::Utils::Benchmark::measure
        (
            "std::make_shared with one default constructible dependency",
            BENCHMARK_ITERATION_COUNT,
            [&]()
            {
                instances.push_back(std::make_shared< Testing::MissingConstructor >(std::make_shared< Testing::MissingConstructorDependency >()));
            },
            BENCHMARK_WARM_UP_CYCLE_COUNT
        ) << std::endl;
    }

    {
        ContainerBuilder builder;
        auto container = builder.build();

        std::vector< std::shared_ptr< Testing::MissingConstructor > > instances((BENCHMARK_ITERATION_COUNT + BENCHMARK_WARM_UP_CYCLE_COUNT) * 2);
        std::cout << Testing::Utils::Benchmark::measure
        (
            "resolve not registered transient",
            BENCHMARK_ITERATION_COUNT,
            [&]()
            {
                instances.push_back(container->resolve< Testing::MissingConstructor >());
            },
            BENCHMARK_WARM_UP_CYCLE_COUNT
        ) << std::endl;
    }

    {
        ContainerBuilder builder;
        builder.registerType< Testing::MissingConstructorDependency >();
        builder.registerType< Testing::MissingConstructor >();

        auto container = builder.build();

        std::vector< std::shared_ptr< Testing::MissingConstructor > > instances((BENCHMARK_ITERATION_COUNT + BENCHMARK_WARM_UP_CYCLE_COUNT) * 2);
        std::cout << Testing::Utils::Benchmark::measure
        (
            "resolve registered transient",
            BENCHMARK_ITERATION_COUNT,
            [&]()
            {
                instances.push_back(container->resolve< Testing::MissingConstructor >());
            },
            BENCHMARK_WARM_UP_CYCLE_COUNT
        ) << std::endl;
    }

    {
        ContainerBuilder builder;
        builder.registerType< Testing::MissingConstructorDependency >().singleInstance();
        builder.registerType< Testing::MissingConstructor >();

        auto container = builder.build();

        std::vector< std::shared_ptr< Testing::MissingConstructor > > instances((BENCHMARK_ITERATION_COUNT + BENCHMARK_WARM_UP_CYCLE_COUNT) * 2);
        std::cout << Testing::Utils::Benchmark::measure
        (
            "resolve with registered single instance dependency",
            BENCHMARK_ITERATION_COUNT,
            [&]()
            {
                instances.push_back(container->resolve< Testing::MissingConstructor >());
            },
            BENCHMARK_WARM_UP_CYCLE_COUNT
        ) << std::endl;
    }

    {
        ContainerBuilder builder;
        builder.registerInstance(std::make_shared< Testing::MissingConstructorDependency >());
        builder.registerType< Testing::MissingConstructor >();

        auto container = builder.build();

        std::vector< std::shared_ptr< Testing::MissingConstructor > > instances((BENCHMARK_ITERATION_COUNT + BENCHMARK_WARM_UP_CYCLE_COUNT) * 2);
        std::cout << Testing::Utils::Benchmark::measure
        (
            "resolve with registered instance dependency",
            BENCHMARK_ITERATION_COUNT,
            [&]()
            {
                instances.push_back(container->resolve< Testing::MissingConstructor >());
            },
            BENCHMARK_WARM_UP_CYCLE_COUNT
        ) << std::endl;
    }
}

BOOST_AUTO_TEST_SUITE_END()