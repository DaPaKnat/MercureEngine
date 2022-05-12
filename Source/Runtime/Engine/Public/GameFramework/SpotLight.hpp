#ifndef __SPOT_LIGHT_HPP__
#define __SPOT_LIGHT_HPP__

#include "GameFramework/Entity.hpp"

#include "Light/SpotLightComponent.hpp"

DECLARE_LOG_CATEGORY(LogSpotLight);

class ENGINE_API SpotLight : public Entity
{
    REFLECT(SpotLight)

    typedef Entity Super;

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        SpotLight   ();

        SpotLight   (SpotLight const&)  = default;

        SpotLight   (SpotLight&&)       = default;

        ~SpotLight  ()                  = default;

    // ============================== [Public Local Operators] ============================== //

        SpotLight&  operator=(SpotLight const&) = default;

        SpotLight&  operator=(SpotLight&&)      = default;

    private:

    // ============================== [Private Local Variables] ============================== //

        SpotLightComponent* m_component = nullptr;

    // ============================== [Private Local Variables] ============================== //

        bool    ComponentsCreation              ();

};  // !class SpotLight

#include "SpotLight.generated.hpp"

#endif // !__SPOT_LIGHT_HPP__