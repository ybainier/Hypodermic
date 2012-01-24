#ifndef     HYPODERMIC_IINITIALIZABLE_H_
# define    HYPODERMIC_IINITIALIZABLE_H_


namespace Hypodermic
{

    class IInitializable
    {
    public:
        virtual void initialize() = 0;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_IINITIALIZABLE_H_ */