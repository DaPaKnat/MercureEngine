#include "PCH.hpp"

#include "Vulkan/Asset/Model/Vertex.hpp"

// ============================== [Public Static Methods] ============================== //

void                                                Vertex::HashCombine                 (size_t& p_seed, size_t p_hash)
{
    p_seed += 0x9e3779b9 + (p_seed << 6) + (p_seed >> 2);
    p_seed ^= p_hash;
}

VkVertexInputBindingDescription                     Vertex::GetBindingDescription       ()
{
    return VkVertexInputBindingDescription {
        0u,
        sizeof(Vertex),
        VK_VERTEX_INPUT_RATE_VERTEX
    };
}

std::array<VkVertexInputAttributeDescription, 4>    Vertex::GetAttributeDescriptions    ()
{
    return std::array<VkVertexInputAttributeDescription, 4> {
        0u,
        0u,
        VK_FORMAT_R32G32B32_SFLOAT,
        offsetof(Vertex, position),

        1u,
        0u,
        VK_FORMAT_R32G32B32_SFLOAT,
        offsetof(Vertex, normal),

        2u,
        0u,
        VK_FORMAT_R32G32_SFLOAT,
        offsetof(Vertex, uv),

        3u,
        0u,
        VK_FORMAT_R32G32B32_SFLOAT,
        offsetof(Vertex, tangent)
    };
}