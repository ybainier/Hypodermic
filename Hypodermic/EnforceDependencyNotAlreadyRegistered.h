#pragma once


namespace Hypodermic
{
namespace Extensions
{

	template <class TDescriptorInfo, class TDependency>
	struct EnforceDependencyNotAlreadyRegistered
	{
		template <class T>
		struct Act
		{
			static_assert(!TDescriptorInfo::template IsDependencyRegistered< TDependency >::value, "TDependency is already registered for instance T");

			typedef void Type;
		};

		static typename Act< typename TDescriptorInfo::InstanceType >::Type act() {}
	};

} // namespace Extensions
} // namespace Hypodermic