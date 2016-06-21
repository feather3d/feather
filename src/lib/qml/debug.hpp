/***********************************************************************
 *
 * Filename: debug.hpp
 *
 * Description: vulkan debug code, based on Vulkan example code.
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

#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "deps.hpp"
#include "vulkan_deps.hpp"
#include "textureloader.hpp"

namespace feather
{

    namespace vulkan
    {

        namespace debug
        {
            // Default validation layers
            extern int validationLayerCount;
            extern const char *validationLayerNames[];

            // Default debug callback
            VkBool32 messageCallback(
                    VkDebugReportFlagsEXT flags,
                    VkDebugReportObjectTypeEXT objType,
                    uint64_t srcObject,
                    size_t location,
                    int32_t msgCode,
                    const char* pLayerPrefix,
                    const char* pMsg,
                    void* pUserData);

            // Load debug function pointers and set debug callback
            // if callBack is NULL, default message callback will be used
            void setupDebugging(
                    VkInstance instance, 
                    VkDebugReportFlagsEXT flags, 
                    VkDebugReportCallbackEXT callBack);
            // Clear debug callback
            void freeDebugCallback(VkInstance instance);

        } // namespace debug

    } // namespace vulkan

} // namespace feather

#endif
