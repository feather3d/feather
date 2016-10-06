/***********************************************************************
 *
 * Filename: qml_deps.hpp
 *
 * Description: Dependencies for the qml wrapper.
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

#ifndef QML_DEPS_HPP
#define QML_DEPS_HPP

// QT5
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickPaintedItem>
#include <QtQuick/QQuickItem>
#include <QtQuick/qquickwindow.h>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QSGFlatColorMaterial>
#include <QQuickItem>
#include <QtCore/QMutex>
#include <QtCore/QThread>
#include <QtGui/QGuiApplication>
#include <QtGui/QOffscreenSurface>
#include <QTime>
#include <QTimer>

#include <qmath.h>

#include <QtCore/QRectF>

// QtOpenGL
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtGui/qopenglshaderprogram.h>
#include <qsgsimpletexturenode.h>
#include <QVector>
#include <QPainter>
#include <QPaintEngine>
#include <QtOpenGL/QtOpenGL>
#include <QtOpenGL/QGLFunctions>

// Qt3D
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QNode>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
//#include <Qt3DCore/QTranslateTransform>
//#include <Qt3DCore/QScaleTransform>
#include <Qt3DRender/QCameraLens>
#include <Qt3DRender/QCamera>

#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QPerVertexColorMaterial>
#include <Qt3DRender/QShaderProgram>
#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QTechnique>
#include <Qt3DRender/QRenderPass>
//#include <Qt3DRender/QOpenGLFilter> // gone
#include <Qt3DRender/QParameter>
//#include <Qt3DRender/QParameterMapping> // gone
#include <Qt3DRender/qattribute.h>
#include <Qt3DRender/qbuffer.h>
#include <Qt3DRender/qgeometry.h>
//#include <Qt3DRender/QWindow> // might be Q3DQuickWindow now
//#include <Qt3DRender/QFrameGraph> // might be QFrameGraphNode now
#include <Qt3DRender/QPointLight>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DRender/QLayer>

#include <Qt3DRender/QLayerFilter>
#include <Qt3DRender/QClearBuffers>
#include <Qt3DRender/QRenderTargetSelector>
#include <Qt3DRender/QRenderPassFilter>
#include <Qt3DRender/QCameraSelector>
#include <Qt3DRender/QViewport>
//#include <Qt3DRender/QAbstractTextureProvider> // gone
#include <Qt3DRender/QRenderTarget>
//#include <Qt3DRender/QRenderAttachment> // gone
#include <Qt3DRender/QTextureWrapMode>
#include <Qt3DRender/QTexture>

#include <Qt3DInput/QInputAspect>
#include <Qt3DInput/QMouseDevice>
//#include <Qt3DInput/QMouseController> // gone
#include <Qt3DInput/QMouseEvent>

//#include <Qt3DQuick/quick3dconfiguration.h>
#include <Qt3DQuickExtras/Qt3DQuickExtras>

#endif
