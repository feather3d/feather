/***********************************************************************
 *
 * Filename: debug.cpp
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

#include "debug.hpp"

namespace feather
{

    namespace vulkan
    {

        namespace debug
        {
            int validationLayerCount = 9;
            const char *validationLayerNames[] = 
            {
                "VK_LAYER_LUNARG_threading",
                "VK_LAYER_LUNARG_mem_tracker",
                "VK_LAYER_LUNARG_object_tracker",
                "VK_LAYER_LUNARG_draw_state",
                "VK_LAYER_LUNARG_param_checker",
                "VK_LAYER_LUNARG_swapchain",
                "VK_LAYER_LUNARG_device_limits",
                "VK_LAYER_LUNARG_image",
                "VK_LAYER_GOOGLE_unique_objects",
            };

            PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallback;
            PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallback;
            PFN_vkDebugReportMessageEXT dbgBreakCallback;

            VkDebugReportCallbackEXT msgCallback;

            VkBool32 messageCallback(
                    VkDebugReportFlagsEXT flags,
                    VkDebugReportObjectTypeEXT objType,
                    uint64_t srcObject,
                    size_t location,
                    int32_t msgCode,
                    const char* pLayerPrefix,
                    const char* pMsg,
                    void* pUserData)
            {
                char *message = (char *)malloc(strlen(pMsg) + 100);

                assert(message);

                if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
                {
                    std::cout << "ERROR: " << "[" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg << "\n";
                }
                else
                    if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
                    {
                        // Uncomment to see warnings
                        //std::cout << "WARNING: " << "[" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg << "\n";
                    }
                    else
                    {
                        return false;
                    }

                fflush(stdout);

                free(message);
                return false;
            }

            void setupDebugging(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportCallbackEXT callBack)
            {
                CreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
                DestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
                dbgBreakCallback = (PFN_vkDebugReportMessageEXT)vkGetInstanceProcAddr(instance, "vkDebugReportMessageEXT");

                VkDebugReportCallbackCreateInfoEXT dbgCreateInfo;
                dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
                dbgCreateInfo.pNext = NULL;
                dbgCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)messageCallback;
                dbgCreateInfo.pUserData = NULL;
                dbgCreateInfo.flags = flags;
                VkDebugReportCallbackEXT debugReportCallback;
                VkResult err = CreateDebugReportCallback(
                        instance,
                        &dbgCreateInfo,
                        NULL,
                        &debugReportCallback);
                assert(!err);


            }

            void freeDebugCallback(VkInstance instance)
            {
                if (msgCallback != NULL)
                {
                    DestroyDebugReportCallback(instance, msgCallback, nullptr);
                }
            }

        } // namespace debug

    } // namespace vulkan

} // namespace feather
