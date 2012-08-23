#pragma once

/*****************************************************
Selective library loader for Hypodermic library.

Library path:
Before including this file, you may specify the library root directory as following (without trailing backslashes)
#define HYPODERMIC_LIBROOT       "..\\..\\libs\\Hypodermic"
Paths can be either absolute or relative to the directory that contains the project file.
The current file will then resolve the correct library name and subdirectories according to your
project configuration (debug vs release) and platform (win32 vs win64).

If you choose to not define this macro, you must specify in your project settings the path to the directory
that contains the .lib files.

******************************************************/

#include <boost/config.hpp>


#if defined(BOOST_MSVC)

//////////////////////////////////////////////////////////////////////////
// configure
#if defined(_DEBUG)
# define HYPODERMIC_LIBNAME "hypodermic_d"
#else
# define HYPODERMIC_LIBNAME "hypodermic"
#endif

#if _MSC_VER == 1600
# define HYPODERMIC_LIBFILE HYPODERMIC_LIBNAME "-vc100.lib"
#elif _MSC_VER == 1700
# define HYPODERMIC_LIBFILE HYPODERMIC_LIBNAME "-vc110.lib"
#else
# error "Unsupported platform toolset for auto-linking"
#endif

#ifdef HYPODERMIC_LIBROOT

# if _WIN64
#  define HYPODERMIC_PLATFORM "x64\\"
# else
#  define HYPODERMIC_PLATFORM ""
# endif

# define HYPODERMIC_LIBPATH  HYPODERMIC_LIBROOT "\\lib\\" HYPODERMIC_PLATFORM HYPODERMIC_LIBFILE

#else
# define HYPODERMIC_LIBPATH  HYPODERMIC_LIBFILE
#endif

//////////////////////////////////////////////////////////////////////////
// set libraries
#pragma message( "Hypodermic library " HYPODERMIC_LIBPATH " loaded" )
#pragma comment( lib, HYPODERMIC_LIBPATH )

//////////////////////////////////////////////////////////////////////////
// cleanup symbols
#undef HYPODERMIC_PLATFORM
#undef HYPODERMIC_LIBPATH
#undef HYPODERMIC_LIBNAME
#undef HYPODERMIC_LIBFILE

#endif /* defined(BOOST_MSVC) */