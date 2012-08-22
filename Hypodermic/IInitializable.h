#ifndef     HYPODERMIC_IINITIALIZABLE_H_
# define    HYPODERMIC_IINITIALIZABLE_H_


namespace Hypodermic
{

    class IInitializable
    {
    public:
        virtual ~IInitializable() = 0;

        virtual void initialize() = 0;
    };

    inline IInitializable::~IInitializable() {}

} // namespace Hypodermic


#endif /* !HYPODERMIC_IINITIALIZABLE_H_ */