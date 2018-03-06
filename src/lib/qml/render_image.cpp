/***********************************************************************
 *
 * Filename: render_image.cpp
 *
 * Description: Displays the rendered image.
 *
 * Copyright (C) 2018 Richard Layman, rlayman2000@yahoo.com 
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

#include "render_image.hpp"
#include "commands.hpp"
#include "selection.hpp"
#include "field.hpp"
#include "state.hpp"
#include "plugin.hpp"
#include "qml_status.hpp"


// RENDER IMAGE 

RenderImage::RenderImage(QQuickItem* parent) : 
    QQuickPaintedItem(parent),
    m_pTimer(new QTimer(this)),
    mouseX(0),
    mouseY(0),
    m_RenderBuffer(feather::render::RenderBuffer()),
    mWidth(400),
    mHeight(200)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    updateImage();

    //m_RenderBuffer = feather::render::RenderBuffer();

    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(update()));
}


RenderImage::~RenderImage()
{
    clearImage();
}


void RenderImage::start_render_update()
{
    m_pTimer->start(1000);
}


void RenderImage::stop_render_update()
{
    m_pTimer->stop();
}


void RenderImage::set_buffer(char* buffer)
{
    QFile file("/home/richard/tv.data");
    if (!file.open(QFile::ReadOnly)) {
        std::cout << "FILE FAILED TO LOAD\n";
        return;
    }

    m_RenderBuffer.data=file.readAll().data();
}


void RenderImage::paint(QPainter* painter)
{
    std::cout << "update paint\n";
    //char* test;
    //set_buffer(test);
    QImage image(reinterpret_cast<uchar*>(m_RenderBuffer.data), mWidth, mHeight, QImage::Format_RGB888);
    painter->drawImage(QRect(0,0,mWidth,mHeight),image);
}

void RenderImage::updateImage()
{
    int xpos = 50;
    int ypos = 50;

    clearImage();

    std::vector<int> uids;

    // disabled selection as root for testing
    //feather::qml::command::get_selected_nodes(uids);
    //uids.push_back(0);

    //std::cout << uids.size() << " nodes are selected\n";

    /*
    std::for_each(uids.begin(),uids.end(),[](int& n){ std::cout << n << ","; });

    updateNode(nullptr,0,xpos,ypos);
    for(auto n : m_nodes) 
        updateLinks(n->uid());
    */
}

void RenderImage::clearImage()
{
    //std::for_each(m_links.begin(), m_links.end(), [](SceneGraphLink* link){ delete link; });
    //m_links.clear();
    //std::for_each(m_nodes.begin(), m_nodes.end(), [](SceneGraphNode* node){ delete node; });
    //m_nodes.clear();
}

void RenderImage::mousePressEvent(QMouseEvent* event)
{
    std::cout << "mouse press\n";
    // close and connection dialogs if the are open
};

void RenderImage::mouseReleaseEvent(QMouseEvent* event){ std::cout << "mouse release\n"; };
void RenderImage::hoverEnterEvent(QHoverEvent* event){ std::cout << "hover enter\n"; };
void RenderImage::hoverLeaveEvent(QHoverEvent* event){ std::cout << "hover leave\n"; };
void RenderImage::hoverMoveEvent(QHoverEvent* event){ std::cout << "hover move\n"; };
void RenderImage::mouseMoveEvent(QMouseEvent* event){ std::cout << "mouse move\n"; };
