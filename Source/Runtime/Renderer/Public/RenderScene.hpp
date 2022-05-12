#ifndef __RENDER_SCENE_HPP__
#define __RENDER_SCENE_HPP__

#include "Light/SpotLightComponent.hpp"
#include "Light/PointLightComponent.hpp"
#include "Light/DirectionalLightComponent.hpp"

#include "Camera/CameraComponent.hpp"
#include "Components/StaticMeshComponent.hpp"

struct RenderScene : public UniqueObject
{
    std::atomic_bool                        isAvailable;
    CameraComponent*                        camera;
    std::vector<SpotLightComponent*>        spotLightComponents;
    std::vector<PointLightComponent*>       pointLightComponents;
    std::vector<DirectionalLightComponent*> directionalLightComponents;
    std::vector<StaticMeshComponent*>       meshComponents;

};  // !class RenderScene

#endif // !__RENDER_SCENE_HPP__