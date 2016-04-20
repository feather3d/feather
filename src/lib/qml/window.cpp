/***********************************************************************
 *
 * Filename: window.hpp
 *
 * Description: base vulkan window.
 *
 * Copyright (C) 2015 Richard Layman, rlayman2000@yahoo.com 
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

#include "window.hpp"
#include "debug.hpp"

// KEYCODES
#define KEY_c 0x36
#define KEY_space 0x41

using namespace feather::vulkan;

Window::Window(unsigned int _width, unsigned int _height, float _zoom, bool _validation, QWindow* parent) : QWindow(parent),
//m_title(_title),
m_width(_width),
m_height(_height),
m_zoom(_zoom),
m_validation(_validation),
m_prepared(false),
m_pTextureLoader(nullptr),
m_rotation(glm::vec3()),
m_currentBuffer(0),
m_frameTimer(1.0),
m_setupCommandBuffer(VK_NULL_HANDLE),
m_postPresentCommandBuffer(VK_NULL_HANDLE),
m_prePresentCommandBuffer(VK_NULL_HANDLE),
m_colorFormat(VK_FORMAT_R8G8B8A8_UNORM),
//m_colorFormat(VK_FORMAT_B8G8R8A8_SRGB),
//m_colorFormat(VK_FORMAT_R32G32B32A32_SFLOAT),
m_defaultClearColor({ { 0.325f, 0.325f, 0.325f, 1.0f } })
{
    // set the view mode
    m_mode = POINT | WIREFRAME | SHADED;
 
    // add the nodes
    m_aNodes.push_back(new Axis(0));
    m_aNodes.push_back(new Grid(1));
    m_aNodes.push_back(new Mesh(2));
    m_aNodes.push_back(new PointLight(3));

    // setup    
    initConnection();
    initVulkan(m_validation);

    m_pPipelines = new Pipelines();

    // startup
    setupWindow();
    initSwapChain();
    prepare();
    renderLoop();
}


Window::~Window()
{
    // Clean up used Vulkan resources 
    // Note : Inherited destructor cleans up resources stored in base class
    m_pPipelines->cleanup(m_device);
    delete m_pPipelines;
    m_pPipelines=0;

    vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, nullptr);

    // go through each node and clean it up
    for(auto node : m_aNodes) {
        node->freeBuffer(m_device);
        switch(node->type())
        {
            case Node::Axis:
                delete static_cast<Axis*>(node);
                break;
            case Node::Grid:
                delete static_cast<Grid*>(node);
                break;
            case Node::Mesh:
                delete static_cast<Mesh*>(node);
                break;
            case Node::Light:
                delete static_cast<PointLight*>(node); 
                break;
        }
    }

    //vkMeshLoader::freeMeshBufferResources(device, &meshes.object);

    // destroy vert unifom data
    vkDestroyBuffer(m_device, m_uniformDataVS.buffer, nullptr);
    vkFreeMemory(m_device, m_uniformDataVS.memory, nullptr);

    // destroy geom unifom data
    vkDestroyBuffer(m_device, m_uniformDataGS.buffer, nullptr);
    vkFreeMemory(m_device, m_uniformDataGS.memory, nullptr);
}


std::vector<unsigned int> Window::cameras()
{
    return std::vector<unsigned int>();
}


unsigned int Window::current_camera()
{
    return 0;
}


void Window::set_camera(unsigned int uid)
{

}


void Window::initConnection()
{
    const xcb_setup_t *setup;
    xcb_screen_iterator_t iter;
    int scr;

    // get connection
    m_pConnection = xcb_connect(NULL, &scr);
    if (m_pConnection == NULL) {
        printf("Could not find a compatible Vulkan ICD!\n");
        fflush(stdout);
        exit(1);
    }

    setup = xcb_get_setup(m_pConnection);
    iter = xcb_setup_roots_iterator(setup);
    while (scr-- > 0)
        xcb_screen_next(&iter);
    m_pScreen = iter.data;
}


void Window::initVulkan(bool validation)
{
    VkResult err;

    // Vulkan instance
    err = createInstance(validation);
    if (err)
    {
        feather::vulkan::tools::exitFatal("Could not create Vulkan instance : \n" + feather::vulkan::tools::errorString(err), "Fatal error");
    }

    // Physical device
    uint32_t gpuCount = 0;
    // Get number of available physical devices
    err = vkEnumeratePhysicalDevices(m_instance, &gpuCount, nullptr);
    assert(!err);		
    assert(gpuCount > 0);
    // Enumerate devices
    std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
    err = vkEnumeratePhysicalDevices(m_instance, &gpuCount, physicalDevices.data());
    if (err)
    {
        feather::vulkan::tools::exitFatal("Could not enumerate phyiscal devices : \n" + feather::vulkan::tools::errorString(err), "Fatal error");
    }

    std::cout << "Vulkan GPU count: " << gpuCount << std::endl;

    // Note : 
    // This example will always use the first physical device reported, 
    // change the vector index if you have multiple Vulkan devices installed 
    // and want to use another one
    m_physicalDevice = physicalDevices[0];

    // Find a queue that supports graphics operations
    uint32_t graphicsQueueIndex = 0;
    //uint32_t m_queueCount;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &m_queueCount, NULL);
    assert(m_queueCount >= 1);

    //std::vector<VkQueueFamilyProperties> m_queueProps;
    m_queueProps.resize(m_queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &m_queueCount, m_queueProps.data());

    for (graphicsQueueIndex = 0; graphicsQueueIndex < m_queueCount; graphicsQueueIndex++)
    {
        if (m_queueProps[graphicsQueueIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            break;
    }
    assert(graphicsQueueIndex < m_queueCount);

    // Vulkan device
    std::array<float, 1> queuePriorities = { 0.0f };
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphicsQueueIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = queuePriorities.data();

    err = createDevice(queueCreateInfo, validation);
    assert(!err);

    // Gather physical device memory properties
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_deviceMemoryProperties);

    // Get the graphics queue
    vkGetDeviceQueue(m_device, graphicsQueueIndex, 0, &m_queue);

    // Find a suitable depth format
    VkBool32 validDepthFormat = feather::vulkan::tools::getSupportedDepthFormat(m_physicalDevice, &m_depthFormat);
    assert(validDepthFormat);

    m_swapChain.connect(m_instance, m_physicalDevice, m_device);

    // Create synchronization objects
    VkSemaphoreCreateInfo semaphoreCreateInfo = feather::vulkan::tools::initializers::semaphoreCreateInfo();
    // Create a semaphore used to synchronize image presentation
    // Ensures that the image is displayed before we start submitting new commands to the queu
    err = vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_semaphores.presentComplete);
    assert(!err);
    // Create a semaphore used to synchronize command submission
    // Ensures that the image is not presented until all commands have been sumbitted and executed
    err = vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_semaphores.renderComplete);
    assert(!err);

    // Set up submit info structure
    // Semaphores will stay the same during application lifetime
    // Command buffer submission info is set by each example
    m_submitInfo = feather::vulkan::tools::initializers::submitInfo();
    m_submitInfo.pWaitDstStageMask = &submitPipelineStages;
    m_submitInfo.waitSemaphoreCount = 1;
    m_submitInfo.pWaitSemaphores = &m_semaphores.presentComplete;
    m_submitInfo.signalSemaphoreCount = 1;
    m_submitInfo.pSignalSemaphores = &m_semaphores.renderComplete;
}


xcb_window_t Window::setupWindow()
{
    uint32_t value_mask, value_list[32];

    m_window = xcb_generate_id(m_pConnection);

    value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    value_list[0] = m_pScreen->black_pixel;
    value_list[1] = 
        XCB_EVENT_MASK_KEY_RELEASE |
        XCB_EVENT_MASK_EXPOSURE |
        XCB_EVENT_MASK_STRUCTURE_NOTIFY |
        XCB_EVENT_MASK_POINTER_MOTION |
        XCB_EVENT_MASK_BUTTON_PRESS |
        XCB_EVENT_MASK_BUTTON_RELEASE;

    xcb_create_window(m_pConnection,
            XCB_COPY_FROM_PARENT,
            m_window, m_pScreen->root,
            0, 0, m_width, m_height, 0,
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            m_pScreen->root_visual,
            value_mask, value_list);


    // send notification when the window is destroyed
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(m_pConnection, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(m_pConnection, cookie, 0);

    xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(m_pConnection, 0, 16, "WM_DELETE_WINDOW");
    m_pAtomDeleteWindow = xcb_intern_atom_reply(m_pConnection, cookie2, 0);

    xcb_change_property(
            m_pConnection,
            XCB_PROP_MODE_REPLACE,
            m_window,
            (*reply).atom,
            4,
            32,
            1,
            &(*m_pAtomDeleteWindow).atom
            );

    xcb_change_property(
            m_pConnection,
            XCB_PROP_MODE_REPLACE,
            m_window,
            XCB_ATOM_WM_NAME,
            XCB_ATOM_STRING,
            8,
            0,//m_title.size(),
            ""//m_title.c_str()
            );

    free(reply);

    xcb_map_window(m_pConnection, m_window);

    return(m_window);
}

VkResult Window::createInstance(bool enabled)
{
    m_validation = enabled;

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    //appInfo.pApplicationName = m_title.c_str();
    //appInfo.pEngineName = m_title.c_str();
    // Temporary workaround for drivers not supporting SDK 1.0.3 upon launch
    // todo : Use VK_API_VERSION 
    appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 2);

    std::vector<const char*> enabledExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };

    enabledExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);

    // todo : check if all extensions are present

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = NULL;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    if (enabledExtensions.size() > 0)
    {
        if (m_validation)
        {
            enabledExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }
        instanceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
    }
    if (m_validation)
    {
        instanceCreateInfo.enabledLayerCount = feather::vulkan::debug::validationLayerCount; // todo : change validation layer names!
        instanceCreateInfo.ppEnabledLayerNames = feather::vulkan::debug::validationLayerNames;
    }
    return vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);
}

VkResult Window::createDevice(VkDeviceQueueCreateInfo requestedQueues, bool validation)
{
    std::vector<const char*> enabledExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = NULL;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &requestedQueues;
    deviceCreateInfo.pEnabledFeatures = NULL;

    if (enabledExtensions.size() > 0)
    {
        deviceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
    }
    if (validation)
    {
        deviceCreateInfo.enabledLayerCount = feather::vulkan::debug::validationLayerCount; // todo : validation layer names
        deviceCreateInfo.ppEnabledLayerNames = feather::vulkan::debug::validationLayerNames;
    }

    return vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device);
}

void Window::initSwapChain()
{
    m_swapChain.initSurface(m_pConnection, m_window);
}

void Window::prepare()
{
    if (m_validation)
    {
        feather::vulkan::debug::setupDebugging(m_instance, VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT, NULL);
    }

    // Setup Vulkan
    createCommandPool();
    createSetupCommandBuffer();
    setupSwapChain();
    createCommandBuffers();
    setupDepthStencil();
    setupSelection();
    setupRenderPass();
    m_pPipelines->createCache(m_device);
    //createPipelineCache();
    setupFrameBuffer();
    flushSetupCommandBuffer();
    // Recreate setup command buffer for derived class
    createSetupCommandBuffer();
    // Create a simple texture loader class 
    m_pTextureLoader = new feather::vulkan::tools::VulkanTextureLoader(m_physicalDevice, m_device, m_queue, m_commandPool);

    // Setup Data

    bool prep=true;

    prepareSemaphore();
    prepareVertices();
    prepareUniformBuffers();
    setupDescriptorSetLayout();
    m_pPipelines->prepare(m_device, m_renderPass, &m_vertices.vi);
    setupDescriptorPool();
    setupDescriptorSet();
    buildCommandBuffers();

    m_prepared = prep;
}

void Window::createCommandPool()
{
    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.queueFamilyIndex = m_swapChain.queueNodeIndex;
    cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VkResult vkRes = vkCreateCommandPool(m_device, &cmdPoolInfo, nullptr, &m_commandPool);
    assert(!vkRes);
}

void Window::createSetupCommandBuffer()
{
    if (m_setupCommandBuffer != VK_NULL_HANDLE)
    {
        vkFreeCommandBuffers(m_device, m_commandPool, 1, &m_setupCommandBuffer);
        m_setupCommandBuffer = VK_NULL_HANDLE; // todo : check if still necessary
    }

    VkCommandBufferAllocateInfo cmdBufAllocateInfo =
        feather::vulkan::tools::initializers::commandBufferAllocateInfo(
                m_commandPool,
                VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                1);

    VkResult vkRes = vkAllocateCommandBuffers(m_device, &cmdBufAllocateInfo, &m_setupCommandBuffer);
    assert(!vkRes);

    // todo : Command buffer is also started here, better put somewhere else
    // todo : Check if necessaray at all...
    VkCommandBufferBeginInfo cmdBufInfo = {};
    cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    // todo : check null handles, flags?

    vkRes = vkBeginCommandBuffer(m_setupCommandBuffer, &cmdBufInfo);
    assert(!vkRes);
}

void Window::setupSwapChain()
{
    m_swapChain.create(m_setupCommandBuffer, &m_width, &m_height);
}

void Window::createCommandBuffers()
{
    // Create one command buffer per frame buffer 
    // in the swap chain
    // Command buffers store a reference to the 
    // frame buffer inside their render pass info
    // so for static usage withouth having to rebuild 
    // them each frame, we use one per frame buffer
    m_drawCommandBuffers.resize(m_swapChain.imageCount);

    VkCommandBufferAllocateInfo cmdBufAllocateInfo = 
        feather::vulkan::tools::initializers::commandBufferAllocateInfo(
                m_commandPool,
                VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                (uint32_t)m_drawCommandBuffers.size());

    VkResult vkRes = vkAllocateCommandBuffers(m_device, &cmdBufAllocateInfo, m_drawCommandBuffers.data());
    assert(!vkRes);

    // Command buffers for submitting present barriers
    cmdBufAllocateInfo.commandBufferCount = 1;
    // Pre present
    vkRes = vkAllocateCommandBuffers(m_device, &cmdBufAllocateInfo, &m_prePresentCommandBuffer);
    assert(!vkRes);
    // Post present
    vkRes = vkAllocateCommandBuffers(m_device, &cmdBufAllocateInfo, &m_postPresentCommandBuffer);
    assert(!vkRes);
}

void Window::setupDepthStencil()
{
    VkImageCreateInfo image = {};
    image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image.pNext = NULL;
    image.imageType = VK_IMAGE_TYPE_2D;
    image.format = m_depthFormat;
    image.extent = { m_width, m_height, 1 };
    image.mipLevels = 2;
    image.arrayLayers = 1;
    image.samples = VK_SAMPLE_COUNT_1_BIT; // changing this to 4_BIT removed the depth stencil making objects behind another one appear in front
    image.tiling = VK_IMAGE_TILING_OPTIMAL;
    image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    image.flags = 0;

    VkMemoryAllocateInfo mem_alloc = {};
    mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_alloc.pNext = NULL;
    mem_alloc.allocationSize = 0;
    mem_alloc.memoryTypeIndex = 0;

    VkImageViewCreateInfo depthStencilView = {};
    depthStencilView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    depthStencilView.pNext = NULL;
    depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthStencilView.format = m_depthFormat;
    depthStencilView.flags = 0;
    depthStencilView.subresourceRange = {};
    depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    depthStencilView.subresourceRange.baseMipLevel = 0;
    depthStencilView.subresourceRange.levelCount = 1;
    depthStencilView.subresourceRange.baseArrayLayer = 0;
    depthStencilView.subresourceRange.layerCount = 1;

    VkMemoryRequirements memReqs;
    VkResult err;

    err = vkCreateImage(m_device, &image, nullptr, &m_depthStencil.image);
    assert(!err);
    vkGetImageMemoryRequirements(m_device, m_depthStencil.image, &memReqs);
    mem_alloc.allocationSize = memReqs.size;
    getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &mem_alloc.memoryTypeIndex);
    err = vkAllocateMemory(m_device, &mem_alloc, nullptr, &m_depthStencil.mem);
    assert(!err);

    err = vkBindImageMemory(m_device, m_depthStencil.image, m_depthStencil.mem, 0);
    assert(!err);
    feather::vulkan::tools::setImageLayout(m_setupCommandBuffer, m_depthStencil.image, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    depthStencilView.image = m_depthStencil.image;
    err = vkCreateImageView(m_device, &depthStencilView, nullptr, &m_depthStencil.view);
    assert(!err);

}


void Window::setupSelection()
{

    VkImageCreateInfo isci{
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            nullptr,
            VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT,
            VK_IMAGE_TYPE_2D,
            //VK_FORMAT_R8G8B8A8_UINT,
            VK_FORMAT_R32G32B32A32_UINT,
            //m_colorFormat,
            {
                m_width,
                m_height,
                1
            },
            1,
            1,
            VK_SAMPLE_COUNT_1_BIT,
            VK_IMAGE_TILING_LINEAR,//VK_IMAGE_TILING_OPTIMAL should be used but does not make a full image, have to look into this more.
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
            VK_SHARING_MODE_EXCLUSIVE,
            1,
            &m_queueCount//&m_swapChain.queueNodeIndex//&m_queueCount//&queueFamily
    };
    VkResult errorCode = vkCreateImage( m_device, &isci, nullptr, &m_selection.image);
    //RESULT_HANDLER( errorCode, "vkCreateImage" );
    assert(!errorCode);

    VkMemoryRequirements ismr;
    vkGetImageMemoryRequirements( m_device, m_selection.image, &ismr );

    uint32_t memoryType = 0;
    bool found = false;

    for( uint32_t i = 0; i < 32; ++i ){
        if(  ( ismr.memoryTypeBits & (0x1 << i) )  &&  ( m_deviceMemoryProperties.memoryTypes[i].propertyFlags & ( VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ) ) == ( VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ) ){
            memoryType = i; found = true; break;
        }
    }

    if( !found )
        throw "Can't find compatible mappable memory for image";

    VkMemoryAllocateInfo memoryInfo{
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            nullptr,
            ismr.size,
            memoryType
    };
    errorCode = vkAllocateMemory( m_device, &memoryInfo, nullptr, &m_selection.mem);
    //RESULT_HANDLER( errorCode, "vkAllocateMemory" );
    assert(!errorCode);

    errorCode = vkBindImageMemory( m_device, m_selection.image, m_selection.mem, 0 );
    //RESULT_HANDLER( errorCode, "vkBindImageMemory" );
    assert(!errorCode);

    VkImageViewCreateInfo isvci{
        VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            nullptr,
            0,
            m_selection.image,
            VK_IMAGE_VIEW_TYPE_2D,
            //VK_FORMAT_R8G8B8A8_UNORM,
            VK_FORMAT_R32G32B32A32_UINT,
            {
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY
            },
            {
                VK_IMAGE_ASPECT_COLOR_BIT,
                0,
                VK_REMAINING_MIP_LEVELS,
                0,
                VK_REMAINING_ARRAY_LAYERS
            }
    };
    errorCode = vkCreateImageView( m_device, &isvci, nullptr, &m_selection.view);
    //RESULT_HANDLER( errorCode, "vkCreateImageView" );
    assert(!errorCode);


    /*
    VkFramebuffer framebufferSource;
    VkFramebufferCreateInfo fsci{
        VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            nullptr,
            0,
            m_renderPass,
            1,
            &m_selection.view,
            m_width,
            m_height,
            1
    };
    errorCode = vkCreateFramebuffer( m_device, &fsci, nullptr, &framebufferSource );
    //RESULT_HANDLER( errorCode, "vkCreateFramebuffer" );
    assert(!errorCode);
    */


    /*
    VkImageCreateInfo image = {};
    image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image.pNext = NULL;
    image.imageType = VK_IMAGE_TYPE_2D;
    image.format = VK_FORMAT_R8G8B8A8_UINT;
    image.extent = { m_width, m_height, 1 };
    image.mipLevels = 2;
    image.arrayLayers = 1;
    //image.samples = VK_SAMPLE_COUNT_1_BIT;
    image.samples = VK_SAMPLE_COUNT_4_BIT;
    image.tiling = VK_IMAGE_TILING_OPTIMAL;
    image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    image.flags = 0;

    VkMemoryAllocateInfo mem_alloc = {};
    mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_alloc.pNext = NULL;
    mem_alloc.allocationSize = 0;
    mem_alloc.memoryTypeIndex = 0;

    VkImageViewCreateInfo depthStencilView = {};
    depthStencilView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    depthStencilView.pNext = NULL;
    depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthStencilView.format = m_depthFormat;
    depthStencilView.flags = 0;
    depthStencilView.subresourceRange = {};
    depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    depthStencilView.subresourceRange.baseMipLevel = 0;
    depthStencilView.subresourceRange.levelCount = 1;
    depthStencilView.subresourceRange.baseArrayLayer = 0;
    depthStencilView.subresourceRange.layerCount = 1;

    VkMemoryRequirements memReqs;
    VkResult err;

    err = vkCreateImage(m_device, &image, nullptr, &m_depthStencil.image);
    assert(!err);
    vkGetImageMemoryRequirements(m_device, m_depthStencil.image, &memReqs);
    mem_alloc.allocationSize = memReqs.size;
    getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &mem_alloc.memoryTypeIndex);
    err = vkAllocateMemory(m_device, &mem_alloc, nullptr, &m_depthStencil.mem);
    assert(!err);

    err = vkBindImageMemory(m_device, m_depthStencil.image, m_depthStencil.mem, 0);
    assert(!err);
    feather::vulkan::tools::setImageLayout(m_setupCommandBuffer, m_depthStencil.image, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    depthStencilView.image = m_depthStencil.image;
    err = vkCreateImageView(m_device, &depthStencilView, nullptr, &m_depthStencil.view);
    assert(!err);
    */
}


VkBool32 Window::getMemoryType(uint32_t typeBits, VkFlags properties, uint32_t *typeIndex)
{
    for (uint32_t i = 0; i < 32; i++)
    {
        if ((typeBits & 1) == 1)
        {
            if ((m_deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    return false;
}

void Window::setupRenderPass()
{
    VkAttachmentDescription attachments[3];
    attachments[0].format = m_colorFormat;
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    //attachments[0].samples = VK_SAMPLE_COUNT_4_BIT;
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    attachments[1].format = m_depthFormat;
    attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
    //attachments[1].samples = VK_SAMPLE_COUNT_4_BIT;
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // ADDED - START
    attachments[2].format = VK_FORMAT_R32G32B32A32_UINT;
    attachments[2].samples = VK_SAMPLE_COUNT_1_BIT;
    //attachments[2].samples = VK_SAMPLE_COUNT_4_BIT;
    attachments[2].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[2].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[2].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachments[2].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    // ADDED - END

    VkAttachmentReference colorReference = {};
    colorReference.attachment = 0;
    colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthReference = {};
    depthReference.attachment = 1;
    depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // ADDED - START
    VkAttachmentReference selectionReference = {};
    selectionReference.attachment = 2;
    selectionReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    // ADDED - END

    VkAttachmentReference colorAttachments[2];

    colorAttachments[0] = colorReference;
    colorAttachments[1] = selectionReference;


    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.flags = 0;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = NULL;
    //subpass.colorAttachmentCount = 1;
    //subpass.pColorAttachments = &colorReference;
    subpass.colorAttachmentCount = 2;
    subpass.pColorAttachments = colorAttachments;
    subpass.pResolveAttachments = NULL;
    subpass.pDepthStencilAttachment = &depthReference;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = NULL;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.pNext = NULL;
    renderPassInfo.attachmentCount = 3;
    renderPassInfo.pAttachments = attachments;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 0;
    renderPassInfo.pDependencies = NULL;

    VkResult err;

    err = vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass);
    assert(!err);
}

void Window::setupFrameBuffer()
{
    VkImageView attachments[3];

    // Depth/Stencil attachment is the same for all frame buffers
    attachments[1] = m_depthStencil.view;
    attachments[2] = m_selection.view;

    VkFramebufferCreateInfo frameBufferCreateInfo = {};
    frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frameBufferCreateInfo.pNext = NULL;
    frameBufferCreateInfo.renderPass = m_renderPass;
    frameBufferCreateInfo.attachmentCount = 3;
    frameBufferCreateInfo.pAttachments = attachments;
    frameBufferCreateInfo.width = m_width;
    frameBufferCreateInfo.height = m_height;
    frameBufferCreateInfo.layers = 1;

    // Create frame buffers for every swap chain image
    m_aFrameBuffers.resize(m_swapChain.imageCount);
    for (uint32_t i = 0; i < m_aFrameBuffers.size(); i++)
    {
        attachments[0] = m_swapChain.buffers[i].view;
        VkResult err = vkCreateFramebuffer(m_device, &frameBufferCreateInfo, nullptr, &m_aFrameBuffers[i]);
        assert(!err);
    }
}

void Window::flushSetupCommandBuffer()
{
    VkResult err;

    if (m_setupCommandBuffer == VK_NULL_HANDLE)
        return;

    err = vkEndCommandBuffer(m_setupCommandBuffer);
    assert(!err);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_setupCommandBuffer;

    err = vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE);
    assert(!err);

    err = vkQueueWaitIdle(m_queue);
    assert(!err);

    vkFreeCommandBuffers(m_device, m_commandPool, 1, &m_setupCommandBuffer);
    m_setupCommandBuffer = VK_NULL_HANDLE; // todo : check if still necessary
}


void Window::prepareSemaphore()
{
    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = NULL;

    // This semaphore ensures that the image is complete
    // before starting to submit again
    VkResult err = vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_semaphores.presentComplete);
    assert(!err);

    // This semaphore ensures that all commands submitted
    // have been finished before submitting the image to the queue
    err = vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_semaphores.renderComplete);
    assert(!err);
}

void Window::prepareVertices()
{
    for(auto node : m_aNodes) {
        switch(node->type())
        {
            case Node::Axis:
                static_cast<Axis*>(node)->prepareVertices(m_device,m_deviceMemoryProperties);
                break;
            case Node::Grid:
                static_cast<Grid*>(node)->prepareVertices(m_device,m_deviceMemoryProperties);
                break;
            case Node::Mesh:
                static_cast<Mesh*>(node)->prepareVertices(m_device,m_deviceMemoryProperties);
                break;
            case Node::Light:
                static_cast<PointLight*>(node)->prepareVertices(m_device,m_deviceMemoryProperties);
                break;
        }

        // Binding description
        m_vertices.bindingDescriptions.resize(1);
        m_vertices.bindingDescriptions[0].binding = VERTEX_BUFFER_BIND_ID;
        m_vertices.bindingDescriptions[0].stride = sizeof(Vertex);
        m_vertices.bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        // Attribute descriptions
        // Describes memory layout and shader positions
        m_vertices.attributeDescriptions.resize(5);
        // Location 0 : Position
        m_vertices.attributeDescriptions[0].binding = VERTEX_BUFFER_BIND_ID;
        m_vertices.attributeDescriptions[0].location = 0;
        m_vertices.attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        m_vertices.attributeDescriptions[0].offset = 0;
        m_vertices.attributeDescriptions[0].binding = 0;

        // Location 1 : Normal
        m_vertices.attributeDescriptions[1].binding = VERTEX_BUFFER_BIND_ID;
        m_vertices.attributeDescriptions[1].location = 1;
        m_vertices.attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        m_vertices.attributeDescriptions[1].offset = sizeof(float) * 3;
        m_vertices.attributeDescriptions[1].binding = 0;

        // Location 2 : Texture coordinates
        m_vertices.attributeDescriptions[2].binding = VERTEX_BUFFER_BIND_ID;
        m_vertices.attributeDescriptions[2].location = 2;
        m_vertices.attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        m_vertices.attributeDescriptions[2].offset = sizeof(float) * 6;
        m_vertices.attributeDescriptions[2].binding = 0;

        // Location 3 : Color
        m_vertices.attributeDescriptions[3].binding = VERTEX_BUFFER_BIND_ID;
        m_vertices.attributeDescriptions[3].location = 3;
        m_vertices.attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
        m_vertices.attributeDescriptions[3].offset = sizeof(float) * 8;
        m_vertices.attributeDescriptions[3].binding = 0;

        // This is just for testing now but the end idea is to have each color
        // component to represent a selection component
        // r = object id
        // g = face id
        // b = edge id
        // a = point id
        // since all these are 32 bit, each component should have plenty to
        // work with
        // Location 4 : Selection Id 
        m_vertices.attributeDescriptions[4].binding = VERTEX_BUFFER_BIND_ID;
        m_vertices.attributeDescriptions[4].location = 4;
        m_vertices.attributeDescriptions[4].format = VK_FORMAT_R32G32B32A32_UINT;
        m_vertices.attributeDescriptions[4].offset = sizeof(float) * 11;
        m_vertices.attributeDescriptions[4].binding = 0;
 
        // Assign to vertex buffer
        m_vertices.vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        m_vertices.vi.pNext = NULL;
        m_vertices.vi.vertexBindingDescriptionCount = m_vertices.bindingDescriptions.size();
        m_vertices.vi.pVertexBindingDescriptions = m_vertices.bindingDescriptions.data();
        m_vertices.vi.vertexAttributeDescriptionCount = m_vertices.attributeDescriptions.size();
        m_vertices.vi.pVertexAttributeDescriptions = m_vertices.attributeDescriptions.data();
    }
}


VkBool32 Window::createBuffer(VkBufferUsageFlags usage, VkDeviceSize size, void * data, VkBuffer *buffer, VkDeviceMemory *memory)
{
    // Prepare and initialize uniform buffer containing shader uniforms
    VkMemoryRequirements memReqs;
    
    VkMemoryAllocateInfo memAlloc = tools::initializers::memoryAllocateInfo();
    VkBufferCreateInfo bufferCreateInfo = tools::initializers::bufferCreateInfo(usage, size);

    // Create a new buffer
     VkResult err = vkCreateBuffer(m_device, &bufferCreateInfo, nullptr, buffer);
    assert(!err);
    // Get memory requirements including size, alignment and memory type 
     vkGetBufferMemoryRequirements(m_device, *buffer, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    // Gets the appropriate memory type for this type of buffer allocation
    // Only memory types that are visible to the host
     getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &memAlloc.memoryTypeIndex);
    // Allocate memory for the uniform buffer
     err = vkAllocateMemory(m_device, &memAlloc, nullptr, memory);
    assert(!err);
    if (data != nullptr)
    {
        void *mapped;
        err = vkMapMemory(m_device, *memory, 0, size, 0, &mapped);
        assert(!err);
        memcpy(mapped, data, size);
        vkUnmapMemory(m_device, *memory);
    }
    // Bind memory to buffer
     err = vkBindBufferMemory(m_device, *buffer, *memory, 0);
    assert(!err);
    return true;
}


VkBool32 Window::createBuffer(VkBufferUsageFlags usage, VkDeviceSize size, void * data, VkBuffer * buffer, VkDeviceMemory * memory, VkDescriptorBufferInfo * descriptor)
{
	VkBool32 res = createBuffer(usage, size, data, buffer, memory);
	if (res)
	{
		descriptor->offset = 0;
		descriptor->buffer = *buffer;
		descriptor->range = size;
		return true;
	}
	else
	{
		return false;
	}
}


void Window::prepareUniformBuffers()
{
    // Vertex shader uniform buffer block
    createBuffer(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            sizeof(m_uboVS),
            &m_uboVS,
            &m_uniformDataVS.buffer,
            &m_uniformDataVS.memory,
            &m_uniformDataVS.descriptor);

    // Geometry shader uniform buffer block
    createBuffer(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            sizeof(m_uboGS),
            &m_uboGS,
            &m_uniformDataGS.buffer,
            &m_uniformDataGS.memory,
            &m_uniformDataGS.descriptor);

    updateUniformBuffers();
}

void Window::updateUniformBuffers()
{

    void* data;
    VkResult err = vkMapMemory(m_device, m_selection.mem, 0, VK_WHOLE_SIZE, 0, &data);
    assert(!err);
    //std::ofstream ofs("/home/richard/out.data",std::ostream::binary);
    //ofs.write((char*)data, m_width * m_height * 4);
    uint32_t offset = (m_mousePos.y * (m_width*4)) + (m_mousePos.x * 4);
    uint32_t r = ((uint32_t*)data)[offset];
    uint32_t g = ((uint32_t*)data)[offset+1];
    uint32_t b = ((uint32_t*)data)[offset+2];
    uint32_t a = ((uint32_t*)data)[offset+3];

    glm::mat4 viewMatrix = glm::mat4();

    // Update matrices
    m_uboVS.projection = glm::perspective(glm::radians(60.0f), (float)m_width / (float)m_height, 0.1f, 256.0f);

    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, m_zoom));

    m_uboVS.model = glm::mat4();
    //m_uboVS.model = glm::scale(m_uboVS.model, glm::vec3(1,-1,1)); // flip the view
    m_uboVS.model = viewMatrix * glm::translate(m_uboVS.model, glm::vec3(0,0,0));
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    m_uboVS.p1 = r;
    m_uboVS.p2 = g;
    m_uboVS.p3 = b;
    m_uboVS.object = a;

    //m_uboVS.point_mode = (m_mode && POINT) ? true : false;
    //m_uboVS.point_mode = true;
    //m_uboVS.wireframe_mode = (m_mode && WIREFRAME) ? true : false;
    m_uboVS.mode = m_mode;

    // Map uniform buffer and update it
    uint8_t *pData;
    err = vkMapMemory(m_device, m_uniformDataVS.memory, 0, sizeof(m_uboVS), 0, (void **)&pData);
    assert(!err);
    memcpy(pData, &m_uboVS, sizeof(m_uboVS));
    vkUnmapMemory(m_device, m_uniformDataVS.memory);

    /*
    // Geometry shader
    m_uboGS.model = m_uboVS.model;
    m_uboGS.projection = m_uboVS.projection;
    m_uboGS.mode = m_uboVS.mode;
    err = vkMapMemory(m_device, m_uniformDataGS.memory, 0, sizeof(m_uboGS), 0, (void **)&pData);
    assert(!err);
    memcpy(pData, &m_uboGS, sizeof(m_uboGS));
    vkUnmapMemory(m_device, m_uniformDataGS.memory);
    */

    /* 
    void* data;
    err = vkMapMemory(m_device, m_selection.mem, 0, VK_WHOLE_SIZE, 0, &data);
    assert(!err);
    //std::ofstream ofs("/home/richard/out.data",std::ostream::binary);
    //ofs.write((char*)data, m_width * m_height * 4);
    uint32_t offset = (m_mousePos.y * (m_width*4)) + (m_mousePos.x * 4);
    uint32_t r = ((uint32_t*)data)[offset];
    uint32_t g = ((uint32_t*)data)[offset+1];
    uint32_t b = ((uint32_t*)data)[offset+2];
    uint32_t a = ((uint32_t*)data)[offset+3];
    */

    /*
    uint32_t r = color & 0x000000ff;
    uint32_t g = (color >> 8) & 0x000000ff;
    uint32_t b = (color >> 16) & 0x000000ff;
    uint32_t a = (color >> 24) & 0x000000ff;
    */
    std::cout << " r=" << r << " g=" << g << " b=" << b << " a=" << a << std::endl;
    vkUnmapMemory(m_device, m_selection.mem);

    // Geometry shader
    m_uboGS.model = m_uboVS.model;
    m_uboGS.projection = m_uboVS.projection;
    m_uboGS.mode = m_uboVS.mode;
    m_uboGS.p1 = r;
    m_uboGS.p2 = g;
    m_uboGS.p3 = b;
    m_uboGS.object = a;

    err = vkMapMemory(m_device, m_uniformDataGS.memory, 0, sizeof(m_uboGS), 0, (void **)&pData);
    assert(!err);
    memcpy(pData, &m_uboGS, sizeof(m_uboGS));
    vkUnmapMemory(m_device, m_uniformDataGS.memory);

    set_selection();
}


void Window::updateNodeBuffers()
{
    // Map uniform buffer and update it
    uint8_t *pData;
    VkResult err = vkMapMemory(m_device, m_uniformDataVS.memory, 0, sizeof(m_uboVS), 0, (void **)&pData);
    assert(!err);
    memcpy(pData, &m_uboVS, sizeof(m_uboVS));
    vkUnmapMemory(m_device, m_uniformDataVS.memory);

    assert(!err);
}


void Window::setupDescriptorSetLayout()
{

    // Setup layout of descriptors used in this example
    // Basically connects the different shader stages to descriptors
    // for binding uniform buffers, image samplers, etc.
    // So every shader binding should map to one descriptor set layout
    // binding

    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings =
    {
        // Binding 0 : Vertex shader ubo
        tools::initializers::descriptorSetLayoutBinding(
                VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                VK_SHADER_STAGE_VERTEX_BIT,
                0),
        // Binding 1 : Geometry shader ubo
        tools::initializers::descriptorSetLayoutBinding(
                VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                VK_SHADER_STAGE_GEOMETRY_BIT,
                1)
    };

    VkDescriptorSetLayoutCreateInfo descriptorLayout =
        tools::initializers::descriptorSetLayoutCreateInfo(
                setLayoutBindings.data(),
                setLayoutBindings.size());

    VkResult err = vkCreateDescriptorSetLayout(m_device, &descriptorLayout, nullptr, &m_descriptorSetLayout);
    assert(!err);

    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo =
        tools::initializers::pipelineLayoutCreateInfo(
                &m_descriptorSetLayout,
                1);

    m_pPipelines->createLayout(m_device, pPipelineLayoutCreateInfo);
}

void Window::setupDescriptorPool()
{
    // uses two ubos
    std::vector<VkDescriptorPoolSize> poolSizes =
    {
        tools::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2),
    };

    VkDescriptorPoolCreateInfo descriptorPoolInfo =
        tools::initializers::descriptorPoolCreateInfo(
                poolSizes.size(),
                poolSizes.data(),
                2);

    VkResult vkRes = vkCreateDescriptorPool(m_device, &descriptorPoolInfo, nullptr, &m_descriptorPool);
    assert(!vkRes);

    /*
    // We need to tell the API the number of max. requested descriptors per type
    VkDescriptorPoolSize typeCounts[1];
    // This example only uses one descriptor type (uniform buffer) and only
    // requests one descriptor of this type
    typeCounts[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    typeCounts[0].descriptorCount = 1;
    // For additional types you need to add new entries in the type count list
    // E.g. for two combined image samplers :
    // typeCounts[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    // typeCounts[1].descriptorCount = 2;

    // Create the global descriptor pool
    // All descriptors used in this example are allocated from this pool
    VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.pNext = NULL;
    descriptorPoolInfo.poolSizeCount = 1;
    descriptorPoolInfo.pPoolSizes = typeCounts;
    // Set the max. number of sets that can be requested
    // Requesting descriptors beyond maxSets will result in an error
    descriptorPoolInfo.maxSets = 1;

    VkResult vkRes = vkCreateDescriptorPool(m_device, &descriptorPoolInfo, nullptr, &m_descriptorPool);
    assert(!vkRes);
    */
}

void Window::setupDescriptorSet()
{
    // Update descriptor sets determining the shader binding points
    // For every binding point used in a shader there needs to be one
    // descriptor set matching that binding point

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_descriptorSetLayout;

    /* 
    VkDescriptorSetAllocateInfo allocInfo =
        tools::initializers::descriptorSetAllocateInfo(
                m_descriptorPool,
                &m_descriptorSetLayout,
                1);
    */

    VkResult vkRes = vkAllocateDescriptorSets(m_device, &allocInfo, &m_descriptorSet);
    assert(!vkRes);

    std::vector<VkWriteDescriptorSet> writeDescriptorSets =
    {
        // Binding 0 : Vertex shader shader ubo
        tools::initializers::writeDescriptorSet(
                m_descriptorSet,
                VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                0,
                &m_uniformDataVS.descriptor),
        // Binding 1 : Geometry shader ubo
        tools::initializers::writeDescriptorSet(
                m_descriptorSet,
                VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                1,
                &m_uniformDataGS.descriptor)
    };

    vkUpdateDescriptorSets(m_device, writeDescriptorSets.size(), writeDescriptorSets.data(), 0, NULL);

    /*
    VkWriteDescriptorSet writeDescriptorSet = {};

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_descriptorSetLayout;

    VkResult vkRes = vkAllocateDescriptorSets(m_device, &allocInfo, &m_descriptorSet);
    assert(!vkRes);

    // Binding 0 : Uniform buffer
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = m_descriptorSet;
    writeDescriptorSet.descriptorCount = 1;
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescriptorSet.pBufferInfo = &m_uniformDataVS.descriptor;
    // Binds this uniform buffer to binding point 0
    writeDescriptorSet.dstBinding = 0;

    vkUpdateDescriptorSets(m_device, 1, &writeDescriptorSet, 0, NULL);
    */
}

void Window::buildCommandBuffers()
{
    VkCommandBufferBeginInfo cmdBufInfo = {};
    cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufInfo.pNext = NULL;

    VkClearValue clearValues[3];
    clearValues[0].color = m_defaultClearColor;
    clearValues[1].depthStencil = { 1.0f, 0 };
    clearValues[2].color = {1.0f,0.0f,0.0f,0.0f};

    VkRenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = NULL;
    renderPassBeginInfo.renderPass = m_renderPass;
    renderPassBeginInfo.renderArea.offset.x = 0;
    renderPassBeginInfo.renderArea.offset.y = 0;
    renderPassBeginInfo.renderArea.extent.width = m_width;
    renderPassBeginInfo.renderArea.extent.height = m_height;
    renderPassBeginInfo.clearValueCount = 3;
    renderPassBeginInfo.pClearValues = clearValues;

    VkResult err;

    for (int32_t i = 0; i < m_drawCommandBuffers.size(); ++i)
    {
        // Set target frame buffer
        renderPassBeginInfo.framebuffer = m_aFrameBuffers[i];

        err = vkBeginCommandBuffer(m_drawCommandBuffers[i], &cmdBufInfo);
        assert(!err);

        vkCmdBeginRenderPass(m_drawCommandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Update dynamic viewport state
        VkViewport viewport = {};
        viewport.height = (float)m_height;
        viewport.width = (float)m_width;
        viewport.minDepth = (float) 0.0f;
        viewport.maxDepth = (float) 1.0f;
        vkCmdSetViewport(m_drawCommandBuffers[i], 0, 1, &viewport);

        // Update dynamic scissor state
        VkRect2D scissor = {};
        scissor.extent.width = m_width;
        scissor.extent.height = m_height;
        scissor.offset.x = 0;
        scissor.offset.y = 0;
        vkCmdSetScissor(m_drawCommandBuffers[i], 0, 1, &scissor);

        // Bind descriptor sets describing shader binding points
        vkCmdBindDescriptorSets(m_drawCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pPipelines->layout(), 0, 1, &m_descriptorSet, 0, NULL);

        for(auto node : m_aNodes){
            //std::cout << "binding node, i count=" << meshBuffer.indexCount << std::endl;
            // Bind triangle vertices

            VkDeviceSize offsets[1] = { 0 };

            switch(node->type())
            {
                case Node::Axis:
                    static_cast<Axis*>(node)->updateVertices(m_device,m_deviceMemoryProperties);
                    break;
                case Node::Grid:
                    static_cast<Grid*>(node)->updateVertices(m_device,m_deviceMemoryProperties);
                    break;
                case Node::Mesh:
                    static_cast<Mesh*>(node)->updateVertices(m_device,m_deviceMemoryProperties);
                    break;
                case Node::Light:
                    static_cast<PointLight*>(node)->updateVertices(m_device,m_deviceMemoryProperties);
                    break;
            }

            m_pPipelines->bind(m_device, m_drawCommandBuffers[i], node, offsets);
            // reset line width
            //vkCmdSetLineWidth(m_drawCommandBuffers[i], 1.0);

        }

        vkCmdEndRenderPass(m_drawCommandBuffers[i]);

        // Add a present memory barrier to the end of the command buffer
        // This will transform the frame buffer color attachment to a
        // new layout for presenting it to the windowing system integration 
        VkImageMemoryBarrier prePresentBarrier = {};
        prePresentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        prePresentBarrier.pNext = NULL;
        prePresentBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        //prePresentBarrier.dstAccessMask = 0;
        prePresentBarrier.dstAccessMask = VK_ACCESS_HOST_READ_BIT | VK_ACCESS_MEMORY_READ_BIT;
        prePresentBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        //prePresentBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        prePresentBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
        prePresentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        prePresentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        prePresentBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };			
        prePresentBarrier.image = m_swapChain.buffers[i].image;
        //prePresentBarrier.image = m_selection.image;

        VkImageMemoryBarrier *pMemoryBarrier = &prePresentBarrier;
        vkCmdPipelineBarrier(
                m_drawCommandBuffers[i], 
                VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 
                //VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 
                VK_FLAGS_NONE,
                0, nullptr,
                0, nullptr,
                1, &prePresentBarrier);

        err = vkEndCommandBuffer(m_drawCommandBuffers[i]);
        assert(!err);
    }

}


void Window::renderLoop()
{
    xcb_flush(m_pConnection);
    while (!m_quit)
    {
        auto tStart = std::chrono::high_resolution_clock::now();
        xcb_generic_event_t *event;
        event = xcb_poll_for_event(m_pConnection);
        if (event) 
        {
            handleEvent(event);
            free(event);
        }
        render();
        auto tEnd = std::chrono::high_resolution_clock::now();
        auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
        m_frameTimer = tDiff / 1000.0f;
   }
}

void Window::render()
{
    // TODO
    // add rendering code here

    if (!m_prepared)
        return;

    vkDeviceWaitIdle(m_device);
    draw();
    vkDeviceWaitIdle(m_device);

    updateUniformBuffers();
}

void Window::draw()
{
    VkResult err;
    // Get next image in the swap chain (back/front buffer)
    err = m_swapChain.acquireNextImage(m_semaphores.presentComplete, &m_currentBuffer);
    // had to disable this to get to work in i3
    //assert(!err);

    // The submit infor strcuture contains a list of
    // command buffers and semaphores to be submitted to a queue
    // If you want to submit multiple command buffers, pass an array
    VkPipelineStageFlags pipelineStages = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pWaitDstStageMask = &pipelineStages;
    // The wait semaphore ensures that the image is presented 
    // before we start submitting command buffers agein
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &m_semaphores.presentComplete;
    // Submit the currently active command buffer
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_drawCommandBuffers[m_currentBuffer];
    // The signal semaphore is used during queue presentation
    // to ensure that the image is not rendered before all
    // commands have been submitted
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &m_semaphores.renderComplete;

    // Submit to the graphics queue
    err = vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE);
    assert(!err);

    // Present the current buffer to the swap chain
    // We pass the signal semaphore from the submit info
    // to ensure that the image is not rendered until
    // all commands have been submitted
    err = m_swapChain.queuePresent(m_queue, m_currentBuffer, m_semaphores.renderComplete);
    assert(!err);

    // Add a post present image memory barrier
    // This will transform the frame buffer color attachment back
    // to it's initial layout after it has been presented to the
    // windowing system
    // See buildCommandBuffers for the pre present barrier that 
    // does the opposite transformation 
    VkImageMemoryBarrier postPresentBarrier = {};
    postPresentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    postPresentBarrier.pNext = NULL;
    postPresentBarrier.srcAccessMask = 0;
    postPresentBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    postPresentBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    postPresentBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    postPresentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    postPresentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    postPresentBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    postPresentBarrier.image = m_swapChain.buffers[m_currentBuffer].image;
    //postPresentBarrier.image = m_selection.image;


    // Use dedicated command buffer from example base class for submitting the post present barrier
    VkCommandBufferBeginInfo cmdBufInfo = {};
    cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    err = vkBeginCommandBuffer(m_postPresentCommandBuffer, &cmdBufInfo);
    assert(!err);

    // Put post present barrier into command buffer
    vkCmdPipelineBarrier(
            m_postPresentCommandBuffer,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_FLAGS_NONE,
            0, nullptr,
            0, nullptr,
            1, &postPresentBarrier);

    err = vkEndCommandBuffer(m_postPresentCommandBuffer);
    assert(!err);

    // Submit to the queue
    submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_postPresentCommandBuffer;

    err = vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE);
    assert(!err);

    err = vkQueueWaitIdle(m_queue);
    assert(!err);

}

void Window::handleEvent(const xcb_generic_event_t *event)
{
    switch (event->response_type & 0x7f)
    {
        case XCB_CLIENT_MESSAGE:
            if ((*(xcb_client_message_event_t*)event).data.data32[0] ==
                    (*m_pAtomDeleteWindow).atom) {
                m_quit = true;
            }
            break;
        case XCB_MOTION_NOTIFY:
            {
                xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *)event;
                if (m_mouseButtons.left)
                {
                    m_rotation.x += (m_mousePos.y - (float)motion->event_y) * 1.25f;
                    m_rotation.y -= (m_mousePos.x - (float)motion->event_x) * 1.25f;
                    viewChanged();
                }
                if (m_mouseButtons.right)
                {
                    m_zoom += (m_mousePos.y - (float)motion->event_y) * .005f;
                    viewChanged();
                }
                m_mousePos = glm::vec2((float)motion->event_x, (float)motion->event_y);
            }
            break;
        case XCB_BUTTON_PRESS:
            {
                xcb_button_press_event_t *press = (xcb_button_press_event_t *)event;
                m_mouseButtons.left = (press->detail & XCB_BUTTON_INDEX_1);
                m_mouseButtons.right = (press->detail & XCB_BUTTON_INDEX_3);
            }
            break;
        case XCB_BUTTON_RELEASE:
            {
                xcb_button_press_event_t *press = (xcb_button_press_event_t *)event;
                if (press->detail & XCB_BUTTON_INDEX_1)
                    m_mouseButtons.left = false;
                if (press->detail & XCB_BUTTON_INDEX_3)
                    m_mouseButtons.right = false;
            }
            break;
        case XCB_KEY_RELEASE:
            {
                const xcb_key_release_event_t *keyEvent = (const xcb_key_release_event_t *)event;
                if (keyEvent->detail == 0x9)
                    m_quit = true;
                keyPressed(keyEvent->detail);
            }
            break;
        case XCB_DESTROY_NOTIFY:
            m_quit = true;
            break;
        default:
            break;
    }
}

void Window::keyPressed(uint32_t keyCode)
{
    // TODO
    std::cout << "keycode: " << keyCode  << std::endl;
    switch(keyCode){
        case KEY_c:
            std::cout << "c pressed\n";
            step += 0.2;
            nodeChanged();
            break;
        case KEY_space:
            std::cout << "space pressed\n";
            step -= 0.2;
            nodeChanged();
            break;
        default:
            break;
    } 
}

void Window::viewChanged()
{
    updateUniformBuffers();
}

void Window::set_selection()
{
    for(auto node : m_aNodes){
        switch(node->type())
        {
            case Node::Mesh:
                if(node->id()==m_uboGS.object)
                    static_cast<Mesh*>(node)->set_selection(
                            m_device,
                            m_deviceMemoryProperties,
                            m_uboGS.p1,
                            m_uboGS.p2,
                            m_uboGS.p3
                            );
                break;
        }
    }
}

void Window::nodeChanged()
{
    // update the vertex buffer
    for (int32_t i = 0; i < m_drawCommandBuffers.size(); ++i)
    {
        for(auto node : m_aNodes){
            //std::cout << "binding node, i count=" << meshBuffer.indexCount << std::endl;
            // Bind triangle vertices
            VkDeviceSize offsets[1] = { 0 };
            switch(node->type())
            {
                case Node::Axis:
                    static_cast<Axis*>(node)->updateVertices(m_device,m_deviceMemoryProperties,step);
                    vkCmdBindVertexBuffers(m_drawCommandBuffers[i], VERTEX_BUFFER_BIND_ID, 1, &static_cast<Axis*>(node)->buffer()->vertices.buf, offsets);
                break;
                case Node::Grid:
                static_cast<Grid*>(node)->updateVertices(m_device,m_deviceMemoryProperties,step);
                vkCmdBindVertexBuffers(m_drawCommandBuffers[i], VERTEX_BUFFER_BIND_ID, 1, &static_cast<Grid*>(node)->buffer()->vertices.buf, offsets);
                break;
                case Node::Mesh:
                    static_cast<Mesh*>(node)->updateVertices(m_device,m_deviceMemoryProperties,step);
                    vkCmdBindVertexBuffers(m_drawCommandBuffers[i], VERTEX_BUFFER_BIND_ID, 1, &static_cast<Mesh*>(node)->buffer()->vertices.buf, offsets);
                   break;
                case Node::Light:
                    static_cast<PointLight*>(node)->updateVertices(m_device,m_deviceMemoryProperties);
                    vkCmdBindVertexBuffers(m_drawCommandBuffers[i], VERTEX_BUFFER_BIND_ID, 1, &static_cast<PointLight*>(node)->buffer()->vertices.buf, offsets);
                break;
            }

            // vkCmdBindVertexBuffers(m_drawCommandBuffers[i], VERTEX_BUFFER_BIND_ID, 1, &meshBuffer.vertices.buf, offsets);
        }
    }

    viewChanged();
}

