#pragma once


namespace Hypodermic
{
namespace Utils
{

    template <class...>
    struct ArgumentPack
    {
        using Type = ArgumentPack;
    };

} // namespace Utils
} // namespace Hypodermic
