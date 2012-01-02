#ifndef     HYPODERMIC_ITYPE_CASTER_H_
# define    HYPODERMIC_ITYPE_CASTER_H_

# include <typeinfo>


namespace Hypodermic
{

    class ITypeCaster
    {
    public:
        virtual const std::type_info& typeInfo() const = 0;

        virtual void* cast(void* concreteInstance) const = 0;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_ITYPE_CASTER_H_ */