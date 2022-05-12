#include "PCH.hpp"
#include "AssetManager.hpp"

#include "Components/StaticMeshComponent.hpp"

// ============================== [Public Local Methods] ============================== //

void    StaticMeshComponent::SetModel               (ANSICHAR const* p_name) noexcept
{
    AssetManager& assetManager = AssetManager::Get();

    m_model = assetManager.Get<Model>(p_name, ELoadingMode::BLOCKING);

    if (m_model)
        m_materialInstances.resize(m_model->GetMeshes().size());
    else
        m_materialInstances.clear();

    for (auto& materialInstance : m_materialInstances)
    {
        if (!materialInstance)
            materialInstance = assetManager.Get<MaterialInstance>("Default/MaterialInstances/default", ELoadingMode::ASYNCHRONOUS);
    }
}

void    StaticMeshComponent::SetModel               (std::shared_ptr<Model> const& p_model) noexcept
{
    if (p_model)
    {
        m_model = p_model;

        m_materialInstances.resize(m_model->GetMeshes().size());

        for (auto& materialInstance : m_materialInstances)
        {
            if (!materialInstance)
                materialInstance = AssetManager::Get().Get<MaterialInstance>("Default/MaterialInstances/default", ELoadingMode::ASYNCHRONOUS);
        }
    }

    else
        m_materialInstances.clear();
}

void    StaticMeshComponent::SetMaterialInstance    (uint32          p_index,
                                                     ANSICHAR const* p_name) noexcept
{
    if (p_index < m_materialInstances.size())
    {
        m_materialInstances[p_index] = AssetManager::Get().Get<MaterialInstance>(p_name, ELoadingMode::ASYNCHRONOUS);

        if (!m_materialInstances[p_index])
            m_materialInstances[p_index] = AssetManager::Get().Get<MaterialInstance>("Default/MaterialInstances/default", ELoadingMode::ASYNCHRONOUS);
    }
}

void    StaticMeshComponent::SetMaterialInstance    (uint32                                   p_index,
                                                     std::shared_ptr<MaterialInstance> const& p_materialInstance) noexcept
{
    if (p_index < m_materialInstances.size())
    {
        m_materialInstances[p_index] = p_materialInstance;
    }
}

void    StaticMeshComponent::Serialize  (Json&  p_serialize)    const
{
    PrimitiveComponent::Serialize(p_serialize);

    if (m_model.get() != nullptr)
        p_serialize["Model"] = m_model.get()->GetName();

    for (auto materialInstance : m_materialInstances)
    {
        if (materialInstance.get() != nullptr)
            p_serialize["MaterialInstances"] += materialInstance.get()->GetName();
    }
}

void    StaticMeshComponent::Deserialize  (Json const&  p_deserialize,
                                           Json const&  p_components)
{
    PrimitiveComponent::Deserialize(p_deserialize, p_components);

    if (p_deserialize.contains("Model"))
        SetModel(p_deserialize["Model"].get<std::string>().c_str());

    if (p_deserialize.contains("MaterialInstances"))
    {
        int count = 0;

        for (auto const& node : p_deserialize["MaterialInstances"])
        {
            SetMaterialInstance(count, AssetManager::Get().Get<MaterialInstance>(node.get<std::string>().c_str()));
            count++;
        }
    }
}