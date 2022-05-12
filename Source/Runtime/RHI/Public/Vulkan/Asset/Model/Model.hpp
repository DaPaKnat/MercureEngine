#ifndef __VULKAN_MODEL_HPP__
#define __VULKAN_MODEL_HPP__

#include "Asset.hpp"
#include "Vertex.hpp"

#include "Vulkan/Object/DeviceAllocator.hpp"

// ============================== [Data Structures] ============================== //

struct MeshCreateInfo
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

};  // !struct MeshCreateInfo

struct Mesh
{
    uint32 vertexCount;
    Buffer vertexBuffer;
    uint32 indexCount;
    Buffer indexBuffer;

};  // !struct Mesh

// =============================================================================== //

class ENGINE_API Model : public Asset
{
    REFLECT(Model)

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        Model   () = delete;

        Model   (std::string                 const& p_name) : Asset(p_name) {};

        Model   (std::string                 const& p_name,
                 std::vector<MeshCreateInfo> const& p_meshes);

        ~Model  () = default;

    // ============================== [Public Local Methods] ============================== //

        INLINE std::vector<Mesh> const& GetMeshes() const noexcept { return m_meshes; }

    private:

    // ============================== [Private Local Properties] ============================== //
        
        std::vector<Mesh> m_meshes;

    // ============================== [Interface Private Local Methods] ============================== //

        void    Deserialize (std::string const& p_path) noexcept final override;

        void    Serialize   (std::string const& p_path) noexcept final override;

};  // !class Model

#include "Model.generated.hpp"

#endif // !__VULKAN_MODEL_HPP__