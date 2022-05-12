#ifndef __RENDER_LIST_HPP__
#define __RENDER_LIST_HPP__

#include "RHIAsset.hpp"

#include "Light/SpotLightComponent.hpp"
#include "Light/PointLightComponent.hpp"
#include "Light/DirectionalLightComponent.hpp"

// ============================== [Data Structure] ============================== //

struct MS_ALIGN(16) Camera
{
    Vector4   position;
    Matrix4x4 view;
    Matrix4x4 projection;
};

// ============================== [Using Declaration] ============================== //

using MeshInstance = std::tuple<size_t, Matrix4x4, MaterialData const*, Mesh const*>;

// ================================================================================= //

struct RenderList : public UniqueObject
{
    Camera                                 camera;
    std::vector<SpotLightData>             spotLights;
    std::vector<PointLightData>            pointLights;
    std::vector<DirectionalLightData>      directionalLights;
    std::vector<MaterialRenderData const*> materials;
    std::vector<MeshInstance>              opaqueMeshes;
    std::vector<MeshInstance>              transparentMeshes;

};  // !class RenderList

#endif // !__RENDER_LIST_HPP__