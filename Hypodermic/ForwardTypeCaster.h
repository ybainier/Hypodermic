#ifndef     HYPODERMIC_FORWARD_TYPE_CASTER_H_
# define    HYPODERMIC_FORWARD_TYPE_CASTER_H_

# include <memory>
# include <Hypodermic/ITypeCaster.h>


namespace Hypodermic
{

    class ForwardTypeCaster : public ITypeCaster
    {
    public:
        ForwardTypeCaster(const std::type_info& typeInfo);

        const std::type_info& typeInfo() const;

        std::shared_ptr< void > cast(std::shared_ptr< void > concreteInstance) const;

    private:
        const std::type_info& typeInfo_;
    };

} // namespace Hypodermic

#endif /* !HYPODERMIC_FORWARD_TYPE_CASTER_H_ */