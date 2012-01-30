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
                return new T(c.resolve< Arg1 >());
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
                return new T(c.resolve< Arg1, Arg2 >());
            };
        }
    };

    //template <class T, class Arguments = boost::mpl::vector<> >
    //struct AutowiredConstructor
    //{
    //    static_assert(std::is_class< T >::value, "AutowiredConstructor< T > requires T to be a class");

    //    typedef T ConstructibleType;
    //    typedef AutowiredConstructor< T, Arguments > Signature;

    //    template <class ServiceT>
    //    struct Inject
    //    {
    //        typedef typename AutowiredConstructor
    //                <
    //                    T,
    //                    typename boost::mpl::push_back< Arguments, ServiceT >::type
    //                >::Signature Signature;
    //    };


    //    typedef std::function< T*(IComponentContext&) > Delegate;
    //    static Delegate createDelegate()
    //    {
    //        return createDelegate(boost::mpl::int_< boost::mpl::size< Arguments >::value >());
    //    }

    //private:
    //    static Delegate createDelegate(boost::mpl::int_< 0 > dummy)
    //    {
    //        return [](IComponentContext&) -> T* { return new T(); };
    //    }

    //    static Delegate createDelegate(boost::mpl::int_< 1 > dummy)
    //    {
    //        return [](IComponentContext& c) -> T*
    //        {
    //            return new T(c.resolve< typename boost::mpl::at_c< Arguments, 0 >::type >());
    //        };
    //    }

    //    static Delegate createDelegate(boost::mpl::int_< 2 > dummy)
    //    {
    //        return [](IComponentContext& c) -> T*
    //        {
    //            return new T(c.resolve< typename boost::mpl::at_c< Arguments, 0 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 1 >::type >());
    //        };
    //    }

    //    static Delegate createDelegate(boost::mpl::int_< 3 > dummy)
    //    {
    //        return [](IComponentContext& c) -> T*
    //        {
    //            return new T(c.resolve< typename boost::mpl::at_c< Arguments, 0 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 1 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 2 >::type >());
    //        };
    //    }

    //    static Delegate createDelegate(boost::mpl::int_< 4 > dummy)
    //    {
    //        return [](IComponentContext& c) -> T*
    //        {
    //            return new T(c.resolve< typename boost::mpl::at_c< Arguments, 0 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 1 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 2 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 3 >::type >());
    //        };
    //    }

    //    static Delegate createDelegate(boost::mpl::int_< 5 > dummy)
    //    {
    //        return [](IComponentContext& c) -> T*
    //        {
    //            return new T(c.resolve< typename boost::mpl::at_c< Arguments, 0 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 1 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 2 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 3 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 4 >::type >());
    //        };
    //    }

    //    static Delegate createDelegate(boost::mpl::int_< 6 > dummy)
    //    {
    //        return [](IComponentContext& c) -> T*
    //        {
    //            return new T(c.resolve< typename boost::mpl::at_c< Arguments, 0 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 1 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 2 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 3 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 4 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 5 >::type >());
    //        };
    //    }

    //    static Delegate createDelegate(boost::mpl::int_< 7 > dummy)
    //    {
    //        return [](IComponentContext& c) -> T*
    //        {
    //            return new T(c.resolve< typename boost::mpl::at_c< Arguments, 0 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 1 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 2 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 3 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 4 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 5 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 6 >::type >());
    //        };
    //    }

    //    static Delegate createDelegate(boost::mpl::int_< 8 > dummy)
    //    {
    //        return [](IComponentContext& c) -> T*
    //        {
    //            return new T(c.resolve< typename boost::mpl::at_c< Arguments, 0 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 1 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 2 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 3 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 4 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 5 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 6 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 7 >::type >());
    //        };
    //    }

    //    static Delegate createDelegate(boost::mpl::int_< 9 > dummy)
    //    {
    //        return [](IComponentContext& c) -> T*
    //        {
    //            return new T(c.resolve< typename boost::mpl::at_c< Arguments, 0 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 1 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 2 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 3 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 4 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 5 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 6 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 7 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 8 >::type >());
    //        };
    //    }

    //    static Delegate createDelegate(boost::mpl::int_< 10 > dummy)
    //    {
    //        return [](IComponentContext& c) -> T*
    //        {
    //            return new T(c.resolve< typename boost::mpl::at_c< Arguments, 0 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 1 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 2 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 3 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 4 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 5 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 6 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 7 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 8 >::type >(),
    //                         c.resolve< typename boost::mpl::at_c< Arguments, 9 >::type >());
    //        };
    //    }
    //};

} // namespace Hypodermic


#endif /* !HYPODERMIC_AUTOWIRED_CONSTRUCTOR_H_ */