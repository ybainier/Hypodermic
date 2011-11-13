#ifndef		HYPODERMIC_FUNC_H_
# define	HYPODERMIC_FUNC_H_

# include <boost/function.hpp>


namespace Hypodermic
{

	template <class T, class TResult>
	struct Func
	{
		Func(::boost::function< TResult(T) > func)
			: func_(func)
		{
		}

		TResult operator()(T arg) const
		{
			return func_(arg);
		}

	private:
		::boost::function< TResult(T) > func_;
	};


	template <class TResult>
	struct Func< void, TResult >
	{
		Func(::boost::function< TResult() > func)
			: func_(func)
		{
		}

		TResult operator()() const
		{
			return func_();
		}

	private:
		::boost::function< TResult() > func_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_FUNC_H_ */