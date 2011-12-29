#ifndef     SINGLE_REGISTRATION_STYLE_H_
# define    SINGLE_REGISTRATION_STYLE_H_

# include <boost/uuid/uuid.hpp>
# include <boost/uuid/random_generator.hpp>


namespace Hypodermic
{

    class SingleRegistrationStyle
    {
    public:
        SingleRegistrationStyle()
            : id_(boost::uuids::random_generator()())
            , preserveDefaults_(false)
            , target_(nullptr)
        {
        }

        const boost::uuids::uuid& id() const
        {
            return id_;
        }

        void id(const boost::uuids::uuid& value)
        {
            id_ = value;
        }

        bool preserveDefaults() const
        {
            return preserveDefaults_;
        }

        void preserveDefaults(bool value)
        {
            preserveDefaults_ = value;
        }

        IComponentRegistration* target()
        {
            return target_;
        }

        void target(IComponentRegistration* value)
        {
            target_ = value;
        }

    private:
        boost::uuids::uuid id_;
        bool preserveDefaults_;
        IComponentRegistration* target_;
    };

} // namespace Hypodermic


#endif /* !SINGLE_REGISTRATION_STYLE_H_ */