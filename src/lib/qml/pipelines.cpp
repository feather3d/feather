/***********************************************************************
 *
 * Filename: pipelines.cpp
 *
 * Description: holds all the pools for shaders.
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

#include "pipelines.hpp"
//#include "mesh.hpp"
//#include "pointlight.hpp"

#include "swapchain.hpp"

using namespace feather::vulkan;


// PIPELINES


Pipelines::Pipelines()
{

}

Pipelines::~Pipelines()
{
}

void Pipelines::cleanup(VkDevice device)
{
    // Clean up used Vulkan resources 
    // Note : Inherited destructor cleans up resources stored in base class

    // axis
    vkDestroyPipeline(device, m_axisPipeline.wire, nullptr);
    // grid 
    vkDestroyPipeline(device, m_gridPipeline.wire, nullptr);
    // light
    vkDestroyPipeline(device, m_lightPipeline.wire, nullptr);
    // mesh 
    vkDestroyPipeline(device, m_meshPipeline.wire, nullptr);
    vkDestroyPipeline(device, m_meshPipeline.point, nullptr);
    vkDestroyPipeline(device, m_meshPipeline.shade, nullptr);
    vkDestroyPipeline(device, m_meshPipeline.faceselect, nullptr);

    vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
}

void Pipelines::bind(VkDevice device, VkCommandBuffer buffer, Node* node, VkDeviceSize offsets[1])
{
    switch(node->type())
    {
        case Node::Null:
            bindMesh(device, buffer, node, offsets);
            break;
        case Node::Axis:
            bindAxis(device, buffer, node, offsets);
            break;
        case Node::Grid:
            bindGrid(device, buffer, node, offsets);
            break;
        case Node::Camera:
            bindMesh(device, buffer, node, offsets);
            break;
        case Node::Light:
            bindLight(device, buffer, node, offsets);
            break;
        case Node::Mesh:
            bindMesh(device, buffer, node, offsets);
            break;
        default:
            bindMesh(device, buffer, node, offsets);
    }
}


void Pipelines::bindAxis(VkDevice device, VkCommandBuffer buffer, Node* node, VkDeviceSize offsets[1])
{
    vkCmdBindVertexBuffers(buffer, VERTEX_BUFFER_BIND_ID, 1, &static_cast<Axis*>(node)->buffer()->vertices.buf, offsets);

    // EDGES

    // set line width
    vkCmdSetLineWidth(buffer, 2.0);

    // Shading
    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_axisPipeline.wire);

    // Draw indexed 
    vkCmdDrawIndexed(buffer, static_cast<Axis*>(node)->buffer()->edgeCount, 1, 0, 0, 1);

}


void Pipelines::bindGrid(VkDevice device, VkCommandBuffer buffer, Node* node, VkDeviceSize offsets[1])
{
    vkCmdBindVertexBuffers(buffer, VERTEX_BUFFER_BIND_ID, 1, &static_cast<Grid*>(node)->buffer()->vertices.buf, offsets);

    // EDGES

    // set line width
    vkCmdSetLineWidth(buffer, 1.0);

    // Shading
    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_gridPipeline.wire);

    // Draw indexed 
    vkCmdDrawIndexed(buffer, static_cast<Grid*>(node)->buffer()->edgeCount, 1, 0, 0, 1);

}


void Pipelines::bindMesh(VkDevice device, VkCommandBuffer buffer, Node* node, VkDeviceSize offsets[1])
{

    // SHADE

    vkCmdBindVertexBuffers(buffer, VERTEX_BUFFER_BIND_ID, 1, &static_cast<Mesh*>(node)->buffer()->vertices.buf, offsets);
    // Bind triangle indices
    vkCmdBindIndexBuffer(buffer, static_cast<Mesh*>(node)->buffer()->indices.buf, 0, VK_INDEX_TYPE_UINT32);

    // Bind the rendering pipeline (including the shaders)
    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_meshPipeline.shade);

    // Draw indexed triangle
    vkCmdDrawIndexed(buffer, static_cast<Mesh*>(node)->buffer()->indexCount, 1, 0, 0, 1);

    // SELECT

    // Bind triangle indices
    vkCmdBindIndexBuffer(buffer, static_cast<Mesh*>(node)->buffer()->faceSelectIndices.buf, 0, VK_INDEX_TYPE_UINT32);

    // Bind the rendering pipeline (including the shaders)
    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_meshPipeline.faceselect);
 
    // Draw indexed triangle
    vkCmdDrawIndexed(buffer, static_cast<Mesh*>(node)->buffer()->faceSelectCount, 1, 0, 0, 1);
 
   
    // EDGES

    // Bind indices
    vkCmdBindIndexBuffer(buffer, static_cast<Mesh*>(node)->buffer()->edges.buf, 0, VK_INDEX_TYPE_UINT32);

    // set line width
    vkCmdSetLineWidth(buffer, 4.0);

    // Shading
    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_meshPipeline.wire);

    // Draw indexed 
    vkCmdDrawIndexed(buffer, static_cast<Mesh*>(node)->buffer()->edgeCount, 1, 0, 0, 1);

    // POINTS

    // Shading 
    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_meshPipeline.point);

    // Draw indexed 
    vkCmdDrawIndexed(buffer, static_cast<Mesh*>(node)->buffer()->edgeCount, 1, 0, 0, 1);

}

void Pipelines::bindLight(VkDevice device, VkCommandBuffer buffer, Node* node, VkDeviceSize offsets[1])
{
    vkCmdBindVertexBuffers(buffer, VERTEX_BUFFER_BIND_ID, 1, &static_cast<PointLight*>(node)->buffer()->vertices.buf, offsets);

    // EDGES

    // set line width
    vkCmdSetLineWidth(buffer, 2.0);

    // Shading
    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_lightPipeline.wire);

    // Draw indexed 
    vkCmdDrawIndexed(buffer, static_cast<PointLight*>(node)->buffer()->edgeCount, 1, 0, 0, 1);

}


void Pipelines::createCache(VkDevice device)
{
    VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
    pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    VkResult err = vkCreatePipelineCache(device, &pipelineCacheCreateInfo, nullptr, &m_pipelineCache);
    assert(!err);
}


void Pipelines::createLayout(VkDevice device, VkPipelineLayoutCreateInfo info)
{
    VkResult err;
    err = vkCreatePipelineLayout(device, &info, nullptr, &m_pipelineLayout);
    assert(!err);
}


void Pipelines::prepare(VkDevice device, VkRenderPass renderPass, VkPipelineVertexInputStateCreateInfo* vi)
{
    // Create our rendering pipeline used in this example
    // Vulkan uses the concept of rendering pipelines to encapsulate
    // fixed states
    // This replaces OpenGL's huge (and cumbersome) state machine
    // A pipeline is then stored and hashed on the GPU making
    // pipeline changes much faster than having to set dozens of 
    // states
    // In a real world application you'd have dozens of pipelines
    // for every shader set used in a scene
    // Note that there are a few states that are not stored with
    // the pipeline. These are called dynamic states and the 
    // pipeline only stores that they are used with this pipeline,
    // but not their states


    // Solid Shading Pipeline
    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};

    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    // The layout used for this pipeline
    pipelineCreateInfo.layout = m_pipelineLayout;
    // Renderpass this pipeline is attached to
    pipelineCreateInfo.renderPass = renderPass;

    VkResult err;

    // Vertex input state for edge shading 
    // Describes the topoloy used with this pipeline
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    // This pipeline renders vertex data as line lists
    inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    //inputAssemblyState.primitiveRestartEnable = VK_FALSE;
 

    // Rasterization state
    VkPipelineRasterizationStateCreateInfo rasterizationState = {};
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    // Solid polygon mode
    rasterizationState.polygonMode = VK_POLYGON_MODE_LINE;
    // No culling
    rasterizationState.cullMode = VK_CULL_MODE_NONE;
    rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizationState.depthClampEnable = VK_TRUE;
    rasterizationState.rasterizerDiscardEnable = VK_FALSE;
    rasterizationState.depthBiasEnable = VK_FALSE;
    //rasterizationState.lineWidth = 4.0; // this does not work yet, have to enable wide lines?

    // Color blend state
    // Describes blend modes and color masks
    VkPipelineColorBlendStateCreateInfo colorBlendState = {};
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    // One blend attachment state
    // Blending is not used in this example
    VkPipelineColorBlendAttachmentState blendAttachmentState[1] = {};
    blendAttachmentState[0].colorWriteMask = 0xf;
    blendAttachmentState[0].blendEnable = VK_FALSE;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = blendAttachmentState;

    // Viewport state
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    // One viewport
    viewportState.viewportCount = 1;
    // One scissor rectangle
    viewportState.scissorCount = 1;

    // Enable dynamic states
    // Describes the dynamic states to be used with this pipeline
    // Dynamic states can be set even after the pipeline has been created
    // So there is no need to create new pipelines just for changing
    // a viewport's dimensions or a scissor box
    VkPipelineDynamicStateCreateInfo dynamicState = {};
    // The dynamic state properties themselves are stored in the command buffer
    std::vector<VkDynamicState> dynamicStateEnables;
    dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);
    dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);
    dynamicStateEnables.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pDynamicStates = dynamicStateEnables.data();
    dynamicState.dynamicStateCount = dynamicStateEnables.size();

    // Depth and stencil state
    // Describes depth and stenctil test and compare ops
    VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
    // Basic depth compare setup with depth writes and depth test enabled
    // No stencil used 
    depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilState.depthTestEnable = VK_TRUE;
    depthStencilState.depthWriteEnable = VK_TRUE;
    depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencilState.depthBoundsTestEnable = VK_FALSE;
    depthStencilState.back.failOp = VK_STENCIL_OP_KEEP;
    depthStencilState.back.passOp = VK_STENCIL_OP_KEEP;
    depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;
    depthStencilState.stencilTestEnable = VK_FALSE;
    depthStencilState.front = depthStencilState.back;

    // Multi sampling state
    VkPipelineMultisampleStateCreateInfo multisampleState = {};
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.pSampleMask = NULL;
    // No multi sampling used in this example
    multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    //multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_4_BIT;


    // Load shaders

    VkPipelineShaderStageCreateInfo shaderStages[3] = { {},{} };

    
    // Assign states to solid shading
    // Three shader stages
    pipelineCreateInfo.stageCount = 3;
    // Assign pipeline state create information
    pipelineCreateInfo.pVertexInputState = vi;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineCreateInfo.pRasterizationState = &rasterizationState;
    pipelineCreateInfo.pColorBlendState = &colorBlendState;
    pipelineCreateInfo.pMultisampleState = &multisampleState;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pDepthStencilState = &depthStencilState;
    pipelineCreateInfo.pStages = shaderStages;
    pipelineCreateInfo.renderPass = renderPass;
    pipelineCreateInfo.pDynamicState = &dynamicState;


    // WIRE

    // mesh
    shaderStages[0] = loadShader(device, "shaders/spv/wire.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shaderStages[1] = loadShader(device, "shaders/spv/wire.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
    shaderStages[2] = loadShader(device, "shaders/spv/wire.mesh.geom.spv", VK_SHADER_STAGE_GEOMETRY_BIT);
    err = vkCreateGraphicsPipelines(device, m_pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_meshPipeline.wire);
    assert(!err);

    // axis
    shaderStages[2] = loadShader(device, "shaders/spv/wire.axis.geom.spv", VK_SHADER_STAGE_GEOMETRY_BIT);
    err = vkCreateGraphicsPipelines(device, m_pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_axisPipeline.wire);
    assert(!err);

    // grid 
    shaderStages[2] = loadShader(device, "shaders/spv/wire.grid.geom.spv", VK_SHADER_STAGE_GEOMETRY_BIT);
    err = vkCreateGraphicsPipelines(device, m_pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_gridPipeline.wire);
    assert(!err);


    // light 
    shaderStages[2] = loadShader(device, "shaders/spv/wire.light.geom.spv", VK_SHADER_STAGE_GEOMETRY_BIT);
    err = vkCreateGraphicsPipelines(device, m_pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_lightPipeline.wire);
    assert(!err);


    // POINT
    pipelineCreateInfo.stageCount = 3;
    rasterizationState.polygonMode = VK_POLYGON_MODE_POINT;
    
    // mesh
    shaderStages[0] = loadShader(device, "shaders/spv/point.mesh.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shaderStages[1] = loadShader(device, "shaders/spv/point.mesh.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
    shaderStages[2] = loadShader(device, "shaders/spv/point.mesh.geom.spv", VK_SHADER_STAGE_GEOMETRY_BIT);
    err = vkCreateGraphicsPipelines(device, m_pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_meshPipeline.point);
    assert(!err);


    // SHADE
    pipelineCreateInfo.stageCount = 3;
    rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    rasterizationState.cullMode = VK_CULL_MODE_FRONT_BIT;
  
    // mesh 
    shaderStages[0] = loadShader(device, "shaders/spv/shade.mesh.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shaderStages[1] = loadShader(device, "shaders/spv/shade.mesh.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
    shaderStages[2] = loadShader(device, "shaders/spv/shade.mesh.geom.spv", VK_SHADER_STAGE_GEOMETRY_BIT);
    err = vkCreateGraphicsPipelines(device, m_pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_meshPipeline.shade);
    assert(!err);

    // MESH FACE SELECT
    pipelineCreateInfo.stageCount = 3;
    shaderStages[0] = loadShader(device, "shaders/spv/select.mesh.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shaderStages[1] = loadShader(device, "shaders/spv/select.mesh.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
    shaderStages[2] = loadShader(device, "shaders/spv/shade.mesh.geom.spv", VK_SHADER_STAGE_GEOMETRY_BIT);
    err = vkCreateGraphicsPipelines(device, m_pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_meshPipeline.faceselect);
    assert(!err);

}

VkPipelineShaderStageCreateInfo Pipelines::loadShader(VkDevice device, const char * fileName, VkShaderStageFlagBits stage)
{
    VkPipelineShaderStageCreateInfo shaderStage = {};
    shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStage.stage = stage;
    shaderStage.module = feather::vulkan::tools::loadShader(fileName, device, stage);
    shaderStage.pName = "main"; // todo : make param
    assert(shaderStage.module != NULL);
    m_shaderModules.push_back(shaderStage.module);
    return shaderStage;
}


