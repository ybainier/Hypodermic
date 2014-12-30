-- require https://github.com/d-led/premake-meta-cpp in 'premake' folder
-- generate projects with: premake/premake4(.osx) gmake (or )

include 'premake'

make_solution 'hypodermic'

platforms 'native'

local OS = os.get()

local settings = {
	links = {
		macosx = {}
	},
	libdirs = {
		macosx = {
			'/usr/local/lib'
		}
	},
	includedirs = {
		macosx = {
			'/usr/local/include'
		}
	}
}

includedirs {
	'.',
	settings.includedirs[OS]
}

libdirs {
	settings.libdirs[OS]
}

--------------------------------
make_static_lib( 'hypodermic', {
		'Hypodermic/**.h',
		'Hypodermic/**.cpp',
	}
)

use_standard('c++11')

---------------------------------------
make_console_app( 'hypodermic.tests', {
		'Hypodermic.Tests/**.cpp'
	}
)

use_standard('c++11')

links {
	'hypodermic',
	settings.links[OS]
}

run_target_after_build()
