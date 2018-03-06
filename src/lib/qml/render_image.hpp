/***********************************************************************
 *
 * Filename: render_image.hpp
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

#ifndef RENDER_IMAGE_HPP
#define RENDER_IMAGE_HPP

#include "deps.hpp"
#include "qml_deps.hpp"
#include "field_model.hpp"
#include "render.hpp"

class RenderImage : public QQuickPaintedItem
{
    Q_OBJECT
         Q_PROPERTY(int clickX READ clickX)
         Q_PROPERTY(int clickY READ clickY)
 
    public:
        RenderImage(QQuickItem* parent=0);
        ~RenderImage();

        void start_render_update();
        void stop_render_update();
        void paint(QPainter* painter);
        Q_INVOKABLE void update_render() { updateImage(); update(); };
        
        void set_buffer(char* buffer);
        feather::render::RenderBuffer& getRenderBuffer() { return m_RenderBuffer; };
        inline int clickX() { return mouseX; };
        inline int clickY() { return mouseY; };

    protected:
        void mousePressEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);
        void hoverEnterEvent(QHoverEvent* event);
        void hoverLeaveEvent(QHoverEvent* event);
        void hoverMoveEvent(QHoverEvent* event);
        void mouseMoveEvent(QMouseEvent* event);

    signals:
        void imageUpdated();

    private:
        void updateImage();
        void clearImage();
        QTimer* m_pTimer;
        int mouseX;
        int mouseY;
        char* dataBuffer;
        uint16_t mWidth;
        uint16_t mHeight;
        feather::render::RenderBuffer m_RenderBuffer;
};

#endif
