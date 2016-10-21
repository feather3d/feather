#ifndef QVULKANVIEW_H
#define QVULKANVIEW_H
#include "vulkan/vulkan.h"
#include <QObject>
#include <QWindow>
#include <QWidget>
#include <QFrame>
#include "window.hpp"

//class QVulkanInstance;

class QVulkanView : public QWindow
{
    Q_OBJECT
public:
    explicit QVulkanView(QWindow *parent = 0);
    ~QVulkanView();
    void resizeEvent(QResizeEvent *event) override;
    //void setWindowId(WId id);
signals:

public slots:
private:
    //static QVulkanInstance *s_vulkanInstance;
    //VkSurfaceKHR m_vkSurface;
    feather::vulkan::Window* m_pWindow;
};

/*
class QVulkanWidget: public QWidget
{
    Q_OBJECT
public:
    explicit QVulkanWidget(QWidget *parent = 0);
    ~QVulkanWidget();
    void resizeEvent(QResizeEvent *event) override;
signals:

public slots:
private:
    //static QVulkanInstance *s_vulkanInstance;
    //VkSurfaceKHR m_vkSurface;
    QVulkanView* m_pView;
};
*/

#endif // QVULKANVIEW_H
