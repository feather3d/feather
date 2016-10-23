//#define VK_USE_PLATFORM_XCB_KHR 1

#include "qvulkanwindow.hpp"
#include <QDebug>
#include <QApplication>
#include <QStringList>
//#include "qvulkaninstance.h"
// TODO - I need to fix this below line so that you can build using the above line
#include <QtGui/5.7.0/QtGui/qpa/qplatformnativeinterface.h>

//QVulkanInstance* QVulkanView::s_vulkanInstance = nullptr;

QVulkanView::QVulkanView(QWindow *parent) : QWindow(parent)
{
    setWidth(1200);
    setHeight(800);

    QPlatformNativeInterface *native =  QGuiApplication::platformNativeInterface();
    
    VkResult err;
    xcb_connection_t* connection = static_cast<xcb_connection_t *>(native->nativeResourceForWindow("connection", this));

    std::cout << "connection:" << native->nativeResourceForWindow("connection", this) << std::endl;
    //xcb_window_t window = static_cast<xcb_window_t>(winId());
    m_pWindow = new feather::vulkan::Window(static_cast<xcb_window_t>(winId()),connection,"QVulkanView",width(),height());
    //xcb_window_t window = m_pWindow->window();

    //qDebug()<<connection<<window;

    /*
    VkXcbSurfaceCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.connection = connection;
    createInfo.window = window;
    */

    //err = vkCreateXcbSurfaceKHR(s_vulkanInstance->data(), &createInfo, NULL, &m_vkSurface);
    //err = vkCreateXcbSurfaceKHR(m_pWindow->instance(), &createInfo, NULL, &m_pWindow->surface());

    Q_ASSERT(!err);

    show();
    /*
    VkResult err;
    if(!s_vulkanInstance)
        s_vulkanInstance = new QVulkanInstance(true);

    QVector<const char*> instance_layers;
    QVector<const char*> instance_extensions;

    QStringList exts = s_vulkanInstance->extensionNames();
    if(exts.contains(VK_KHR_SURFACE_EXTENSION_NAME)) {
        instance_extensions.append(VK_KHR_SURFACE_EXTENSION_NAME);
    }
    if(exts.contains(VK_KHR_XCB_SURFACE_EXTENSION_NAME)) {
        instance_extensions.append(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
    }
    if(exts.contains(VK_EXT_DEBUG_REPORT_EXTENSION_NAME)) {
        instance_extensions.append(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }


    s_vulkanInstance->init(instance_layers, instance_extensions);

    Q_ASSERT(s_vulkanInstance->devices().size() > 0);
    QVulkanPhysicalDevice physDevice(s_vulkanInstance->devices()[0]);
//    QVector<const char*> extensions;
    if(physDevice.extensionNames().contains(VK_KHR_SWAPCHAIN_EXTENSION_NAME)) {
        //extensions.append(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    } else {
        qFatal("vkEnumerateDeviceExtensionProperties failed to find "
                 "the " VK_KHR_SWAPCHAIN_EXTENSION_NAME
                 " extension.\n\nDo you have a compatible "
                 "Vulkan installable client driver (ICD) installed?\nPlease "
                 "look at the Getting Started guide for additional "
                 "information.\n");
    }
    Q_ASSERT(physDevice.hasGraphicsQueue() > 0);
    QPlatformNativeInterface *native =  QGuiApplication::platformNativeInterface();

    // Create a WSI surface for the window:
    #ifdef _WIN32
        HINST hinst = static_cast<HINST>(native->nativeResourceForWindow("connection", this));
        VkWin32SurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.hinstance = hinst;
        createInfo.hwnd = winId();
        err = vkCreateWin32SurfaceKHR(demo->inst, &createInfo, NULL, &demo->surface);
    #else  // _WIN32
    //FIXME requires qpa headers...
//        xcb_window_t *window = static_cast<QXcbWindow *>(QSurface::surfaceHandle())->xcb_window();

        xcb_connection_t* connection = static_cast<xcb_connection_t *>(native->nativeResourceForWindow("connection", this));
        xcb_window_t window = static_cast<xcb_window_t>(winId());

        qDebug()<<connection<<window;

        VkXcbSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.connection = connection;
        createInfo.window = window;

        err = vkCreateXcbSurfaceKHR(s_vulkanInstance->data(), &createInfo, NULL, &m_vkSurface);
    #endif // _WIN32
    Q_ASSERT(!err);


    uint32_t graphicsQueueNodeIndex = UINT32_MAX;
    uint32_t presentQueueNodeIndex = UINT32_MAX;

    for(int i = 0; i < physDevice.queueProperties().size(); i++) {
        VkQueueFamilyProperties& prop = physDevice.queueProperties()[i];
        if(prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            if (graphicsQueueNodeIndex == UINT32_MAX) {
                graphicsQueueNodeIndex = i;
            }
            if(physDevice.queueSupportsPresent(i, m_vkSurface)) {
                graphicsQueueNodeIndex = i;
                presentQueueNodeIndex = i;
                break;
            }
        }
    }

    if (presentQueueNodeIndex == UINT32_MAX) {
        // If didn't find a queue that supports both graphics and present, then
        // find a separate present queue.
        for (int i = 0; i < physDevice.queueProperties().size(); ++i) {
            if(physDevice.queueSupportsPresent(i, m_vkSurface)) {
                presentQueueNodeIndex = i;
                break;
            }
        }
    }

    // Generate error if could not find both a graphics and a present queue
    if (graphicsQueueNodeIndex == UINT32_MAX ||
            presentQueueNodeIndex == UINT32_MAX) {
        qFatal("Could not find a graphics and a present queue");
    }

    // TODO: Add support for separate queues, including presentation,
    //       synchronization, and appropriate tracking for QueueSubmit.
    // NOTE: While it is possible for an application to use a separate graphics
    //       and a present queues, this demo program assumes it is only using
    //       one:
    if (graphicsQueueNodeIndex != presentQueueNodeIndex) {
        qFatal("Could not find a common graphics and a present queue");
    }

    QVector<const char*> extensions;
    foreach(VkExtensionProperties ext, physDevice.extensions())
        extensions.push_back(ext.extensionName);

    QVulkanDevice device(physDevice.device(), graphicsQueueNodeIndex, instance_layers, extensions);

    VkSurfaceFormatKHR surfFormat;

    QVector<VkSurfaceFormatKHR> formats = physDevice.surfaceFormats(m_vkSurface);

    // If the format list includes just one entry of VK_FORMAT_UNDEFINED,
    // the surface has no preferred format.  Otherwise, at least one
    // supported format will be returned.
    if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED) {
        surfFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
    } else {
        Q_ASSERT(formats.size() >= 1);
        surfFormat.format = formats[0].format;
    }
    surfFormat.colorSpace = formats[0].colorSpace;
    qDebug()<<"format:"<<surfFormat.format<<"colorspace"<<surfFormat.colorSpace;
    physDevice.init(m_vkSurface);
    VkCommandPool commandPool = device.createCommandPool(graphicsQueueNodeIndex);
    */
}

QVulkanView::~QVulkanView()
{
/*
    uint32_t i;

    demo->prepared = false;

    for (i = 0; i < demo->swapchainImageCount; i++) {
        vkDestroyFramebuffer(demo->device, demo->framebuffers[i], NULL);
    }
    free(demo->framebuffers);
    vkDestroyDescriptorPool(demo->device, demo->desc_pool, NULL);

    vkDestroyPipeline(demo->device, demo->pipeline, NULL);
    vkDestroyPipelineCache(demo->device, demo->pipelineCache, NULL);
    vkDestroyRenderPass(demo->device, demo->render_pass, NULL);
    vkDestroyPipelineLayout(demo->device, demo->pipeline_layout, NULL);
    vkDestroyDescriptorSetLayout(demo->device, demo->desc_layout, NULL);

    for (i = 0; i < DEMO_TEXTURE_COUNT; i++) {
        vkDestroyImageView(demo->device, demo->textures[i].view, NULL);
        vkDestroyImage(demo->device, demo->textures[i].image, NULL);
        vkFreeMemory(demo->device, demo->textures[i].mem, NULL);
        vkDestroySampler(demo->device, demo->textures[i].sampler, NULL);
    }
    demo->fpDestroySwapchainKHR(demo->device, demo->swapchain, NULL);

    vkDestroyImageView(demo->device, demo->depth.view, NULL);
    vkDestroyImage(demo->device, demo->depth.image, NULL);
    vkFreeMemory(demo->device, demo->depth.mem, NULL);

    vkDestroyBuffer(demo->device, demo->uniform_data.buf, NULL);
    vkFreeMemory(demo->device, demo->uniform_data.mem, NULL);

    for (i = 0; i < demo->swapchainImageCount; i++) {
        vkDestroyImageView(demo->device, demo->buffers[i].view, NULL);
        vkFreeCommandBuffers(demo->device, demo->cmd_pool, 1,
                             &demo->buffers[i].cmd);
    }
    free(demo->buffers);

    free(demo->queue_props);

    vkDestroyCommandPool(demo->device, demo->cmd_pool, NULL);
    vkDestroyDevice(demo->device, NULL);
    if (demo->validate) {
        demo->DestroyDebugReportCallback(demo->inst, demo->msg_callback, NULL);
    }
    vkDestroySurfaceKHR(demo->inst, demo->surface, NULL);
    vkDestroyInstance(demo->inst, NULL);
*/
}

void QVulkanView::resizeEvent(QResizeEvent *event)
{
/*
    uint32_t i;

    // Don't react to resize until after first initialization.
    if (!demo->prepared) {
        return;
    }
    // In order to properly resize the window, we must re-create the swapchain
    // AND redo the command buffers, etc.
    //
    // First, perform part of the demo_cleanup() function:
    demo->prepared = false;

    for (i = 0; i < demo->swapchainImageCount; i++) {
        vkDestroyFramebuffer(demo->device, demo->framebuffers[i], NULL);
    }
    free(demo->framebuffers);
    vkDestroyDescriptorPool(demo->device, demo->desc_pool, NULL);

    vkDestroyPipeline(demo->device, demo->pipeline, NULL);
    vkDestroyPipelineCache(demo->device, demo->pipelineCache, NULL);
    vkDestroyRenderPass(demo->device, demo->render_pass, NULL);
    vkDestroyPipelineLayout(demo->device, demo->pipeline_layout, NULL);
    vkDestroyDescriptorSetLayout(demo->device, demo->desc_layout, NULL);

    for (i = 0; i < DEMO_TEXTURE_COUNT; i++) {
        vkDestroyImageView(demo->device, demo->textures[i].view, NULL);
        vkDestroyImage(demo->device, demo->textures[i].image, NULL);
        vkFreeMemory(demo->device, demo->textures[i].mem, NULL);
        vkDestroySampler(demo->device, demo->textures[i].sampler, NULL);
    }

    vkDestroyImageView(demo->device, demo->depth.view, NULL);
    vkDestroyImage(demo->device, demo->depth.image, NULL);
    vkFreeMemory(demo->device, demo->depth.mem, NULL);

    vkDestroyBuffer(demo->device, demo->uniform_data.buf, NULL);
    vkFreeMemory(demo->device, demo->uniform_data.mem, NULL);

    for (i = 0; i < demo->swapchainImageCount; i++) {
        vkDestroyImageView(demo->device, demo->buffers[i].view, NULL);
        vkFreeCommandBuffers(demo->device, demo->cmd_pool, 1,
                             &demo->buffers[i].cmd);
    }
    vkDestroyCommandPool(demo->device, demo->cmd_pool, NULL);
    free(demo->buffers);

    // Second, re-perform the demo_prepare() function, which will re-create the
    // swapchain:
    demo_prepare(demo);
    */
}

/*
QVulkanWidget::QVulkanWidget(QWidget *parent) : QWidget(parent)
{
    QVulkanView* m_pView;
} 

QVulkanWidget::~QVulkanWidget()
{

}
*/
