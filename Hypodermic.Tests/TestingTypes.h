#pragma once


//using namespace Hypodermic;


namespace Testing
{

    class DefaultConstructibleBase
    {
    public:
        virtual ~DefaultConstructibleBase() {}
    };

    class DefaultConstructible1 : public DefaultConstructibleBase
    {
    };

    class DefaultConstructible2 : public DefaultConstructibleBase
    {
    };

} // namespace Testing