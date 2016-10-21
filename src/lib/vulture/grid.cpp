/***********************************************************************
 *
 * Filename: grid.cpp
 *
 * Description: vulkan grid.
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

#include "grid.hpp"

using namespace feather::vulkan;

Grid::Grid(uint32_t _id) : Node(Node::Grid,_id)
{

}

Grid::~Grid()
{

}

void Grid::build()
{
    m_vertexBuffer.clear();
    m_indexBuffer.clear();
    m_edgeBuffer.clear();
    m_vertexBuffer.push_back({{10.0f,0.0f,10.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f,0.0f},{0,0,0,0}});
    m_vertexBuffer.push_back({{10.0f,0.0f,-10.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f,0.0f},{0,0,0,0}});
    m_vertexBuffer.push_back({{-10.0f,0.0f,-10.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f,0.0f},{0,0,0,0}});
    m_edgeBuffer = {{0},{1},{0},{2}};
}

void Grid::prepareVertices(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties)
{
    build();

    buildVertex(device, deviceMemoryProperties);
    buildIndex(device, deviceMemoryProperties);
    buildEdge(device, deviceMemoryProperties);
}

void Grid::updateVertices(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties, float step)
{

}

void Grid::loadShaders()
{

}
