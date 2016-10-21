/***********************************************************************
 *
 * Filename: pipelines.hpp
 *
 * Description: holds all the pipelines for shaders.
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

#ifndef PIPELINES_HPP
#define PIPELINES_HPP

#include "deps.hpp"
#include "vulkan_deps.hpp"
#include "node.hpp"
#include "axis.hpp"
#include "grid.hpp"
#include "mesh.hpp"
#include "pointlight.hpp"


#define VERTEX_BUFFER_BIND_ID 0

namespace feather
{

    namespace vulkan
    {

        class Pipelines
        {
            public:
                Pipelines();
                ~Pipelines();
                void cleanup(VkDevice device);
                void bind(VkDevice device, VkCommandBuffer buffer, Node* node, VkDeviceSize offset[1]);
                void prepare(VkDevice device, VkRenderPass renderPass, VkPipelineVertexInputStateCreateInfo* vi);
                void createCache(VkDevice device);
                void createLayout(VkDevice device, VkPipelineLayoutCreateInfo info);
                VkPipelineLayout layout() { return m_pipelineLayout; };

            private:
                VkPipelineCache m_pipelineCache;
                VkPipelineLayout m_pipelineLayout;

                // bind axis
                void bindAxis(VkDevice device, VkCommandBuffer buffer, Node* node, VkDeviceSize offsets[1]);
                // bind grid 
                void bindGrid(VkDevice device, VkCommandBuffer buffer, Node* node, VkDeviceSize offsets[1]);
                // bind mesh
                void bindMesh(VkDevice device, VkCommandBuffer buffer, Node* node, VkDeviceSize offsets[1]);
                // bind light
                void bindLight(VkDevice device, VkCommandBuffer buffer, Node* node, VkDeviceSize offsets[1]);

                VkPipelineShaderStageCreateInfo loadShader(VkDevice device, const char * fileName, VkShaderStageFlagBits stage);

                std::vector<VkShaderModule> m_shaderModules;

                struct {
                    VkPipeline wire;
                } m_axisPipeline;

                struct {
                    VkPipeline wire;
                } m_gridPipeline;

                struct {
                    VkPipeline wire;
                    VkPipeline point;
                    VkPipeline shade;
                    VkPipeline faceselect;
                } m_meshPipeline;

                struct {
                    VkPipeline wire;
                } m_lightPipeline;

        };

    } // namespace vulkan

} // namespace feather

#endif
