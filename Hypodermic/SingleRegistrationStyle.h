#ifndef     HYPODERMIC_SINGLE_REGISTRATION_STYLE_H_
# define    HYPODERMIC_SINGLE_REGISTRATION_STYLE_H_

# include <boost/uuid/uuid.hpp>


namespace Hypodermic
{
    class IComponentRegistration;


    class SingleRegistrationStyle
    {
    public:
        SingleRegistrationStyle();

        const boost::uuids::uuid& id() const;

        void id(const boost::uuids::uuid& value);

        bool preserveDefaults() const;

        void preserveDefaults(bool value);

        IComponentRegistration* target();

        void target(IComponentRegistration* value);

    private:
        boost::uuids::uuid id_;
        bool preserveDefaults_;
        IComponentRegistration* target_;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_SINGLE_REGISTRATION_STYLE_H_ */