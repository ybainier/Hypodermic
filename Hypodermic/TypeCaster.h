#ifndef     HYPODERMIC_TYPE_CASTER_H_
# define    HYPODERMIC_TYPE_CASTER_H_

# include <memory>
# include <Hypodermic/ITypeCaster.h>


namespace Hypodermic
{

    template <class ConcreteType, class InterfaceType>
    class TypeCaster : public ITypeCaster
    {
    public:
        TypeCaster(const std::type_info& typeInfo);

        const std::type_info& typeInfo() const;

        std::shared_ptr< void > cast(std::shared_ptr< void > concreteInstance) const;

    private:
        const std::type_info& typeInfo_;
    };

} // namespace Hypodermic


# include <Hypodermic/TypeCaster.hpp>

#endif /* !HYPODERMIC_TYPE_CASTER_H_ */