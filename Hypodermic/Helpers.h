#ifndef		HELPERS_H_
# define	HELPERS_H_


# ifndef		HYPODERMIC_NO_HELPERS

#   define		CREATE(TResult, Expr)					\
				Func< IComponentContext*, TResult >(	\
					[](IComponentContext* c) -> TResult	\
					{									\
						return (Expr);					\
					})

#   define		INJECT(Dependency)	(c->resolve< Dependency >())

# endif /* !HYPODERMIC_NO_HELPERS */


#endif /* !HELPERS_H_ */