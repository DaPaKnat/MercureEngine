#ifndef __DIRECTIONAL_LIGHT_HPP__
#define __DIRECTIONAL_LIGHT_HPP__

#include "GameFramework/Entity.hpp"

#include "Light/DirectionalLightComponent.hpp"

DECLARE_LOG_CATEGORY(LogDirectionalLight);

class ENGINE_API DirectionalLight : public Entity
{
    REFLECT(DirectionalLight)

    typedef Entity Super;

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        DirectionalLight    ();

        DirectionalLight    (DirectionalLight const&)   = default;

        DirectionalLight    (DirectionalLight&&)        = default;

        ~DirectionalLight   ()                          = default;

    // ============================== [Public Local Operators] ============================== //

        DirectionalLight&   operator=(DirectionalLight const&)  = default;

        DirectionalLight&   operator=(DirectionalLight&&)       = default;

    private:

    // ============================== [Private Local Variables] ============================== //

        DirectionalLightComponent* m_component = nullptr;

    // ============================== [Private Local Methods] ============================== //

        bool    ComponentsCreation              ();
        

};  // !class DirectionalLight

#include "DirectionalLight.generated.hpp"

#endif // !__DIRECTIONAL_LIGHT_HPP__