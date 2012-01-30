#ifndef     HYPODERMIC_AUTOWIRED_CONSTRUCTOR_H_
# define    HYPODERMIC_AUTOWIRED_CONSTRUCTOR_H_

# include <functional>
# include <type_traits>

# include <boost/mpl/at.hpp>
# include <boost/mpl/int.hpp>
# include <boost/mpl/push_back.hpp>
# include <boost/mpl/size.hpp>
# include <boost/mpl/vector.hpp>

# include <Hypodermic/IComponentContext.h>


namespace Hypodermic
{
    template <class T> struct ArgResolver;
    template <class Signature> struct AutowiredConstructor {};


    template <class T>
    struct AutowiredConstructor< T() >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef T ConstructibleType;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext&) -> T* { return new T(); };
        }
    };

    template <class T, class Arg1>
    struct AutowiredConstructor< T(Arg1) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef T ConstructibleType;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(ArgResolver< Arg1 >::resolve(c));
            };
        }
    };

    template <class T, class Arg1, class Arg2>
    struct AutowiredConstructor< T(Arg1, Arg2) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef T ConstructibleType;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(c.resolve< Arg1 >(), c.resolve< Arg2 >());
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef T ConstructibleType;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(c.resolve< Arg1 >(), c.resolve< Arg2 >(), c.resolve< Arg3 >());
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3, class Arg4>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3, Arg4) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef T ConstructibleType;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(c.resolve< Arg1 >(), c.resolve< Arg2 >(), c.resolve< Arg3 >(), c.resolve< Arg4 >());
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3, Arg4, Arg5) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef T ConstructibleType;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(c.resolve< Arg1 >(), c.resolve< Arg2 >(), c.resolve< Arg3 >(), c.resolve< Arg4 >(), c.resolve< Arg5 >());
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef T ConstructibleType;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(c.resolve< Arg1 >(), c.resolve< Arg2 >(), c.resolve< Arg3 >(), c.resolve< Arg4 >(), c.resolve< Arg5 >(),
                             c.resolve< Arg6 >());
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef T ConstructibleType;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(c.resolve< Arg1 >(), c.resolve< Arg2 >(), c.resolve< Arg3 >(), c.resolve< Arg4 >(), c.resolve< Arg5 >(),
                             c.resolve< Arg6 >(), c.resolve< Arg7 >());
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef T ConstructibleType;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(c.resolve< Arg1 >(), c.resolve< Arg2 >(), c.resolve< Arg3 >(), c.resolve< Arg4 >(), c.resolve< Arg5 >(),
                             c.resolve< Arg6 >(), c.resolve< Arg7 >(), c.resolve< Arg8 >());
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Arg9>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef T ConstructibleType;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(c.resolve< Arg1 >(), c.resolve< Arg2 >(), c.resolve< Arg3 >(), c.resolve< Arg4 >(), c.resolve< Arg5 >(),
                             c.resolve< Arg6 >(), c.resolve< Arg7 >(), c.resolve< Arg8 >(), c.resolve< Arg9 >());
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Arg9, class Arg10>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef T ConstructibleType;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(c.resolve< Arg1 >(), c.resolve< Arg2 >(), c.resolve< Arg3 >(), c.resolve< Arg4 >(), c.resolve< Arg5 >(),
                             c.resolve< Arg6 >(), c.resolve< Arg7 >(), c.resolve< Arg8 >(), c.resolve< Arg9 >(), c.resolve< Arg10 >());
            };
        }
    };

    template <class T>
    struct ArgResolver
    {
        static std::shared_ptr< T > resolve(IComponentContext& c)
        {
            return c.resolve< T >();
        }
    };

    template <class T>
    struct ArgResolver< std::vector< T > >
    {
        static std::vector< std::shared_ptr< T > > resolve(IComponentContext& c)
        {
            return c.resolveAll< T >();
        }
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_AUTOWIRED_CONSTRUCTOR_H_ */