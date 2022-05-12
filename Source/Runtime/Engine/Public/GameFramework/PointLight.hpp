#ifndef __POINT_LIGHT_HPP__
#define __POINT_LIGHT_HPP__

#include "GameFramework/Entity.hpp"

#include "Light/PointLightComponent.hpp"

DECLARE_LOG_CATEGORY(LogPointLight);

class ENGINE_API PointLight : public Entity
{
    REFLECT(PointLight)

    typedef Entity Super;

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        PointLight  ();

        PointLight  (PointLight const&) = default;

        PointLight  (PointLight&&)      = default;

        ~PointLight ()                  = default;

    // ============================== [Public Local Operators] ============================== //

        PointLight& operator=(PointLight const&)    = default;

        PointLight& operator=(PointLight&&)         = default;

    private:

    // ============================== [Private Local Variables] ============================== //

        PointLightComponent* m_component = nullptr;

    // ============================== [Private Local Variables] ============================== //

        bool    ComponentsCreation              ();

};  // !class PointLight

#include "PointLight.generated.hpp"

#endif // !__POINT_LIGHT_HPP__