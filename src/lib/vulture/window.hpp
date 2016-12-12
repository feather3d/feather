/***********************************************************************
 *
 * Filename: window.hpp
 *
 * Description: base vulkan window.
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

#ifndef VULKAN_HPP
#define VULKAN_HPP

#include "deps.hpp"
#include "vulkan_deps.hpp"
#include "swapchain.hpp"
#include "textureloader.hpp"
#include "axis.hpp"
#include "grid.hpp"
#include "mesh.hpp"
#include "pointlight.hpp"
#include "pipelines.hpp"

#define VERTEX_BUFFER_BIND_ID 0
#define ENABLE_VALIDATION false
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

// feather
#include "types.hpp"
#include "status.hpp"

//#include "parameter.hpp"
//#include "field.hpp"

namespace feather
{

    namespace vulkan
    {
        struct {
            bool left = false;
            bool right = false;
        } m_mouseButtons;

        // Synchronization semaphores
        struct {
            VkSemaphore presentComplete;
            VkSemaphore renderComplete;
        } m_semaphores;

        struct 
        {
            VkImage image;
            VkDeviceMemory mem;
            VkImageView view;
        } m_depthStencil;

        // buf and mem are in the Node
        struct {
            VkPipelineVertexInputStateCreateInfo vi;
            std::vector<VkVertexInputBindingDescription> bindingDescriptions;
            std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
        } m_vertices;

        /*
        struct uniformData {
            VkBuffer buffer;
            VkDeviceMemory memory;
            VkDescriptorBufferInfo descriptor;
        } m_uniformDataVS, m_uniformDataGS;
        */

        struct {
            glm::mat4 projection;
            glm::mat4 model;
            int mode;
            // current selected components
            int p1;
            int p2;
            int p3;
            int object;
        } m_uboVS, m_uboGS;

        // selection testing
        struct 
        {
            VkImage image;
            VkDeviceMemory mem;
            VkImageView view;
        } m_selection;


        class Window
        {

            public:
                enum Mode { POINT=0x0001, WIREFRAME=0x0002, SHADED=0x0004, POINT_NORMALS=0x0008, FACE_NORMALS=0x0010 };
 
                //Window(xcb_window_t window=0, xcb_connection_t* connection=nullptr, std::string _title="", unsigned int _width=1280, unsigned int _height=720, float _zoom=-2.0, bool _validation=false);
                Window(std::string _title="", unsigned int _width=1280, unsigned int _height=720, float _zoom=-2.0, bool _validation=false);
                ~Window();
                std::vector<unsigned int> cameras();
                unsigned int current_camera();
                void set_camera(unsigned int uid);
                //xcb_window_t window() { return m_window; };
                //VkInstance& instance() { return m_instance; };
                //VkSurfaceKHR& surface() { return m_swapChain.get_surface(); };

            private:
                std::string m_title;
                unsigned int m_width;
                unsigned int m_height;
                float m_zoom;
                bool m_validation;
                bool m_prepared;
                bool m_quit;
                bool m_updateRender;
                feather::vulkan::tools::VulkanTextureLoader* m_pTextureLoader;
                glm::vec3 m_rotation;
                glm::vec2 m_mousePos;
                // Active frame buffer index
                uint32_t m_currentBuffer;
                float m_frameTimer;
	
                xcb_connection_t* m_pConnection;
                xcb_screen_t* m_pScreen;
                xcb_window_t m_window;
                xcb_intern_atom_reply_t* m_pAtomDeleteWindow;
                void handleEvent(const xcb_generic_event_t *event);

                /*
                struct {
                    bool left = false;
                    bool right = false;
                } m_mouseButtons;
                */

                void initConnection();
                void initVulkan(bool validation);
                xcb_window_t setupWindow();
                void initSwapChain();
                void prepare();
                void renderLoop();

                void getErrorCode(VkResult code);

                // Prepare - setup vulkan 
                void createCommandPool();
                void createSetupCommandBuffer();
                void setupSwapChain();
                void createCommandBuffers();
                void setupDepthStencil();
                void setupSelection();
                void setupRenderPass();
                void setupFrameBuffer();
                void flushSetupCommandBuffer();

                // Prepare - setup data
                void prepareSemaphore();
                void prepareVertices();
                VkBool32 createBuffer(VkBufferUsageFlags usage, VkDeviceSize size, void * data, VkBuffer *buffer, VkDeviceMemory *memory);
                VkBool32 createBuffer(VkBufferUsageFlags usage, VkDeviceSize size, void * data, VkBuffer *buffer, VkDeviceMemory *memory, VkDescriptorBufferInfo * descriptor);
                void prepareUniformBuffers();
                void setupDescriptorSetLayout();
                void setupDescriptorPool();
                void setupDescriptorSet();
                void buildCommandBuffers();

                void updateUniformBuffers();
                void updateNodeBuffers();

                void keyPressed(uint32_t keyCode);
                void viewChanged();
                void set_selection();
                void nodeChanged();
                void render();
                void draw();

                // VULKAN MEMBERS
                VkPhysicalDevice m_physicalDevice;
                VkPhysicalDeviceMemoryProperties m_deviceMemoryProperties;
                VkDevice m_device;
                VkQueue m_queue;
                std::vector<VkQueueFamilyProperties> m_queueProps;
                uint32_t m_queueCount;
                VkInstance m_instance;
                // Contains command buffers and semaphores to be presented to the queue
                VkSubmitInfo m_submitInfo;
                // Pipeline stage flags for the submit info structure
                VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
                // Depth format is selected during Vulkan initialization
                VkFormat m_depthFormat;
                VulkanSwapChain m_swapChain;
                VkCommandPool m_commandPool;
		VkCommandBuffer m_setupCommandBuffer;
                std::vector<VkCommandBuffer> m_drawCommandBuffers;
                // Command buffer for submitting a post present image barrier
                VkCommandBuffer m_postPresentCommandBuffer;
                // Command buffer for submitting a pre present image barrier
                VkCommandBuffer m_prePresentCommandBuffer;
                // Color buffer format
                VkFormat m_colorFormat;
                // Global render pass for framebuffer writes
                VkRenderPass m_renderPass;
                // Pipeline cache object
                VkPipelineCache m_pipelineCache;
                VkPipelineLayout m_pipelineLayout;
                VkDescriptorSet m_descriptorSet;
                VkDescriptorSetLayout m_descriptorSetLayout;
                VkDescriptorPool m_descriptorPool;
                // List of available frame buffers (same as number of swap chain images)
                std::vector<VkFramebuffer> m_aFrameBuffers;
                // List of shader modules created (stored for cleanup)
                //std::vector<VkShaderModule> m_shaderModules;
                VkClearColorValue m_defaultClearColor;

                VkResult createInstance(bool enabled);
                VkResult createDevice(VkDeviceQueueCreateInfo requestedQueues, bool validation);
                VkBool32 getMemoryType(uint32_t typeBits, VkFlags properties, uint32_t *typeIndex);


                /*
                // Synchronization semaphores
                struct {
                    VkSemaphore presentComplete;
                    VkSemaphore renderComplete;
                } m_semaphores;

                struct 
                {
                    VkImage image;
                    VkDeviceMemory mem;
                    VkImageView view;
                } m_depthStencil;

                // buf and mem are in the Node
                struct {
                    VkPipelineVertexInputStateCreateInfo vi;
                    std::vector<VkVertexInputBindingDescription> bindingDescriptions;
                    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
                } m_vertices;

                struct uniformData {
                    VkBuffer buffer;
                    VkDeviceMemory memory;
                    VkDescriptorBufferInfo descriptor;
                } m_uniformDataVS, m_uniformDataGS;

                struct {
                    glm::mat4 projection;
                    glm::mat4 model;
                    int mode;
                    // current selected components
                    int p1;
                    int p2;
                    int p3;
                    int object;
                } m_uboVS, m_uboGS;
                */

                Pipelines* m_pPipelines;

                std::vector<Node*> m_aNodes;

                /*
                // selection testing
                struct 
                {
                    VkImage image;
                    VkDeviceMemory mem;
                    VkImageView view;
                } m_selection;
                */

                void* m_selectionData;

                // TESTING
                float step=1.0;
                unsigned int m_mode;
        };

    } // namespace vulkan

} // namespace feather

#endif
