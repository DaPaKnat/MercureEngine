#ifndef __VULKAN_VERTEX_HPP__
#define __VULKAN_VERTEX_HPP__

#include "Vulkan/Vulkan.hpp"

struct ENGINE_API Vertex
{
    // ============================== [Public Static Methods] ============================== //

        static void                                             HashCombine             (size_t& p_seed, size_t p_hash);

        static VkVertexInputBindingDescription                  GetBindingDescription   ();

        static std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescriptions();

    // ============================== [Public Local Variables] ============================== //

        Vector3 position;

        Vector3 normal;

        Vector2 uv;

        Vector3 tangent;

    // ============================== [Public Constructors and Destructor] ============================== //

        Vertex  ()                          = default;

        Vertex  (Vertex const&  p_other)    = default;

        Vertex  (Vertex&&       p_other)    = default;

        ~Vertex ()                          = default;

    // ============================== [Public Local Operators] ============================== //

        Vertex& operator=   (Vertex const&  p_other)    = default;

        Vertex& operator=   (Vertex&&       p_other)    = default;

        bool    operator==  (Vertex const&  p_other)    const
        { 
            return position == p_other.position &&
                   normal   == p_other.normal   &&
                   uv       == p_other.uv;
        }

        bool    operator!=  (Vertex const&  p_other)    const
        { 
            return position != p_other.position ||
                   normal   != p_other.normal   ||
                   uv       != p_other.uv;
        }

};  // !struct Vertex

namespace std
{
    template<> struct hash<Vector2>
    {
        size_t operator()(Vector2 const& p_vector) const
        {
            size_t seed = 0;

            hash<float> hasher;

            Vertex::HashCombine(seed, hasher(p_vector.m_x));
            Vertex::HashCombine(seed, hasher(p_vector.m_y));

            return seed;
        }
    };

    template<> struct hash<Vector3>
    {
        size_t operator()(Vector3 const& p_vector) const
        {
            size_t seed = 0;

            hash<float> hasher;

            Vertex::HashCombine(seed, hasher(p_vector.m_x));
            Vertex::HashCombine(seed, hasher(p_vector.m_y));
            Vertex::HashCombine(seed, hasher(p_vector.m_z));

            return seed;
        }
    };

    template<> struct hash<Vertex>
    {
        size_t operator()(Vertex const& p_vertex) const
        {
            size_t h1 = hash<Vector3>()(p_vertex.position);
            size_t h2 = hash<Vector3>()(p_vertex.normal);
            size_t h3 = hash<Vector2>()(p_vertex.uv);
            size_t h4 = hash<Vector3>()(p_vertex.tangent);

            return ((((h1 ^ (h2 << 1)) >> 1) ^ h3) << 1) ^ h4;
        }
    };
}

#include "Vertex.generated.hpp"

#endif // !__VULKAN_VERTEX_HPP__