#include "PCH.hpp"
#include "RHI.hpp"
#include "AssetManager.hpp"

#include "Vulkan/Object/Fence.hpp"

#include "Vulkan/Asset/Model/Model.hpp"

// ============================== [Public Constructor] ============================== //

Model::Model    (std::string                 const& p_name,
                 std::vector<MeshCreateInfo> const& p_meshes) : Asset(p_name)
{
    m_isPending.store(true, std::memory_order_relaxed);

    auto const& device    = RHI::Get().GetDevice   ();
    auto const& allocator = RHI::Get().GetAllocator();

    std::vector<Buffer> stagingVertexBuffers;
    std::vector<Buffer> stagingIndexBuffers;

    Fence         fence;
    CommandBuffer cmdBuffer(device->GetTransferCommandPool()->AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));

    cmdBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    for (size_t i = 0; i < p_meshes.size(); ++i)
    {
        if (p_meshes[i].vertices.empty() || p_meshes[i].vertices.empty())
            continue;

        Mesh   newMesh          = {};
        size_t vertexBufferSize = sizeof(Vertex) * p_meshes[i].vertices.size();
        size_t indexBufferSize  = sizeof(uint32) * p_meshes[i].indices .size();

        // Staging vertex buffer.
        Buffer             stagingVertexBuffer   = {};
        VkBufferCreateInfo stagingVertexBufferCI = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };

        stagingVertexBufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        stagingVertexBufferCI.size  = vertexBufferSize;

        allocator->CreateBuffer(stagingVertexBuffer, stagingVertexBufferCI, VMA_ALLOCATION_CREATE_MAPPED_BIT, VMA_MEMORY_USAGE_CPU_ONLY, 0u);

        memcpy(stagingVertexBuffer.allocationInfo.pMappedData, p_meshes[i].vertices.data(), vertexBufferSize);

        // Actual vertex buffer.
        VkBufferCreateInfo vertexBufferCI = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };

        vertexBufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        vertexBufferCI.size  = vertexBufferSize;

        allocator->CreateBuffer(newMesh.vertexBuffer, vertexBufferCI, 0u, VMA_MEMORY_USAGE_GPU_ONLY, 0u);

        // Staging index buffer.
        Buffer             stagingIndexBuffer   = {};
        VkBufferCreateInfo stagingIndexBufferCI = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };

        stagingIndexBufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        stagingIndexBufferCI.size  = indexBufferSize;

        allocator->CreateBuffer(stagingIndexBuffer, stagingIndexBufferCI, VMA_ALLOCATION_CREATE_MAPPED_BIT, VMA_MEMORY_USAGE_CPU_ONLY, 0u);

        memcpy(stagingIndexBuffer.allocationInfo.pMappedData, p_meshes[i].indices.data(), static_cast<size_t>(indexBufferSize));

        // Actual index buffer.
        VkBufferCreateInfo indexBufferCI = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };

        indexBufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        indexBufferCI.size  = indexBufferSize;

        allocator->CreateBuffer(newMesh.indexBuffer, indexBufferCI, 0u, VMA_MEMORY_USAGE_GPU_ONLY, 0u);

        // Copies data from staging buffers to actual buffers.
        stagingVertexBuffers.push_back(std::move(stagingVertexBuffer));
        stagingIndexBuffers .push_back(std::move(stagingIndexBuffer));

        cmdBuffer.CopyBufferToBuffer(stagingVertexBuffers.back(), newMesh.vertexBuffer);
        cmdBuffer.CopyBufferToBuffer(stagingIndexBuffers .back(), newMesh.indexBuffer);

        // Stores data for later use.
        newMesh.vertexCount = static_cast<uint32>(p_meshes[i].vertices.size());
        newMesh.indexCount  = static_cast<uint32>(p_meshes[i].indices .size());

        m_meshes.push_back(std::move(newMesh));
    }

    cmdBuffer.End();

    device->GetTransferQueue()->Submit(cmdBuffer.GetHandle(), fence.GetHandle());

    // Waits for the transfer operations to complete before destroying the staging buffers.
    fence.Wait();

    // The command buffer can be freed after the transfer operation has been completed.
    device->GetTransferCommandPool()->FreeCommandBuffer(cmdBuffer);

    for (size_t i = 0; i < m_meshes.size(); ++i)
    {
        // Staging buffers are temporary so they need to be freed after usage.
        allocator->DestroyBuffer(stagingVertexBuffers[i]);
        allocator->DestroyBuffer(stagingIndexBuffers [i]);

        // Setups debug info.
        Debug::SetBufferName(device->GetLogicalDevice(), m_meshes[i].vertexBuffer.handle, (m_name + "_VB_" + std::to_string(i)).c_str());
        Debug::SetBufferName(device->GetLogicalDevice(), m_meshes[i].indexBuffer .handle, (m_name + "_IB_" + std::to_string(i)).c_str());
    }

    m_isLoaded .store(true,  std::memory_order_release);
    m_isPending.store(false, std::memory_order_release);
}

// ============================== [Interface Private Local Methods] ============================== //

void    Model::Deserialize   (std::string const& p_path) noexcept
{
    std::ifstream asset(p_path, std::ios::in);
    
    if (!asset.is_open())
    {
        m_isPending.store(false, std::memory_order_release);

        LOG(LogAssetManager, Error, "Failed to open \"%s\" for deserialization", p_path.c_str());
        return;
    }
    
    // Checks file header.
    std::string header;

    std::getline(asset, header);

    if (header == Reflect::GetEnumName(EAssetType::MODEL))
    {
        // Parses the json file.
        Json json = Json::parse(asset);

        // Opens model's data file.
        std::ifstream data(ASSET_DIRECTORY + json.value<std::string>("Data", ""), std::ios::in | std::ios::binary);

        if (!data.is_open())
        {
            m_isPending.store(false, std::memory_order_release);

            LOG(LogAssetManager, Error, "Failed to find data file for model : %s", p_path.c_str());
            return;
        }

        auto const& device    = RHI::Get().GetDevice   ();
        auto const& allocator = RHI::Get().GetAllocator();

        // Model's raw data.
        std::vector<ANSICHAR> buffer(json.value<size_t>("Size", 0));

        data.read(buffer.data(), buffer.size());

        // Model's mesh count.
        m_meshes.resize(json.value<size_t>("Count", 0));

        std::vector<Buffer> stagingVertexBuffers(m_meshes.size());
        std::vector<Buffer> stagingIndexBuffers (m_meshes.size());

        Fence         fence;
        CommandBuffer cmdBuffer(device->GetTransferCommandPool()->AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));

        cmdBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        for (size_t i = 0, offsetIndex = 0, countIndex = 0; i < m_meshes.size(); ++i, ++countIndex, ++offsetIndex)
        {
            uint32 vertexCount  = json["Counts"] .at(countIndex)   .get<uint32>();
            uint32 vertexOffset = json["Offsets"].at(offsetIndex)  .get<uint32>();
            uint32 indexCount   = json["Counts"] .at(++countIndex) .get<uint32>();
            uint32 indexOffset  = json["Offsets"].at(++offsetIndex).get<uint32>();

            if (vertexCount == 0u || indexCount == 0u)
            {
                LOG(LogAssetManager, Error, "File corrupted", json.value("Data", "").c_str());
                return;
            }

            VkBufferCreateInfo bufferCI            = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
            Buffer             stagingVertexBuffer = {};
            Buffer             stagingIndexBuffer  = {};
            VkDeviceSize       vertexBufferSize    = sizeof(Vertex) * vertexCount;
            VkDeviceSize       indexBufferSize     = sizeof(uint32) * indexCount;

            // Staging vertex buffer.
            bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            bufferCI.size  = vertexBufferSize;

            allocator->CreateBuffer(stagingVertexBuffer, bufferCI, VMA_ALLOCATION_CREATE_MAPPED_BIT, VMA_MEMORY_USAGE_CPU_ONLY, 0u);

            memcpy(stagingVertexBuffer.allocationInfo.pMappedData, buffer.data() + vertexOffset, static_cast<size_t>(vertexBufferSize));

            // Actual vertex buffer.
            bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            bufferCI.size  = vertexBufferSize;

            allocator->CreateBuffer(m_meshes[i].vertexBuffer, bufferCI, 0u, VMA_MEMORY_USAGE_GPU_ONLY, 0u);

            // Staging index buffer.
            bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            bufferCI.size  = indexBufferSize;

            allocator->CreateBuffer(stagingIndexBuffer, bufferCI, VMA_ALLOCATION_CREATE_MAPPED_BIT, VMA_MEMORY_USAGE_CPU_ONLY, 0u);

            memcpy(stagingIndexBuffer.allocationInfo.pMappedData, buffer.data() + indexOffset, static_cast<size_t>(indexBufferSize));

            // Atual index buffer.
            bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            bufferCI.size  = indexBufferSize;

            allocator->CreateBuffer(m_meshes[i].indexBuffer, bufferCI, 0u, VMA_MEMORY_USAGE_GPU_ONLY, 0u);

            // Copies data from staging buffers to actual buffers.
            stagingVertexBuffers[i] = std::move(stagingVertexBuffer);
            stagingIndexBuffers [i] = std::move(stagingIndexBuffer);

            cmdBuffer.CopyBufferToBuffer(stagingVertexBuffers[i], m_meshes[i].vertexBuffer);
            cmdBuffer.CopyBufferToBuffer(stagingIndexBuffers [i], m_meshes[i].indexBuffer);

            // Stores data for later use.
            m_meshes[i].vertexCount = vertexCount;
            m_meshes[i].indexCount  = indexCount;
        }

        cmdBuffer.End();

        device->GetTransferQueue()->Submit(cmdBuffer.GetHandle(), fence.GetHandle());

        // Waits for the transfer operations to complete before destroying the staging buffers.
        fence.Wait();

        // The command buffer can be freed after the transfer operation has been completed.
        device->GetTransferCommandPool()->FreeCommandBuffer(cmdBuffer);

        for (size_t i = 0; i < m_meshes.size(); ++i)
        {
            // Staging buffers are temporary so they need to be freed after usage.
            allocator->DestroyBuffer(stagingVertexBuffers[i]);
            allocator->DestroyBuffer(stagingIndexBuffers [i]);

            // Setups debug info.
            Debug::SetBufferName(device->GetLogicalDevice(), m_meshes[i].vertexBuffer.handle, (m_name + "_VB_" + std::to_string(i)).c_str());
            Debug::SetBufferName(device->GetLogicalDevice(), m_meshes[i].indexBuffer .handle, (m_name + "_IB_" + std::to_string(i)).c_str());
        }

        m_isLoaded.store(true, std::memory_order_release);
    }

    else
        LOG(LogAssetManager, Error, "Model file corrupted : %s", p_path.c_str());

    m_isPending.store(false, std::memory_order_release);
}

void    Model::Serialize     (std::string const& p_path) noexcept
{
    auto const& device    = RHI::Get().GetDevice   ();
    auto const& allocator = RHI::Get().GetAllocator();

    std::string   dataPath(std::filesystem::path(m_name).replace_extension(".bin").string());

    std::ofstream asset   (p_path,                     std::ios::out);
    std::ofstream data    (ASSET_DIRECTORY + dataPath, std::ios::out | std::ios::binary);

    if (asset.is_open() && data.is_open())
    {
        // Inserts asset type as header in the file.
        asset << EAssetType::MODEL << '\n';

        std::vector<Buffer> vertexBuffers(m_meshes.size());
        std::vector<Buffer> indexBuffers (m_meshes.size());

        Json          json;
        Fence         fence;
        CommandBuffer cmdBuffer(device->GetTransferCommandPool()->AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));

        cmdBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        VkDeviceSize size = 0;

        for (size_t i = 0; i < m_meshes.size(); ++i)
        {
            VkBufferCreateInfo bufferCI     = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
            Buffer             vertexBuffer = {};
            Buffer             indexBuffer  = {};

            // Temporary vertex buffer.
            bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            bufferCI.size  = m_meshes[i].vertexBuffer.size;

            allocator->CreateBuffer(vertexBuffer, bufferCI, VMA_ALLOCATION_CREATE_MAPPED_BIT, VMA_MEMORY_USAGE_CPU_ONLY, 0u);

            // Temporary index buffer.
            bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            bufferCI.size  = m_meshes[i].indexBuffer.size;

            allocator->CreateBuffer(indexBuffer, bufferCI, VMA_ALLOCATION_CREATE_MAPPED_BIT, VMA_MEMORY_USAGE_CPU_ONLY, 0u);

            // Copies data from GPU buffers to CPU readable buffers.
            cmdBuffer.CopyBufferToBuffer(m_meshes[i].vertexBuffer, vertexBuffer);
            cmdBuffer.CopyBufferToBuffer(m_meshes[i].indexBuffer,  indexBuffer);

            // Serializes data for another use.
            json["Counts"]  += m_meshes[i].vertexCount;
            json["Counts"]  += m_meshes[i].indexCount;
            json["Offsets"] += static_cast<uint32>(size);
            json["Offsets"] += static_cast<uint32>(size + vertexBuffer.size);

            size += vertexBuffer.size;
            size += indexBuffer .size;

            vertexBuffers[i] = std::move(vertexBuffer);
            indexBuffers [i] = std::move(indexBuffer);
        }

        cmdBuffer.End();

        device->GetTransferQueue()->Submit(cmdBuffer.GetHandle(), fence.GetHandle());

        // Waits for the transfer operations to complete before destroying the temporary buffers.
        fence.Wait();

        json["Count"] = static_cast<uint32>(m_meshes.size());
        json["Size"]  = static_cast<uint32>(size);
        json["Data"]  = dataPath;

        // The command buffer can be freed after the transfer operation has been completed.
        device->GetTransferCommandPool()->FreeCommandBuffer(cmdBuffer);

        // Staging buffers are temporary so they need to be freed after usage.
        for (size_t i = 0; i < m_meshes.size(); ++i)
        {
            // Writes model's raw data to disk.
            data.write(reinterpret_cast<ANSICHAR*>(vertexBuffers[i].allocationInfo.pMappedData), vertexBuffers[i].size);
            data.write(reinterpret_cast<ANSICHAR*>(indexBuffers [i].allocationInfo.pMappedData), indexBuffers [i].size);

            allocator->DestroyBuffer(vertexBuffers[i]);
            allocator->DestroyBuffer(indexBuffers [i]);
        }

        // Serializes the json on disk.
        asset << json.dump(4);
    }

    else
        LOG(LogAssetManager, Error, "Failed to open \"%s\" or \"%s\" for serialization", p_path.c_str(), dataPath.c_str());

    for (Mesh& mesh : m_meshes)
    {
        allocator->DestroyBuffer(mesh.vertexBuffer);
        allocator->DestroyBuffer(mesh.indexBuffer);
    }

    m_isLoaded .store(false, std::memory_order_release);
    m_isPending.store(false, std::memory_order_release);
}