/***********************************************************************
 *
 * Filename: node.cpp
 *
 * Description: vulkan base node.
 *
 * Copyright (C) 2016 Richard Layman, rlayman2000@yahoo.com 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "node.hpp"

using namespace feather::vulkan;

Node::Node(Node::Type _type, uint32_t _id) : m_id(_id),m_type(_type)
{
    m_pMeshBuffer = new MeshBuffer();
}


Node::~Node()
{

}

uint32_t Node::face(uint32_t p1, uint32_t p2, uint32_t p3)
{
    // Start at 0 and skip by three until a match is found.
    // The fourth number is id
    // For now we are using known face ids but later the
    // mesh will be crossed reference.
    for(int i=0; i < m_faceIds.size(); i+=4){
        if(
                m_faceIds[i] == (p1-1) 
                && m_faceIds[i+1] == (p2-1)
                && m_faceIds[i+2] == (p3-1)
          )
            return m_faceIds[i + 3];
    }

    return 0;    
}


void Node::set_selection(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties, uint32_t face)
{
    std::cout << "set selection " << face << std::endl;

    // if face id is 0, there is no face selected
    if(!face)
    {
        // If I clear out the faceSelectBuffer it will crash the program
        /*
        // rebuild the indics buffer
        vkDestroyBuffer(device, m_pMeshBuffer->indices.buf, nullptr);
        vkFreeMemory(device, m_pMeshBuffer->indices.mem, nullptr);

        // rebuild the indics buffer
        vkDestroyBuffer(device, m_pMeshBuffer->faceSelectIndices.buf, nullptr);
        vkFreeMemory(device, m_pMeshBuffer->faceSelectIndices.mem, nullptr);

        m_indexBuffer.clear();
        m_faceSelectBuffer.clear();

        for(int i=0; i < m_faceIds.size(); i+=3){
            m_indexBuffer.push_back(m_faceIds[i]);
            m_indexBuffer.push_back(m_faceIds[i+1]);
            m_indexBuffer.push_back(m_faceIds[i+2]);
        }

        buildIndex(device, deviceMemoryProperties);
        buildFaceSelect(device, deviceMemoryProperties);
        */
        return;
    }

    // rebuild the indics buffer
    vkDestroyBuffer(device, m_pMeshBuffer->indices.buf, nullptr);
    vkFreeMemory(device, m_pMeshBuffer->indices.mem, nullptr);

    // rebuild the indics buffer
    vkDestroyBuffer(device, m_pMeshBuffer->faceSelectIndices.buf, nullptr);
    vkFreeMemory(device, m_pMeshBuffer->faceSelectIndices.mem, nullptr);

    // rebuilt both the index and selection buffers
    m_indexBuffer.clear();
    m_faceSelectBuffer.clear();
    for(int i=3; i < m_faceIds.size(); i+=4){
        if(m_faceIds[i] == face){
            m_faceSelectBuffer.push_back(m_faceIds[i-3]);
            m_faceSelectBuffer.push_back(m_faceIds[i-2]);
            m_faceSelectBuffer.push_back(m_faceIds[i-1]);
        }else{
            m_indexBuffer.push_back(m_faceIds[i-3]);
            m_indexBuffer.push_back(m_faceIds[i-2]);
            m_indexBuffer.push_back(m_faceIds[i-1]);
        }
    }
    
    // rebuild the indics buffer
    //vkDestroyBuffer(device, m_pMeshBuffer->indices.buf, nullptr);
    //vkFreeMemory(device, m_pMeshBuffer->indices.mem, nullptr);
    buildIndex(device, deviceMemoryProperties);
 
    // rebuild the indics buffer
    //vkDestroyBuffer(device, m_pMeshBuffer->faceSelectIndices.buf, nullptr);
    //vkFreeMemory(device, m_pMeshBuffer->faceSelectIndices.mem, nullptr);
    buildFaceSelect(device, deviceMemoryProperties);
}


void Node::set_selection(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties, uint32_t p1, uint32_t p2, uint32_t p3)
{
    uint32_t id = face(p1,p2,p3);
    set_selection(device, deviceMemoryProperties, id);
}


void Node::freeBuffer(VkDevice device)
{
    // vertices
    vkDestroyBuffer(device, m_pMeshBuffer->vertices.buf, nullptr);
    vkFreeMemory(device, m_pMeshBuffer->vertices.mem, nullptr);
    // indices
    vkDestroyBuffer(device, m_pMeshBuffer->indices.buf, nullptr);
    vkFreeMemory(device, m_pMeshBuffer->indices.mem, nullptr);
}

VkBool32 Node::getMemoryType(VkPhysicalDeviceMemoryProperties deviceMemoryProperties, uint32_t typeBits, VkFlags properties, uint32_t *typeIndex)
{
    for (uint32_t i = 0; i < 32; i++)
    {
        if ((typeBits & 1) == 1)
        {
            if ((deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    return false;
}

void Node::buildVertex(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties)
{
    int vertexBufferSize = m_vertexBuffer.size() * sizeof(Vertex);

    VkMemoryAllocateInfo memAlloc = {};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAlloc.pNext = NULL;
    memAlloc.allocationSize = 0;
    memAlloc.memoryTypeIndex = 0;
    VkMemoryRequirements memReqs;

    VkResult err;
    void *data;

    // Generate vertex buffer
    //	Setup
    VkBufferCreateInfo bufInfo = {};
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.pNext = NULL;
    bufInfo.size = vertexBufferSize;
    bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufInfo.flags = 0;
    //	Copy vertex data to VRAM
    //memset(&m_vertices, 0, sizeof(m_vertices));
    err = vkCreateBuffer(device, &bufInfo, nullptr, &m_pMeshBuffer->vertices.buf);
    assert(!err);
    vkGetBufferMemoryRequirements(device, m_pMeshBuffer->vertices.buf, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    getMemoryType(deviceMemoryProperties, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &memAlloc.memoryTypeIndex);
    err = vkAllocateMemory(device, &memAlloc, nullptr, &m_pMeshBuffer->vertices.mem);
    assert(!err);
    err = vkMapMemory(device, m_pMeshBuffer->vertices.mem, 0, memAlloc.allocationSize, 0, &data);
    assert(!err);
    memcpy(data, m_vertexBuffer.data(), vertexBufferSize);
    vkUnmapMemory(device, m_pMeshBuffer->vertices.mem);
    err = vkBindBufferMemory(device, m_pMeshBuffer->vertices.buf, m_pMeshBuffer->vertices.mem, 0);
    assert(!err);
}

void Node::buildIndex(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties)
{
    // Setup indices
    int indexBufferSize = m_indexBuffer.size() * sizeof(uint32_t);

    VkMemoryAllocateInfo memAlloc = {};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAlloc.pNext = NULL;
    memAlloc.allocationSize = 0;
    memAlloc.memoryTypeIndex = 0;
    VkMemoryRequirements memReqs;

    VkResult err;
    void *data;

    // Generate index buffer
    //	Setup
    VkBufferCreateInfo indexbufferInfo = {};
    indexbufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    indexbufferInfo.pNext = NULL;
    indexbufferInfo.size = indexBufferSize;
    indexbufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    indexbufferInfo.flags = 0;
    // Copy index data to VRAM
    //memset(&m_indices, 0, sizeof(m_indices));
    err = vkCreateBuffer(device, &indexbufferInfo, nullptr, &m_pMeshBuffer->indices.buf);
    assert(!err);
    vkGetBufferMemoryRequirements(device, m_pMeshBuffer->indices.buf, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    getMemoryType(deviceMemoryProperties, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &memAlloc.memoryTypeIndex);
    err = vkAllocateMemory(device, &memAlloc, nullptr, &m_pMeshBuffer->indices.mem);
    assert(!err);
    err = vkMapMemory(device, m_pMeshBuffer->indices.mem, 0, indexBufferSize, 0, &data);
    assert(!err);
    memcpy(data, m_indexBuffer.data(), indexBufferSize);
    vkUnmapMemory(device, m_pMeshBuffer->indices.mem);
    err = vkBindBufferMemory(device, m_pMeshBuffer->indices.buf, m_pMeshBuffer->indices.mem, 0);
    assert(!err);
    m_pMeshBuffer->indexCount = m_indexBuffer.size();

}

void Node::buildFaceSelect(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties)
{
    // Setup indices
    int indexBufferSize = m_faceSelectBuffer.size() * sizeof(uint32_t);

    VkMemoryAllocateInfo memAlloc = {};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAlloc.pNext = NULL;
    memAlloc.allocationSize = 0;
    memAlloc.memoryTypeIndex = 0;
    VkMemoryRequirements memReqs;

    VkResult err;
    void *data;

    // Generate index buffer
    //	Setup
    VkBufferCreateInfo indexbufferInfo = {};
    indexbufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    indexbufferInfo.pNext = NULL;
    indexbufferInfo.size = indexBufferSize;
    indexbufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    indexbufferInfo.flags = 0;
    // Copy index data to VRAM
    //memset(&m_indices, 0, sizeof(m_indices));
    err = vkCreateBuffer(device, &indexbufferInfo, nullptr, &m_pMeshBuffer->faceSelectIndices.buf);
    assert(!err);
    vkGetBufferMemoryRequirements(device, m_pMeshBuffer->faceSelectIndices.buf, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    getMemoryType(deviceMemoryProperties, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &memAlloc.memoryTypeIndex);
    err = vkAllocateMemory(device, &memAlloc, nullptr, &m_pMeshBuffer->faceSelectIndices.mem);
    assert(!err);
    err = vkMapMemory(device, m_pMeshBuffer->faceSelectIndices.mem, 0, indexBufferSize, 0, &data);
    assert(!err);
    memcpy(data, m_faceSelectBuffer.data(), indexBufferSize);
    vkUnmapMemory(device, m_pMeshBuffer->faceSelectIndices.mem);
    err = vkBindBufferMemory(device, m_pMeshBuffer->faceSelectIndices.buf, m_pMeshBuffer->faceSelectIndices.mem, 0);
    assert(!err);
    m_pMeshBuffer->faceSelectCount = m_indexBuffer.size();
}

void Node::buildEdge(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties)
{
    // Setup indices
    int edgeBufferSize = m_edgeBuffer.size() * sizeof(uint32_t);

    VkMemoryAllocateInfo memAlloc = {};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAlloc.pNext = NULL;
    memAlloc.allocationSize = 0;
    memAlloc.memoryTypeIndex = 0;
    VkMemoryRequirements memReqs;

    VkResult err;
    void *data;

    // Generate edge buffer
    //	Setup
    VkBufferCreateInfo edgebufferInfo = {};
    edgebufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    edgebufferInfo.pNext = NULL;
    edgebufferInfo.size = edgeBufferSize;
    edgebufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    edgebufferInfo.flags = 0;
    // Copy edge data to VRAM
    //memset(&m_indices, 0, sizeof(m_indices));
    err = vkCreateBuffer(device, &edgebufferInfo, nullptr, &m_pMeshBuffer->edges.buf);
    assert(!err);
    vkGetBufferMemoryRequirements(device, m_pMeshBuffer->edges.buf, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    getMemoryType(deviceMemoryProperties, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &memAlloc.memoryTypeIndex);
    err = vkAllocateMemory(device, &memAlloc, nullptr, &m_pMeshBuffer->edges.mem);
    assert(!err);
    err = vkMapMemory(device, m_pMeshBuffer->edges.mem, 0, edgeBufferSize, 0, &data);
    assert(!err);
    memcpy(data, m_edgeBuffer.data(), edgeBufferSize);
    vkUnmapMemory(device, m_pMeshBuffer->edges.mem);
    err = vkBindBufferMemory(device, m_pMeshBuffer->edges.buf, m_pMeshBuffer->edges.mem, 0);
    assert(!err);
    m_pMeshBuffer->edgeCount = m_edgeBuffer.size();
}

