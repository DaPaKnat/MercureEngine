#include "PCH.hpp"  

#include "Explorer/EditorExplorer.hpp"
#include "Builder/TextureBuilder.hpp"
#include "AssetManager.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

EditorExplorer::EditorExplorer  () :
    m_root          { },
    m_selectedAsset { nullptr }
{}

EditorExplorer::~EditorExplorer ()  noexcept
{}

// ============================== [Private Local Methods] ============================== //

ExplorerFolder  EditorExplorer::LoadFolder          (std::string_view   p_path)     noexcept
{
    ExplorerFolder newFolder;

    newFolder.name = p_path.substr(p_path.find_last_of("/\\") + 1);

    for (auto const& item : std::filesystem::directory_iterator(p_path))
    {
        if (item.is_directory() && item.path().filename().string() != "Shaders")
            newFolder.folders.push_back(LoadFolder(item.path().string().c_str()));
        else if (item.path().extension().string() == ".asset")
        {
            ExplorerAsset asset = LoadAsset(item.path().string().c_str());

            if (asset.isValid)
                newFolder.assets.push_back(asset);
        }
    }

    return newFolder;
}

ExplorerAsset   EditorExplorer::LoadAsset           (std::string_view   p_path)
{
    ExplorerAsset   asset;
    std::ifstream   data { p_path, std::ios::binary | std::ios::in };
    size_t          lastBackSlash = p_path.find_last_of("/\\") + 1;

    asset.name = p_path.substr(lastBackSlash, p_path.find_last_of(".") - lastBackSlash);
    asset.path = std::filesystem::path(p_path).lexically_relative(ASSET_DIRECTORY).replace_extension("").string();

    if (!data.is_open())
        throw std::runtime_error("Couldn't open asset.");

    std::string check;

    std::getline(data, check);

    if (check.back() == '\r')
        check = check.substr(0, check.size() - 1);

    auto assetType = Reflect::EnumCast<EAssetType>(check);

    if (assetType.has_value() && assetType.value() != EAssetType::SHADER)
    {
        asset.type = assetType.value();

        for (auto& c : check)
            c = tolower(c);

        check[0]        = toupper(check[0]);

        asset.typeName  = check;
        asset.isValid   = true;
    }

    return asset;
}

void            EditorExplorer::OnFileDrop          (ANSICHAR const*    p_path)     noexcept
{
    if (!std::filesystem::exists(ASSET_DIRECTORY + std::string("Game")))
        std::filesystem::create_directory(ASSET_DIRECTORY + std::string("Game"));

	AssetManager::Get().Add(p_path, "Game/");
}

void            EditorExplorer::ShowFolderContent   (ExplorerFolder&    p_folder)   noexcept
{
    if (ImGui::TreeNode(p_folder.name.c_str()))
    {
        for (auto& folder : p_folder.folders)
            ShowFolderContent(folder);

        for (ExplorerAsset& asset : p_folder.assets)
        {
            ImGui::PushID(&asset);

            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;

            if (m_selectedAsset && m_selectedAsset == &asset)
                flags |= ImGuiTreeNodeFlags_Selected;

            if (ImGui::TreeNodeEx(asset.name.c_str(), flags))
            {
                if (ImGui::IsItemClicked())
                    m_selectedAsset = &asset;

                if (ImGui::BeginDragDropSource())
                {
                    switch (asset.type)
                    {
                        case EAssetType::MATERIAL_INSTANCE:
                        {
                            std::shared_ptr<MaterialInstance> materialInstance = AssetManager::Get().Get<MaterialInstance>(m_selectedAsset->path.c_str());
                            ImGui::SetDragDropPayload(typeid(MaterialInstance).name(), &materialInstance, sizeof(materialInstance));

                            break;
                        }

                        case EAssetType::MATERIAL:
                        {
                            std::shared_ptr<Material> material = AssetManager::Get().Get<Material>(m_selectedAsset->path.c_str());
                            ImGui::SetDragDropPayload(typeid(Material).name(), &material, sizeof(material));

                            break;
                        }

                        case EAssetType::MODEL:
                        {
                            std::shared_ptr<Model> model = AssetManager::Get().Get<Model>(m_selectedAsset->path.c_str());
                            ImGui::SetDragDropPayload(typeid(Model).name(), &model, sizeof(model));

                            break;
                        }
                    }

                    ImGui::EndDragDropSource();
                }

                ImGui::TreePop();
            }

            ImGui::PopID();
        }

        ImGui::TreePop();
    }
}

void            EditorExplorer::ShowAssetInspector  ()                                  noexcept
{
    if (nullptr == m_selectedAsset)
        return;

    if (ImGui::BeginChild("AssetInspector"))
    {
        ImGui::AlignTextToFramePadding();

        ImGui::Text("Asset inspector");

        ImGui::Separator();

        ImGui::Text(("Name: " + m_selectedAsset->name).c_str());

        ImGui::Text(("Type: " + m_selectedAsset->typeName).c_str());

        ImGui::Separator();

        switch (m_selectedAsset->type)
        {
            case EAssetType::MATERIAL_INSTANCE:
            {
                std::shared_ptr<MaterialInstance> materialInstance = AssetManager::Get().Get<MaterialInstance>(m_selectedAsset->path.c_str(), ELoadingMode::BLOCKING);

                ImGui::Text("Properties:");
                EditAsset(materialInstance.get());

                break;
            }

            case EAssetType::MODEL:
            {
                std::shared_ptr<Model> model = AssetManager::Get().Get<Model>(m_selectedAsset->path.c_str(), ELoadingMode::BLOCKING);

                EditAsset(model.get());

                break;
            }

            case EAssetType::MATERIAL:
            {
                std::shared_ptr<Material> material = AssetManager::Get().Get<Material>(m_selectedAsset->path.c_str(), ELoadingMode::BLOCKING);

                ImGui::Text("Properties:");
                EditAsset(material.get());
                break;
            }
        }

        ImGui::EndChild();
    }
}

// ===================================================================================== //

template <typename T>
void    EditorExplorer::EditAsset       (T*             p_asset)
{
    static_assert(std::is_base_of_v<Asset, T>);

    Reflect::DoForAllMembers<T>([this, p_asset]  (auto const&   p_member)
    {
        std::string name = p_member.GetName();

        name = name.substr(2);

        name[0] = toupper(name[0]);

        if (name == "Name")
            return;

        if (ImGui::TreeNodeEx(name.c_str()))
        {
            ImGui::PushID(&p_member);

            using MemberT = Reflect::GetMemberType<decltype(p_member)>;

            auto value = p_member.Get(*p_asset);

            EditAssetMember(&value);

            Reflect::SetMemberValue<MemberT>(*p_asset, p_member.GetName(), value);

            ImGui::TreePop();

            ImGui::PopID();
        }
    });

    static_assert(std::is_base_of_v<Asset, T>);
}

template <typename T>
void    EditorExplorer::EditAssetMember (T*             p_member)
{

}

void    EditorExplorer::EditAssetMember (MaterialData*  p_data)
{
    ImGui::PushID(&p_data->albedo);
    ImGui::Text("Albedo");
    EditAssetMember(&p_data->albedo);
    ImGui::PopID();

    ImGui::PushID(&p_data->metallic);
    ImGui::Text("Metallic");
    EditAssetMember(&p_data->metallic);
    ImGui::PopID();

    ImGui::PushID(&p_data->roughness);
    ImGui::Text("Roughness");
    EditAssetMember(&p_data->roughness);
    ImGui::PopID();

    ImGui::PushID(&p_data->ao);
    ImGui::Text("AO");
    EditAssetMember(&p_data->ao);
    ImGui::PopID();
}

void    EditorExplorer::EditAssetMember (Color*         p_data)
{
    ImGui::ColorEdit4("", &p_data->m_r, ImGuiColorEditFlags_NoLabel);
}

void    EditorExplorer::EditAssetMember (Vector3*       p_data)
{
    ImGui::PushItemWidth(70);

    ImGui::PushID(&(p_data->m_x));

    ImGui::DragFloat("", &(p_data->m_x), 1.0f, 0.0f, 0.0f, "X: %.2f", 0.1f);
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::PushID(&(p_data->m_y));
    ImGui::DragFloat("", &(p_data->m_y), 1.0f, 0.0f, 0.0f, "Y: %.2f", 0.1f);
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::PushID(&(p_data->m_z));
    ImGui::DragFloat("", &(p_data->m_z), 1.0f, 0.0f, 0.0f, "Z: %.2f", 0.1f);

    ImGui::PopID();

    ImGui::PopItemWidth();
}