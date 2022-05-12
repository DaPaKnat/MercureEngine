#include "PCH.hpp"
#include "RHI.hpp"
#include "Level.hpp"
#include "Renderer.hpp"
#include "ThreadPool.hpp"
#include "AssetManager.hpp"

// ============================== [Public Constructor] ============================== //

Renderer::Renderer  () :
    m_running { false }
{

}

// ============================== [Module Public Local Methods] ============================== //

void    Renderer::Initialize    (EngineKey const& p_passkey)
{
    LOG(LogRenderer, Warning, "\nInitializing Renderer...\n");

    m_renderList = std::make_unique<RenderList>();

    m_activeScene   = std::make_unique<RenderScene>();
    m_inactiveScene = std::make_unique<RenderScene>();

    m_activeScene  ->isAvailable.store(false, std::memory_order_relaxed);
    m_inactiveScene->isAvailable.store(true,  std::memory_order_relaxed);

    m_initialized = true;

    LOG(LogRenderer, Warning, "\nRenderer initialized\n");
}

void    Renderer::Update        (EngineKey const& p_passkey)
{
    if (!m_initialized)
    {
        LOG(LogRenderer, Error, "Renderer was not initialized");
        return;
    }

    ThreadPool& threadPool = ThreadPool::Get();
    RHI&        rhi        = RHI       ::Get();

    m_running.store(true, std::memory_order_relaxed);

    while (GEngine->IsRunning())
    {
        if (!m_activeScene->isAvailable.load(std::memory_order_acquire))
        {
            threadPool.ExecuteTask();
            continue;
        }

        m_activeScene->isAvailable.store(false, std::memory_order_relaxed);

        // Swaps the scenes.
        m_activeScene.swap(m_inactiveScene);

        // Notifies that the scenes have been swapped.
        m_inactiveScene->isAvailable.store(true, std::memory_order_release);

        // Draws the scene.
        rhi.NextFrame(p_passkey).renderList = m_renderList.get();
        rhi.DrawFrame(p_passkey);
    }

    m_running.store(false, std::memory_order_relaxed);
}

void    Renderer::Shutdown      (EngineKey const& p_passkey)
{
    LOG(LogRenderer, Warning, "\nShutting down Renderer...\n");

    ThreadPool& threadPool = ThreadPool::Get();

    while (m_running.load(std::memory_order_acquire))
    {
        threadPool.ExecuteTask();
    }

    RHI::Get().GetDevice()->WaitIdle();

    m_activeScene  .reset();
    m_inactiveScene.reset();
    m_renderList   .reset();

    m_initialized = false;

    LOG(LogRenderer, Warning, "\nRenderer shut down\n");
}

// ============================== [Public Local Methods] ============================== //

void    Renderer::Render    (EngineKey const& p_passkey,
                             Level     const& p_level) noexcept
{
    if (!m_initialized)
    {
        LOG(LogRenderer, Error, "Renderer was not initialized");
        return;
    }

    #ifndef EDITOR

    ThreadPool& threadPool = ThreadPool::Get();

    while (!m_inactiveScene->isAvailable.load(std::memory_order_acquire) && m_running.load(std::memory_order_relaxed))
    {
        threadPool.ExecuteTask();
    }

    m_inactiveScene->isAvailable.store(false, std::memory_order_relaxed);

    #endif

    m_activeScene->spotLightComponents       .clear();
    m_activeScene->pointLightComponents      .clear();
    m_activeScene->directionalLightComponents.clear();
    m_activeScene->meshComponents            .clear();

    if (p_level.GetWorld() && p_level.GetWorld()->GetMainCamera())
    {
        m_activeScene->camera = p_level.GetWorld()->GetMainCamera();

        m_renderList->camera.position   = m_activeScene->camera->GetWorldLocation ();
        m_renderList->camera.view       = m_activeScene->camera->GetWorldTransform().GetMatrixInverse       ();
        m_renderList->camera.projection = m_activeScene->camera->GetCameraView    ().ComputeProjectionMatrix();

        p_level.GetVisibleComponents<SpotLightComponent>       (m_activeScene->spotLightComponents);
        p_level.GetVisibleComponents<PointLightComponent>      (m_activeScene->pointLightComponents);
        p_level.GetVisibleComponents<DirectionalLightComponent>(m_activeScene->directionalLightComponents);
        p_level.GetVisibleComponents<StaticMeshComponent>      (m_activeScene->meshComponents);

        m_renderList->spotLights       .clear();
        m_renderList->pointLights      .clear();
        m_renderList->directionalLights.clear();

        for (auto const& lightComponent : m_activeScene->spotLightComponents)
        {
            m_renderList->spotLights.emplace_back(lightComponent->GetRenderData());
        }

        for (auto const& lightComponent : m_activeScene->pointLightComponents)
        {
            m_renderList->pointLights.emplace_back(lightComponent->GetRenderData());
        }

        for (auto const& lightComponent : m_activeScene->directionalLightComponents)
        {
            m_renderList->directionalLights.emplace_back(lightComponent->GetRenderData());
        }

        m_renderList->materials        .clear();
        m_renderList->opaqueMeshes     .clear();
        m_renderList->transparentMeshes.clear();

        for (auto const& meshComponent : m_activeScene->meshComponents)
        {
            if (!meshComponent->GetModel() || !meshComponent->GetModel()->IsValid())
                continue;

            Matrix4x4   modelMatrix       = meshComponent->GetRenderMatrix     ();
            auto const& meshes            = meshComponent->GetModel            ()->GetMeshes();
            auto const& materialInstances = meshComponent->GetMaterialInstances();
            
            for (size_t i = 0; i < meshes.size() && i < materialInstances.size(); ++i)
            {
                if (!materialInstances[i]->IsValid() || !materialInstances[i]->GetMaterial()->IsValid())
                    continue;

                auto const& it = std::find_if(m_renderList->materials.begin(),
                                              m_renderList->materials.end  (), [&] (MaterialRenderData const* p_material)
                {
                    return p_material == materialInstances[i]->GetMaterialRenderDataPtr();
                });

                if (it != m_renderList->materials.end())
                {
                    if (materialInstances[i]->IsOpaque())
                    {
                        m_renderList->opaqueMeshes.push_back(std::make_tuple(std::distance(m_renderList->materials.begin(), it),
                                                             modelMatrix,
                                                             materialInstances[i]->GetMaterialDataPtr(),
                                                             &meshes[i]));
                    }

                    else
                    {
                        m_renderList->transparentMeshes.push_back(std::make_tuple(std::distance(m_renderList->materials.begin(), it),
                                                                                  modelMatrix,
                                                                                  materialInstances[i]->GetMaterialDataPtr(),
                                                                                  &meshes[i]));
                    }
                }

                else
                {
                    if (materialInstances[i]->IsOpaque())
                    {
                        m_renderList->opaqueMeshes.push_back(std::make_tuple(m_renderList->materials.size(),
                                                                             modelMatrix,
                                                                             materialInstances[i]->GetMaterialDataPtr(),
                                                                             &meshes[i]));
                    }

                    else
                    {
                        m_renderList->transparentMeshes.push_back(std::make_tuple(m_renderList->materials.size(),
                                                                                  modelMatrix,
                                                                                  materialInstances[i]->GetMaterialDataPtr(),
                                                                                  &meshes[i]));
                    }

                    m_renderList->materials.push_back(materialInstances[i]->GetMaterialRenderDataPtr());
                }
            }
        }

        std::sort(m_renderList->opaqueMeshes.begin(), m_renderList->opaqueMeshes.end(), [this] (MeshInstance const& lhs,
                                                                                                MeshInstance const& rhs)
        {
            return std::get<0>(lhs) < std::get<0>(rhs);
        });

        std::sort(m_renderList->transparentMeshes.begin(), m_renderList->transparentMeshes.end(), [this] (MeshInstance const& lhs,
                                                                                                          MeshInstance const& rhs)
        {
            return Vector3::Distance(m_activeScene->camera->GetWorldLocation(), std::get<1>(lhs).GetOrigin())
                 > Vector3::Distance(m_activeScene->camera->GetWorldLocation(), std::get<1>(rhs).GetOrigin());
        });
    }

    else
    {
        m_renderList->spotLights       .clear();
        m_renderList->pointLights      .clear();
        m_renderList->directionalLights.clear();
        m_renderList->materials        .clear();
        m_renderList->opaqueMeshes     .clear();
        m_renderList->transparentMeshes.clear();

        LOG(LogRenderer, Error, "There is no main camera in the level");
    }

    #if EDITOR

    RHI::Get().NextFrame(p_passkey).renderList = m_renderList.get();
    RHI::Get().DrawFrame(p_passkey);

    #else
    
    m_activeScene->isAvailable.store(true, std::memory_order_release);

    #endif
}