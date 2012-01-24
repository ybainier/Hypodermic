#ifndef     HYPODERMIC_ITYPE_CASTER_H_
# define    HYPODERMIC_ITYPE_CASTER_H_

# include <memory>
# include <typeinfo>


namespace Hypodermic
{

    class ITypeCaster
    {
    public:
        virtual const std::type_info& typeInfo() const = 0;

        virtual std::shared_ptr< void > cast(std::shared_ptr< void > concreteInstance) const = 0;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_ITYPE_CASTER_H_ */