#pragma once

#ifdef _MSC_VER

# define HYPODERMIC_PRAGMA_PUSH __pragma(warning(push))
# define HYPODERMIC_PRAGMA_POP  __pragma(warning(pop))

# define HYPODERMIC_IGNORE_CONDITIONAL_EXPRESSION_IS_CONSTANT   __pragma(warning(disable : 4127))

#else // ifdef _MSC_VER

# define HYPODERMIC_PRAGMA_PUSH
# define HYPODERMIC_PRAGMA_POP 

# define HYPODERMIC_IGNORE_CONDITIONAL_EXPRESSION_IS_CONSTANT

#endif // ifdef _MSC_VER
