#ifndef		HYPODERMIC_HELPERS_H_
# define	HYPODERMIC_HELPERS_H_


# ifndef		HYPODERMIC_NO_HELPERS

#  include <type_traits>
#  include <Hypodermic/IComponentContext.h>

#  define		CREATE(Expr)            [](Hypodermic::IComponentContext* c) -> decltype(Expr) { return (Expr); }

#  define		INJECT(Dependency)      (c->resolve< Dependency >())

#  define		INJECT_ALL(Dependency)  (c->resolveAll< Dependency >())

# endif /* !HYPODERMIC_NO_HELPERS */


#endif /* !HYPODERMIC_HELPERS_H_ */