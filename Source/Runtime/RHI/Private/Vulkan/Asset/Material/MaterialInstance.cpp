#include "PCH.hpp"
#include "AssetManager.hpp"

#include "Vulkan/Asset/Material/MaterialInstance.hpp"

// ============================== [Public Constructor] ============================== //

MaterialInstance::MaterialInstance  (std::string               const& p_name,
                                     std::shared_ptr<Material> const& p_material) : Asset(p_name)
{
    m_isPending.store(true, std::memory_order_relaxed);

    if (p_material->IsValid())
    {
        m_material = p_material;
        m_data     = m_material->GetMaterialData();
    }

    else
    {
        m_material = AssetManager::Get().Get<Material>("Default/Materials/default", ELoadingMode::BLOCKING);
        m_data     = m_material->GetMaterialData();
    }

    m_isLoaded .store(true,  std::memory_order_release);
    m_isPending.store(false, std::memory_order_release);
}

MaterialInstance::MaterialInstance  (std::string const&          p_name,
                                     std::shared_ptr<Material>&& p_material) : Asset(p_name)
{
    m_isPending.store(true, std::memory_order_relaxed);

    if (p_material->IsValid())
    {
        m_material = std::move(p_material);
        m_data     = m_material->GetMaterialData();
    }

    else
    {
        m_material = AssetManager::Get().Get<Material>("Default/Materials/default", ELoadingMode::BLOCKING);
        m_data     = m_material->GetMaterialData();
    }

    m_isLoaded .store(true,  std::memory_order_release);
    m_isPending.store(false, std::memory_order_release);
}

// ============================== [Public Local Methods] ============================== //

void    MaterialInstance::SetAlbedo     (Color const& p_albedo) noexcept
{
    float a = m_data.albedo.m_a;

    m_data.albedo = p_albedo;

    m_data.albedo.m_a = a;
}

void    MaterialInstance::SetAlbedo     (Color&& p_albedo) noexcept
{
    float a = m_data.albedo.m_a;

    m_data.albedo = std::move(p_albedo);

    m_data.albedo.m_a = a;
}

void    MaterialInstance::SetMetallic   (float p_value) noexcept
{
    m_data.metallic = p_value;
}

void    MaterialInstance::SetRoughness  (float p_value) noexcept
{
    m_data.roughness = p_value;
}

void    MaterialInstance::SetAO         (float p_value) noexcept
{
    m_data.ao = p_value;
}

// ============================== [Interface Private Local Methods] ============================== //

void    MaterialInstance::Deserialize   (std::string const& p_path) noexcept
{
    std::ifstream asset(p_path, std::ios::in);

    if (asset.is_open())
    {
        // Checks file header.
        std::string header;

        std::getline(asset, header);

        if (header == Reflect::GetEnumName(EAssetType::MATERIAL_INSTANCE))
        {
            // Parses the json file.
            Json json = Json::parse(asset);

            m_material       = AssetManager::Get().Get<Material>(json.value("Material", "").c_str(), ELoadingMode::ASYNCHRONOUS);

            m_data.albedo    = json.value<Color>("Albedo",    Color::White);
            m_data.metallic  = json.value<float>("Metallic",  0.0f);
            m_data.roughness = json.value<float>("Roughness", 0.0f);
            m_data.ao        = json.value<float>("AO",        0.0f);

            m_isLoaded.store(true, std::memory_order_release);
        }

        else
            LOG(LogAssetManager, Error, "MaterialInstance file corrupted : %s", p_path.c_str());
    }

    else
        LOG(LogAssetManager, Error, "Failed to open \"%s\" for deserialization", p_path.c_str());

    m_isPending.store(false, std::memory_order_release);
}

void    MaterialInstance::Serialize     (std::string const& p_path) noexcept
{
    std::ofstream asset(p_path, std::ios::out);

    if (asset.is_open())
    {
        // Inserts asset type as header in the file.
        asset << EAssetType::MATERIAL_INSTANCE << '\n';

        Json json;

        json["Material"]  = m_material->GetName();
        json["Albedo"]    = m_data.albedo;
        json["Metallic"]  = m_data.metallic;
        json["Roughness"] = m_data.roughness;
        json["AO"]        = m_data.ao;

        // Serializes the json on disk.
        asset << json.dump(4);
    }

    else
        LOG(LogAssetManager, Error, "Failed to open \"%s\" for serialization", p_path.c_str());

    m_isLoaded .store(false, std::memory_order_release);
    m_isPending.store(false, std::memory_order_release);
}