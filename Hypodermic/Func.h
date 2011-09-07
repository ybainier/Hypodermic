#ifndef		FUNC_H_
# define	FUNC_H_

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

} // namespace Hypodermic


#endif /* !FUNC_H_ */