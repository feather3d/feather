/***********************************************************************
 *
 * Filename: mesh.cpp
 *
 * Description: vulkan mesh node.
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

#include "mesh.hpp"

using namespace feather::vulkan;

Mesh::Mesh(uint32_t _id) : Node(Node::Mesh,_id)
{

}

Mesh::~Mesh()
{

}

void Mesh::build()
{
    m_vertexBuffer.clear();
    m_indexBuffer.clear();
    m_faceSelectBuffer.clear();
    
    // vertex layout
    // [position] [normal] [uv] [color] [ids]
    // id layout
    // [r,g,b,a]
    // r = point
    // g = reserved
    // b = reserved
    // a = object
    // 
    // The mouse will return a vec4 layouted out at [r,g,b,a]
    // r = p1
    // g = p2
    // b = p3
    // a = object
    // This is used to tell if the mouse is looking at a point, edge, or face.
    // If the mouse is over a point, the only two valid numbers will be p1 and object. [5,0,0,120] (point 5 in object 120) 
    // If the mouse is over a edge, the only valid numbers are p1, p2 and object. [5,6,0,120]
    // if the mouse is over a face, all the numbers are valid. [5,6,7,120]
    // These values are set in the geometry shader and will pass on if the object is selected to the fragment shader.

    // front
    m_vertexBuffer.push_back({{1.0f,2.0f,0.0f},{0.0f,0.0f,1.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f},{1,0,0,m_id}});
    m_vertexBuffer.push_back({{-1.0f,2.0f,0.0f},{0.0f,0.0f,1.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f},{2,0,0,m_id}});
    m_vertexBuffer.push_back({{-1.0f,0.0f,0.0f},{0.0f,0.0f,1.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f},{3,0,0,m_id}});
    m_vertexBuffer.push_back({{1.0f,0.0f,0.0f},{0.0f,0.0f,1.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f},{4,0,0,m_id}});

    // back
    m_vertexBuffer.push_back({{1.0f,2.0f,-2.0f},{0.0f,0.0f,-1.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f},{5,0,0,m_id}});
    m_vertexBuffer.push_back({{-1.0f,2.0f,-2.0f},{0.0f,0.0f,-1.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f},{6,0,0,m_id}});
    m_vertexBuffer.push_back({{-1.0f,0.0f,-2.0f},{0.0f,0.0f,-1.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f},{7,0,0,m_id}});
    m_vertexBuffer.push_back({{1.0f,0.0f,-2.0f},{0.0f,0.0f,-1.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f},{8,0,0,m_id}});


    // We can get the face id by using the returned point value matched up with the index value.
    // The mouse will return the point value of the first index in it's list.
    // As long as we make sure that none of the 
    m_indexBuffer = {
        // front face
        //{0},{1},{2},
        //{0},{2},{3},
        // back face
        {5},{4},{7},
        {5},{7},{6},
        // left face
        {4},{0},{3},
        {4},{3},{7},
        // right face
        {1},{5},{6},
        {1},{6},{2},
        // top face
        {4},{5},{1},
        {4},{1},{0},
        // bottom face
        {6},{7},{3},
        {6},{3},{2}
    };

    // You need to added a selected face at the beginning
    // of else the selected face will appear transparent
    m_faceSelectBuffer = {
        // front face
        {0},{1},{2},
        {0},{2},{3}
    };

    m_edgeBuffer = {
        // front edges 
        {0},{1},
        {1},{2},
        {2},{3},
        {3},{0},
        // back edges 
        {4},{5},
        {5},{6},
        {6},{7},
        {7},{4},
        // left edges 
        {4},{0},
        {7},{3},
        // right edges
        {1},{5},
        {2},{6}
    };

    m_faceIds = {
        // front face
        {0},{1},{2},{1},
        {0},{2},{3},{1},
        //back face
        {5},{4},{7},{2},
        {5},{7},{6},{2},
        // left face
        {4},{0},{3},{3},
        {4},{3},{7},{3},
        // right face
        {1},{5},{6},{4},
        {1},{6},{2},{4},
        // top face
        {4},{5},{1},{5},
        {4},{1},{0},{5},
        // bottom face
        {6},{7},{3},{6},
        {6},{3},{2},{6}
    };


}

void Mesh::prepareVertices(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties)
{
    build();

    buildVertex(device, deviceMemoryProperties);
    buildIndex(device, deviceMemoryProperties);
    buildFaceSelect(device, deviceMemoryProperties);
    buildEdge(device, deviceMemoryProperties);
}

void Mesh::updateVertices(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties, float step)
{
    // free the vertex buffer
    vkDestroyBuffer(device, m_pMeshBuffer->vertices.buf, nullptr);
    vkFreeMemory(device, m_pMeshBuffer->vertices.mem, nullptr);

    //build();

    m_vertexBuffer.at(0)={{1.0f*step,2.0f*step,0.0f},{0.0f,0.0f,1.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f},{1,0,0,m_id}};
    buildVertex(device, deviceMemoryProperties);
}

