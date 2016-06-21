/***********************************************************************
 *
 * Filename: pointlight.cpp
 *
 * Description: vulkan point light node.
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

#include "pointlight.hpp"

using namespace feather::vulkan;

PointLight::PointLight(uint32_t _id) : Node(Node::Light,_id)
{

}

PointLight::~PointLight()
{

}

void PointLight::build()
{
    m_vertexBuffer.clear();
    m_indexBuffer.clear();
    m_vertexBuffer.push_back({{1.0f,1.0f,0.0f},{0.0f,0.0f,1.0f},{0.0f,0.0f},{1.0f,0.0f,0.0f},{1,0,0,m_id}});
    m_vertexBuffer.push_back({{-1.0f,1.0f,0.0f},{0.0f,0.0f,1.0f},{0.0f,0.0f},{1.0f,1.0f,0.0f},{2,0,0,m_id}});
    m_vertexBuffer.push_back({{-1.0f,1.0f,3.0f},{0.0f,0.0f,1.0f},{0.0f,0.0f},{0.0f,0.0f,1.0f},{3,0,0,m_id}});
    m_vertexBuffer.push_back({{1.0f,1.0f,3.0f},{0.0f,0.0f,1.0f},{0.0f,0.0f},{0.0f,0.0f,1.0f},{4,0,0,m_id}});
    m_indexBuffer = {{0},{1},{2},{0},{2},{3}};
    m_edgeBuffer = {{0},{1},{1},{2},{2},{3},{3},{0}};
}

void PointLight::prepareVertices(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties)
{
    build();

    buildVertex(device, deviceMemoryProperties);
    buildIndex(device, deviceMemoryProperties);
    buildEdge(device, deviceMemoryProperties);
}

void PointLight::updateVertices(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties, float step)
{
    // free the vertex buffer
    vkDestroyBuffer(device, m_pMeshBuffer->vertices.buf, nullptr);
    vkFreeMemory(device, m_pMeshBuffer->vertices.mem, nullptr);

    //build();

    m_vertexBuffer.at(0)={{1.0f*step,1.0f*step,0.0f},{0.0f,0.0f,1.0f},{0.0f,0.0f},{1.0f,0.0f,0.0f},{1,3,3,m_id}};
    buildVertex(device, deviceMemoryProperties);
}

