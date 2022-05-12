#include "PCH.hpp"

#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>

#include "AssetManager.hpp"

#include "Builder/ModelBuilder.hpp"
#include "Builder/TextureBuilder.hpp"

#include <fbxsdk.h>

// ============================== [Private Static Variables] ============================== //

std::array<std::string, 2> ModelBuilder::SupportedExtensions = { ".obj", ".fbx" };

// ============================== [Public Static Methods] ============================== //

bool                    ModelBuilder::IsExtensionSupported  (std::string const& p_extension) noexcept
{
    for (std::string const& supportedExtension : SupportedExtensions)
    {
        if (supportedExtension == p_extension)
            return true;
    }

    return false;
}

std::shared_ptr<Model>  ModelBuilder::BuildFromFile         (std::string const& p_name,
                                                             std::string const& p_path) noexcept
{
    if (p_name.empty() || p_path.empty())
        return nullptr;

    std::string extension(std::filesystem::path(p_path).extension().string());

    if (extension == ".obj")
        return BuildFromObj(p_name, p_path);

    else if (extension == ".fbx")
        return BuildFromFbx(p_name, p_path);

    else if (extension == ".gltf")
        return BuildFromGLTF(p_name, p_path);

    else
        LOG(LogAssetManager, Error, "Extension not supported : %s", extension);

    return nullptr;
}

// ============================== [Private Static Methods (OBJ)] ============================== //

namespace OBJLoader
{
    void    LoadMeshes      (tinyobj::attrib_t             const& p_attribute,
                             std::vector<tinyobj::shape_t> const& p_shapes,
                             std::vector<MeshCreateInfo>&         p_outMeshes)
    {
        std::vector<std::unordered_map<Vertex, uint32>> uniqueVerticesPerMaterial(p_outMeshes.size());

        for (auto const& shape : p_shapes)
        {
            size_t indexOffset = 0;

            for (size_t i = 0; i < shape.mesh.num_face_vertices.size(); ++i)
            {
                uint8 count          = shape.mesh.num_face_vertices[i];
                int32 materialID     = shape.mesh.material_ids     [i];

                auto& uniqueVertices = uniqueVerticesPerMaterial[materialID + 1];
                auto& mesh           = p_outMeshes              [materialID + 1];

                for (uint8 j = 0; j < count; ++j)
                {
                    auto const& index = shape.mesh.indices[indexOffset + j];

                    Vertex vertex = {};

                    vertex.position = {
                        p_attribute.vertices[3 * index.vertex_index + 0],
                        p_attribute.vertices[3 * index.vertex_index + 1],
                        p_attribute.vertices[3 * index.vertex_index + 2]
                    };

                    vertex.normal = {
                        p_attribute.normals[3 * index.normal_index + 0],
                        p_attribute.normals[3 * index.normal_index + 1],
                        p_attribute.normals[3 * index.normal_index + 2],
                    };

                    vertex.uv = {
                        0.0f + p_attribute.texcoords[2 * index.texcoord_index + 0],
                        1.0f - p_attribute.texcoords[2 * index.texcoord_index + 1]
                    };

                    vertex.tangent = {
                        0.0f,
                        0.0f,
                        0.0f
                    };

                    if (uniqueVertices.count(vertex) == 0)
                    {
                        uniqueVertices[vertex] = static_cast<uint32>(mesh.vertices.size());

                        mesh.vertices.push_back(vertex);
                    }

                    mesh.indices.push_back(uniqueVertices[vertex]);
                }

                indexOffset += count;
            }
        }
    }

    void    LoadMaterials   (tinyobj::attrib_t                const& p_attribute,
                             std::vector<tinyobj::material_t> const& p_materials,
                             std::string                      const& p_name,
                             std::string                      const& p_directory)
    {
        AssetManager& assetManager = AssetManager::Get();

        std::filesystem::path name(p_name);

        for (size_t i = 0; i < p_materials.size(); ++i)
        {
            if (p_materials[i].name.empty())
                continue;

            MaterialCreateInfo materialCI;

            materialCI.albedo.m_r = p_materials[i].diffuse[0];
            materialCI.albedo.m_g = p_materials[i].diffuse[1];
            materialCI.albedo.m_b = p_materials[i].diffuse[2];
            materialCI.albedo.m_a = p_materials[i].dissolve;
            materialCI.metallic   = p_materials[i].metallic;
            materialCI.roughness  = p_materials[i].roughness;
            materialCI.ao         = p_materials[i].sheen;

            materialCI.albedoMapName    = name.replace_filename(p_materials[i].diffuse_texname)  .replace_extension("").string();
            materialCI.metallicMapName  = name.replace_filename(p_materials[i].metallic_texname) .replace_extension("").string();
            materialCI.roughnessMapName = name.replace_filename(p_materials[i].roughness_texname).replace_extension("").string();
            materialCI.aoMapName        = name.replace_filename(p_materials[i].sheen_texname)    .replace_extension("").string();

            assetManager.Add(TextureBuilder::BuildFromFile(materialCI.albedoMapName,    p_directory + p_materials[i].diffuse_texname));
            assetManager.Add(TextureBuilder::BuildFromFile(materialCI.metallicMapName,  p_directory + p_materials[i].metallic_texname));
            assetManager.Add(TextureBuilder::BuildFromFile(materialCI.roughnessMapName, p_directory + p_materials[i].roughness_texname));
            assetManager.Add(TextureBuilder::BuildFromFile(materialCI.aoMapName,        p_directory + p_materials[i].sheen_texname));

            if (p_materials[i].normal_texname.empty())
            {
                materialCI.normalMapName = name.replace_filename(p_materials[i].bump_texname).replace_extension("").string();

                assetManager.Add(TextureBuilder::BuildFromFile(materialCI.normalMapName, p_directory + p_materials[i].bump_texname));
            }

            else
            {
                materialCI.normalMapName = name.replace_filename(p_materials[i].normal_texname).replace_extension("").string();

                assetManager.Add(TextureBuilder::BuildFromFile(materialCI.normalMapName, p_directory + p_materials[i].normal_texname));
            }

            if (!assetManager.Add(std::make_shared<Material>(name.replace_filename(p_materials[i].name).string(), std::move(materialCI))))
                LOG(LogAssetManager, Error, "Failed to load materials for : ", name.string().c_str());
        }
    }

    void    ComputeTangents (std::vector<MeshCreateInfo>& p_outMeshes)
    {
        for (auto& mesh : p_outMeshes)
        {
            for (size_t i = 0; i < mesh.indices.size(); i += 3)
            {
                // Gets triangle's vertices.
                Vertex& v1 = mesh.vertices.at(mesh.indices.at(i + 0));
                Vertex& v2 = mesh.vertices.at(mesh.indices.at(i + 1));
                Vertex& v3 = mesh.vertices.at(mesh.indices.at(i + 2));

                // Computes triangle's edges.
                Vector3 edge1(v2.position - v1.position);
                Vector3 edge2(v3.position - v1.position);

                // Computes uvs' deltas.
                Vector2 deltaUV1 = v2.uv - v1.uv;
                Vector2 deltaUV2 = v3.uv - v1.uv;

                // Computes tangents.
                if (float det = Vector2::Cross(deltaUV1, deltaUV2))
                {
                    Vector3 tangent(((deltaUV2.m_y * edge1 - deltaUV1.m_y * edge2) * (1.0f / det)).GetSafeNormalized());

                    v1.tangent = tangent;
                    v2.tangent = tangent;
                    v3.tangent = tangent;
                }
            }
        }
    }
}

std::shared_ptr<Model>  ModelBuilder::BuildFromObj  (std::string const& p_name,
                                                     std::string const& p_path) noexcept
{
    tinyobj::attrib_t                attribute;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    std::string                      warning;
    std::string                      error;
    std::string                      directory(std::filesystem::path(p_path).remove_filename().string());

    if (!tinyobj::LoadObj(&attribute,
                          &shapes,
                          &materials,
                          &warning,
                          &error,
                          p_path   .c_str(),
                          directory.c_str()))
    {
        if (!warning.empty())
            LOG(LogAssetManager, Warning, "Warning during OBJ file loading : %s", warning);
        if (!error  .empty())
            LOG(LogAssetManager, Error,   "Error during OBJ file loading : %s",   error);

        return nullptr;
    }

    if (attribute.vertices .size() == 0 ||
        attribute.normals  .size() == 0 ||
        attribute.texcoords.size() == 0 )
    {
        LOG(LogAssetManager, Error, "Error during OBJ file loading : incomplete model");
        return nullptr;
    }

    std::vector<MeshCreateInfo> meshes(materials.size() + 1);

    OBJLoader::LoadMeshes     (attribute, shapes, meshes);
    OBJLoader::LoadMaterials  (attribute, materials, p_name, directory);
    OBJLoader::ComputeTangents(meshes);

    return std::make_shared<Model>(p_name, meshes);
}

// ============================== [Private Static Methods (FBX)] ============================== //

namespace FBXLoader
{
    void    AddMesh     (fbxsdk::FbxNode*             p_node,
                         fbxsdk::FbxNode*             p_parent,
                         std::vector<MeshCreateInfo>& p_outSubmeshes)
    {
        MeshCreateInfo submesh;

        if (fbxsdk::FbxMesh* mesh = p_node->GetMesh())
        {
            mesh->RemoveBadPolygons();

            submesh.vertices.reserve(mesh->GetPolygonCount());

            for (int32 i = 0u; i < mesh->GetControlPointsCount(); ++i)
            {
                fbxsdk::FbxVector4 position = mesh->GetControlPointAt(i);

                Vertex vertex;

                vertex.position = Vector3(static_cast<float>(position.mData[0]),
                                          static_cast<float>(position.mData[1]),
                                          static_cast<float>(position.mData[2]));

                submesh.vertices.push_back(std::move(vertex));
            }

            submesh.indices.resize(mesh->GetPolygonVertexCount());

            p_outSubmeshes.push_back(submesh);
        }
    }

    void    ProcessNode (fbxsdk::FbxNode*             p_node,
                         fbxsdk::FbxNode*             p_parent,
                         std::vector<MeshCreateInfo>& p_outSubmeshes)
    {
        int32                           childCount = p_node   ->GetChildCount   ();
        fbxsdk::FbxNodeAttribute*       attribute  = p_node   ->GetNodeAttribute();
        fbxsdk::FbxNodeAttribute::EType type       = attribute->GetAttributeType();

        switch (type)
        {
            case fbxsdk::FbxNodeAttribute::eMesh:
                AddMesh(p_node, p_parent, p_outSubmeshes);
                break;

            default:
                break;
        }

        for (int32 i = 0; i < childCount; ++i)
        {
            ProcessNode(p_node->GetChild(i), p_node, p_outSubmeshes);
        }
    }
}

std::shared_ptr<Model>  ModelBuilder::BuildFromFbx  (std::string const& p_name,
                                                     std::string const& p_path) noexcept
{
    fbxsdk::FbxManager*    manager  = fbxsdk::FbxManager   ::Create();
    fbxsdk::FbxIOSettings* settings = fbxsdk::FbxIOSettings::Create(manager, IOSROOT);
    fbxsdk::FbxScene*      scene    = fbxsdk::FbxScene     ::Create(manager, p_name.c_str());
    fbxsdk::FbxImporter*   importer = fbxsdk::FbxImporter  ::Create(manager, "");

    manager->SetIOSettings(settings);

    if (!importer->Initialize(p_path.c_str(), -1, settings))
    {
        LOG(LogAssetManager, Error, "Failed to initialize FBX importer : %s", importer->GetStatus().GetErrorString());
        return nullptr;
    }

    if (!importer->Import(scene))
    {
        LOG(LogAssetManager, Error, "Failed to import FBX scene : %s", importer->GetStatus().GetErrorString());
        return nullptr;
    }

    fbxsdk::FbxGeometryConverter converter(manager);

    converter.Triangulate(scene, true);

    fbxsdk::FbxAxisSystem ownAxisSystem(fbxsdk::FbxAxisSystem::eYAxis,
                                        fbxsdk::FbxAxisSystem::eParityOdd,
                                        fbxsdk::FbxAxisSystem::eRightHanded);

    if (ownAxisSystem != scene->GetGlobalSettings().GetAxisSystem())
        ownAxisSystem.ConvertScene(scene);

    std::vector<MeshCreateInfo> submeshes;

    if (fbxsdk::FbxNode* node = scene->GetRootNode())
    {
        FBXLoader::ProcessNode(node, nullptr, submeshes);
    }

    importer->Destroy();
    scene   ->Destroy();
    manager ->Destroy();

    return std::make_shared<Model>(p_name, submeshes);
}

// ============================== [Private Static Methods (GLTF)] ============================== //

std::shared_ptr<Model>  ModelBuilder::BuildFromGLTF (std::string const& p_name,
                                                     std::string const& p_path) noexcept
{
    return nullptr;
}