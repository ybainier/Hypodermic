#ifndef		HYPODERMIC_ACTION_H_
# define	HYPODERMIC_ACTION_H_

# include <boost/function.hpp>


namespace Hypodermic
{

	template <class Arg1T = void, class Arg2T = void>
	struct Action
	{
		Action(::boost::function< void(Arg1T, Arg2T) > func)
			: func_(func)
		{
		}

		void operator()(Arg1T arg1, Arg2T arg2) const
		{
			func_(arg1, arg2);
		}

	private:
		::boost::function< void(Arg1T, Arg2T) > func_;
	};



	template <class ArgT>
	struct Action< ArgT, void >
	{
		Action(::boost::function< void(ArgT) > func)
			: func_(func)
		{
		}

		void operator()(ArgT arg) const
		{
			func_(arg);
		}

	private:
		::boost::function< void(ArgT) > func_;
	};


	template <>
	struct Action< void, void >
	{
		Action(::boost::function< void() > func)
			: func_(func)
		{
		}

		void operator()() const
		{
			func_();
		}

	private:
		::boost::function< void() > func_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_ACTION_H_ */