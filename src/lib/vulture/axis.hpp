/***********************************************************************
 *
 * Filename: axis.hpp
 *
 * Description: vulkan axis node.
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

#ifndef AXIS_HPP
#define AXIS_HPP

#include "deps.hpp"
#include "vulkan_deps.hpp"
#include "node.hpp"

namespace feather
{

    namespace vulkan
    {

        class Axis : public Node
        {
            public:
                Axis(uint32_t _id);
                ~Axis();
                void prepareVertices(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties);
                void updateVertices(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties, float step=1.0);
                void build();
                void loadShaders();
        };

    } // namespace vulkan

} // namespace feather

#endif
