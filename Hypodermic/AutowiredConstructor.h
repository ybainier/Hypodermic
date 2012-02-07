#ifndef     HYPODERMIC_AUTOWIRED_CONSTRUCTOR_H_
# define    HYPODERMIC_AUTOWIRED_CONSTRUCTOR_H_

# include <functional>
# include <type_traits>

# include <Hypodermic/IComponentContext.h>


namespace Hypodermic
{
    template <bool> struct BoolType : std::true_type {};

    template <> struct BoolType< false > : std::false_type {};

    /*
     * T should be a pointer type or an std::vector of pointer type.
     * Examples:
     *   - AutowiredConstructor< Foo(IBar*) >
     *   - AutowiredConstructor< Bar(std::vector< IBaz* >) >
     *   - AutowiredConstructor< Baz() >
     */
    template <class T> struct ArgResolver
    {
        typedef std::false_type IsResolvable;
    };

    template <class T>
    struct ArgResolver< T* >
    {
        typedef std::true_type IsResolvable;

        typedef typename std::remove_pointer< T >::type Type;

        static std::shared_ptr< Type > resolve(IComponentContext& c)
        {
            return c.resolve< Type >();
        }
    };

    template <class T>
    struct ArgResolver< std::vector< T* > >
    {
        typedef std::true_type IsResolvable;

        typedef typename std::remove_pointer< T >::type Type;

        static std::vector< std::shared_ptr< T > > resolve(IComponentContext& c)
        {
            return c.resolveAll< Type >();
        }
    };

    /*
     * Examples:
     *   - AutowiredConstructor< Foo(IBar*) >
     *   - AutowiredConstructor< Bar(std::vector< IBaz* >) >
     *   - AutowiredConstructor< Baz() >
     */
    template <class Signature> struct AutowiredConstructor
    {
        typedef std::false_type IsSignatureRecognized;
    };


    template <class T>
    struct AutowiredConstructor< T() >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef std::true_type IsSignatureRecognized;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext&) -> T*
            {
                return new T();
            };
        }
    };

    template <class T, class Arg1>
    struct AutowiredConstructor< T(Arg1) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef BoolType< ArgResolver< Arg1 >::IsResolvable::value > IsSignatureRecognized;

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

        typedef BoolType< ArgResolver< Arg1 >::IsResolvable::value
                       && ArgResolver< Arg2 >::IsResolvable::value > IsSignatureRecognized;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(ArgResolver< Arg1 >::resolve(c), ArgResolver< Arg2 >::resolve(c));
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef BoolType< ArgResolver< Arg1 >::IsResolvable::value
                       && ArgResolver< Arg2 >::IsResolvable::value
                       && ArgResolver< Arg3 >::IsResolvable::value > IsSignatureRecognized;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(ArgResolver< Arg1 >::resolve(c), ArgResolver< Arg2 >::resolve(c),
                             ArgResolver< Arg3 >::resolve(c));
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3, class Arg4>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3, Arg4) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef BoolType< ArgResolver< Arg1 >::IsResolvable::value
                       && ArgResolver< Arg2 >::IsResolvable::value
                       && ArgResolver< Arg3 >::IsResolvable::value
                       && ArgResolver< Arg4 >::IsResolvable::value > IsSignatureRecognized;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(ArgResolver< Arg1 >::resolve(c), ArgResolver< Arg2 >::resolve(c),
                             ArgResolver< Arg3 >::resolve(c), ArgResolver< Arg4 >::resolve(c));
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3, Arg4, Arg5) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef BoolType< ArgResolver< Arg1 >::IsResolvable::value
                       && ArgResolver< Arg2 >::IsResolvable::value
                       && ArgResolver< Arg3 >::IsResolvable::value
                       && ArgResolver< Arg4 >::IsResolvable::value
                       && ArgResolver< Arg5 >::IsResolvable::value > IsSignatureRecognized;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(ArgResolver< Arg1 >::resolve(c), ArgResolver< Arg2 >::resolve(c),
                             ArgResolver< Arg3 >::resolve(c), ArgResolver< Arg4 >::resolve(c),
                             ArgResolver< Arg5 >::resolve(c));
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef BoolType< ArgResolver< Arg1 >::IsResolvable::value
                       && ArgResolver< Arg2 >::IsResolvable::value
                       && ArgResolver< Arg3 >::IsResolvable::value
                       && ArgResolver< Arg4 >::IsResolvable::value
                       && ArgResolver< Arg5 >::IsResolvable::value
                       && ArgResolver< Arg6 >::IsResolvable::value > IsSignatureRecognized;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(ArgResolver< Arg1 >::resolve(c), ArgResolver< Arg2 >::resolve(c),
                             ArgResolver< Arg3 >::resolve(c), ArgResolver< Arg4 >::resolve(c),
                             ArgResolver< Arg5 >::resolve(c), ArgResolver< Arg6 >::resolve(c));
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef BoolType< ArgResolver< Arg1 >::IsResolvable::value
                       && ArgResolver< Arg2 >::IsResolvable::value
                       && ArgResolver< Arg3 >::IsResolvable::value
                       && ArgResolver< Arg4 >::IsResolvable::value
                       && ArgResolver< Arg5 >::IsResolvable::value
                       && ArgResolver< Arg6 >::IsResolvable::value
                       && ArgResolver< Arg7 >::IsResolvable::value > IsSignatureRecognized;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(ArgResolver< Arg1 >::resolve(c), ArgResolver< Arg2 >::resolve(c),
                             ArgResolver< Arg3 >::resolve(c), ArgResolver< Arg4 >::resolve(c),
                             ArgResolver< Arg5 >::resolve(c), ArgResolver< Arg6 >::resolve(c),
                             ArgResolver< Arg7 >::resolve(c));
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef BoolType< ArgResolver< Arg1 >::IsResolvable::value
                       && ArgResolver< Arg2 >::IsResolvable::value
                       && ArgResolver< Arg3 >::IsResolvable::value
                       && ArgResolver< Arg4 >::IsResolvable::value
                       && ArgResolver< Arg5 >::IsResolvable::value
                       && ArgResolver< Arg6 >::IsResolvable::value
                       && ArgResolver< Arg7 >::IsResolvable::value
                       && ArgResolver< Arg8 >::IsResolvable::value > IsSignatureRecognized;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(ArgResolver< Arg1 >::resolve(c), ArgResolver< Arg2 >::resolve(c),
                             ArgResolver< Arg3 >::resolve(c), ArgResolver< Arg4 >::resolve(c),
                             ArgResolver< Arg5 >::resolve(c), ArgResolver< Arg6 >::resolve(c),
                             ArgResolver< Arg7 >::resolve(c), ArgResolver< Arg8 >::resolve(c));
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Arg9>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef BoolType< ArgResolver< Arg1 >::IsResolvable::value
                       && ArgResolver< Arg2 >::IsResolvable::value
                       && ArgResolver< Arg3 >::IsResolvable::value
                       && ArgResolver< Arg4 >::IsResolvable::value
                       && ArgResolver< Arg5 >::IsResolvable::value
                       && ArgResolver< Arg6 >::IsResolvable::value
                       && ArgResolver< Arg7 >::IsResolvable::value
                       && ArgResolver< Arg8 >::IsResolvable::value
                       && ArgResolver< Arg9 >::IsResolvable::value > IsSignatureRecognized;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(ArgResolver< Arg1 >::resolve(c), ArgResolver< Arg2 >::resolve(c),
                             ArgResolver< Arg3 >::resolve(c), ArgResolver< Arg4 >::resolve(c),
                             ArgResolver< Arg5 >::resolve(c), ArgResolver< Arg6 >::resolve(c),
                             ArgResolver< Arg7 >::resolve(c), ArgResolver< Arg8 >::resolve(c),
                             ArgResolver< Arg9 >::resolve(c));
            };
        }
    };

    template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Arg9, class Arg10>
    struct AutowiredConstructor< T(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) >
    {
        static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

        typedef BoolType< ArgResolver< Arg1 >::IsResolvable::value
                       && ArgResolver< Arg2 >::IsResolvable::value
                       && ArgResolver< Arg3 >::IsResolvable::value
                       && ArgResolver< Arg4 >::IsResolvable::value
                       && ArgResolver< Arg5 >::IsResolvable::value
                       && ArgResolver< Arg6 >::IsResolvable::value
                       && ArgResolver< Arg7 >::IsResolvable::value
                       && ArgResolver< Arg8 >::IsResolvable::value
                       && ArgResolver< Arg9 >::IsResolvable::value
                       && ArgResolver< Arg10 >::IsResolvable::value > IsSignatureRecognized;

        static std::function< T*(IComponentContext&) > createDelegate()
        {
            return [](IComponentContext& c) -> T*
            {
                return new T(ArgResolver< Arg1 >::resolve(c), ArgResolver< Arg2 >::resolve(c),
                             ArgResolver< Arg3 >::resolve(c), ArgResolver< Arg4 >::resolve(c),
                             ArgResolver< Arg5 >::resolve(c), ArgResolver< Arg6 >::resolve(c),
                             ArgResolver< Arg7 >::resolve(c), ArgResolver< Arg8 >::resolve(c),
                             ArgResolver< Arg9 >::resolve(c), ArgResolver< Arg10 >::resolve(c));
            };
        }
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_AUTOWIRED_CONSTRUCTOR_H_ */