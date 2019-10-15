#pragma once

#include <type_traits>


namespace Hypodermic
{
namespace Extensions
{

	template <class TDescriptorInfo, class TBase, class T>
	struct EnforceBaseOf
	{
		static_assert(std::is_base_of< TBase, T >::value && !std::is_same< TBase, T >::value, "TBase should be a base of T");

		static void act() {}
	};

} // namespace Extensions
} // namespace Hypodermic