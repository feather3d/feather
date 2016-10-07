/***********************************************************************
 *
 * Filename: viewport.hpp
 *
 * Description: Qt3D version of the viewport that displays the scenegraph.
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
#ifndef VIEWPORT_HPP 
#define VIEWPORT_HPP

#include "deps.hpp"
#include "qml_deps.hpp"
#include "types.hpp"

class DrawItem : public Qt3DCore::QEntity
{
    Q_OBJECT

    public:
        enum Type{
            None,
            Line,
            ShadedMesh,
            ComponentMesh,
            PerspCamera
        };
        DrawItem(feather::draw::Item* _item, Type _type=None, Qt3DCore::QNode *parent=0);
        ~DrawItem();
        inline void setType(Type t) { m_type=t; };
        inline Type type() { return m_type; };
        virtual void updateItem(){};
        inline unsigned int uid() { return m_item->uid; };
        inline unsigned int nid() { return m_item->nid; };
        feather::draw::Item* item() { return m_item; };
        feather::draw::Item* m_item;

    private:
        Type m_type;
};


// PERSPECTIVE CAMERA 

class PerspCamera : public DrawItem
{
    Q_OBJECT
    
    public:
        PerspCamera(Qt3DRender::QLayer* layer, feather::draw::Item* _item, Qt3DCore::QNode *parent=0);
        ~PerspCamera();
        void updateItem();
        void updateCameraPosition();
        inline Qt3DRender::QCamera* camera() { return m_pCamera; };

    private:
        Qt3DRender::QMesh *m_pMesh;
        Qt3DCore::QTransform *m_pTransform;
        Qt3DExtras::QPhongMaterial *m_pMaterial;
        Qt3DRender::QCamera* m_pCamera;
};

// WIREFRAME EFFECT
/*
class WireEffect : public Qt3D::QEffect
{
    Q_OBJECT

    public:
        WireEffect(Qt3D::QNode* parent=0);
        ~WireEffect();

    private:
        Qt3D::QAnnotation* m_pPassCriterion;
        Qt3D::QRenderPass* m_pRenderPass; 
        Qt3D::QTechnique* m_pTechnique;
};
*/

// GEOMETRY 

// Shaded Mesh
class MeshGeometry : public Qt3DRender::QGeometry
{
    Q_OBJECT

    public:
        MeshGeometry(int _uid=0, int _nid=0, int _fid=0, Qt3DCore::QNode *parent=0);
        ~MeshGeometry();
        void updateBuffers();
 
    private:
        void build();
        int uid;
        int nid;
        int fid;
        uint vcount; // vertex count
        uint icount; // index count
        Qt3DRender::QBuffer *m_pVertexBuffer;
        Qt3DRender::QBuffer *m_pIndexBuffer;
        Qt3DRender::QAttribute *m_pVAttribute;
        Qt3DRender::QAttribute *m_pVnAttribute;
        //Qt3DRender::QAttribute *m_pColorAttribute;
        Qt3DRender::QAttribute *m_pIndexAttribute;
};


// Points
class MeshPointGeometry : public Qt3DRender::QGeometry
{
    Q_OBJECT

    public:
        MeshPointGeometry(int _uid=0, int _nid=0, int _fid=0, Qt3DCore::QNode *parent=0);
        ~MeshPointGeometry();
        void updateBuffers();
 
    private:
        void build();
        int uid;
        int nid;
        int fid;
        int vcount; // how many vertex
        Qt3DRender::QAttribute *m_pVAttribute;
        Qt3DRender::QBuffer *m_pVertexBuffer;
        Qt3DRender::QAttribute *m_pColorAttribute;
};


// Edges 
class MeshEdgeGeometry : public Qt3DRender::QGeometry
{
    Q_OBJECT

    public:
        MeshEdgeGeometry(int _uid=0, int _nid=0, int _fid=0, Qt3DCore::QNode *parent=0);
        ~MeshEdgeGeometry();
        void updateBuffers();
 
    private:
        void build();
        int uid;
        int nid;
        int fid;
        std::vector<feather::FVertex3D> m_aMeshVData;
        Qt3DRender::QAttribute *m_pVAttribute;
        Qt3DRender::QBuffer *m_pVertexBuffer;
};


// There will be two types of Mesh types - Shaded and Component
//      ShadedMesh is for meshes that are only drawn and can not be selected or interacted with
//      ComponentMesh is a mesh that can have all it's components selected

class ShadedMesh : public DrawItem 
{
    Q_OBJECT
    
    public:
        ShadedMesh(Qt3DRender::QLayer* layer, feather::draw::Item* _item, Qt3DCore::QNode *parent=0);
        ~ShadedMesh();
        void updateItem();
        void test();

    private slots:
        void clicked(Qt3DRender::QPickEvent* event);

    private:
        void build();
        Qt3DCore::QTransform *m_pTransform;
        Qt3DExtras::QPhongMaterial *m_pMaterial;
        Qt3DRender::QGeometryRenderer *m_pMesh;
        Qt3DRender::QPointLight *m_pLight;
        Qt3DRender::QObjectPicker *m_pObjectPicker;
};


class ComponentMesh : public DrawItem 
{
    Q_OBJECT
    
    public:
        ComponentMesh(Qt3DRender::QLayer* layer, feather::draw::Item* _item, Qt3DCore::QNode *parent=0);
        ~ComponentMesh();
        void updateItem();
        void test();

    private slots:
        void clicked(Qt3DRender::QPickEvent* event);

    private:
        void build();
        Qt3DCore::QTransform *m_pTransform;
        Qt3DExtras::QPerVertexColorMaterial *m_pMaterial;
        Qt3DRender::QGeometryRenderer *m_pMeshPoints;
        Qt3DRender::QGeometryRenderer *m_pMeshEdges;
        Qt3DRender::QPointLight *m_pLight;
        Qt3DRender::QObjectPicker *m_pObjectPicker;
};




// LINE

class Line : public DrawItem 
{
    Q_OBJECT
    
    public:
        Line(Qt3DRender::QLayer* layer, feather::draw::Item* _item, Qt3DCore::QNode *parent=0);
        ~Line();
        void updateItem();

    private:
        void build();
        Qt3DCore::QTransform *m_pTransform;
        Qt3DExtras::QPhongMaterial *m_pMaterial;
        Qt3DRender::QGeometryRenderer *m_pMesh;
        //Qt3D::QMouseInput *m_pMouseInput;
        std::vector<feather::FVertex3D> m_aVertex;
        Qt3DRender::QAttribute *m_meshAttribute;
        Qt3DRender::QBuffer *m_vertexBuffer;
};


// Axis 

class AxisGeometry : public Qt3DRender::QGeometry
{
    Q_OBJECT

    public:
        AxisGeometry(Qt3DCore::QNode *parent=0);
 
    private:
        void build();
        std::vector<feather::FVertex3D> m_axis;
        Qt3DRender::QAttribute *m_meshAttribute;
        Qt3DRender::QBuffer *m_vertexBuffer;
};


class Axis : public Qt3DCore::QEntity
{
    Q_OBJECT
    
    public:
        Axis(Qt3DRender::QLayer* layer, Qt3DCore::QNode *parent=0);
        ~Axis();
        AxisGeometry* grid() { return static_cast<AxisGeometry*>(m_pMesh->geometry()); };

    /*
    private Q_SLOTS:
        void mouseClicked();
    */

    private:
        void build();
        Qt3DCore::QTransform *m_pTransform;
        Qt3DRender::QMaterial *m_pMaterial;
        Qt3DRender::QGeometryRenderer *m_pMesh;
        //Qt3D::QMouseInput *m_pMouseInput;
};


// GRID

class GridGeometry : public Qt3DRender::QGeometry
{
    Q_OBJECT

    public:
        GridGeometry(Qt3DCore::QNode *parent=0);
        int majorSubDividLevel();
        int minorSubDividLevel();
        void setMajorSubDividLevel(const int &level);
        void setMinorSubDividLevel(const int &level);
       
    private:
        void build();
        int m_majorLevel;
        int m_minorLevel;
        std::vector<feather::FVertex3D> m_grid;
        Qt3DRender::QAttribute *m_meshAttribute;
        Qt3DRender::QBuffer *m_vertexBuffer;
};


class Grid : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(QColor diffuseColor READ diffuseColor WRITE setDiffuseColor NOTIFY diffuseColorChanged)
    
    public:
        Grid(Qt3DRender::QLayer* layer, Qt3DCore::QNode *parent=0);
        ~Grid();
        GridGeometry* grid() { return static_cast<GridGeometry*>(m_pMesh->geometry()); };
        QColor diffuseColor();

    public Q_SLOTS:
        void setDiffuseColor(const QColor &diffuseColor);
 
    Q_SIGNALS:
        void diffuseColorChanged(QColor);
 
    private:
        void build();
        Qt3DCore::QTransform *m_pTransform;
        Qt3DExtras::QPhongMaterial *m_pMaterial;
        Qt3DRender::QGeometryRenderer *m_pMesh;
};

// MESHES

class TessellatedGeometry : public Qt3DRender::QGeometry
{
    Q_OBJECT
    public:
        TessellatedGeometry(Qt3DCore::QNode *parent=0);

    private:
        Qt3DRender::QAttribute *m_positionAttribute;
        Qt3DRender::QBuffer *m_vertexBuffer;
};



class Object: public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(QColor ambientColor READ ambientColor WRITE setAmbientColor NOTIFY ambientColorChanged)
    Q_PROPERTY(QColor specularColor READ specularColor WRITE setSpecularColor NOTIFY specularColorChanged)
    Q_PROPERTY(QColor diffuseColor READ diffuseColor WRITE setDiffuseColor NOTIFY diffuseColorChanged)
 
    public:
        Object(Qt3DRender::QLayer* layer, Qt3DCore::QNode *parent = 0);
        QColor ambientColor();
        QColor specularColor();
        QColor diffuseColor();

    public Q_SLOTS:
        void setAmbientColor(const QColor &ambientColor);
        void setSpecularColor(const QColor &specularColor);
        void setDiffuseColor(const QColor &diffuseColor);
 
    Q_SIGNALS:
        void ambientColorChanged();
        void specularColorChanged();
        void diffuseColorChanged(QColor);

    /* 
    private Q_SLOTS:
        void onClicked(Qt3D::Q3DMouseEvent* event);
        void onEvent(Qt3D::Q3DMouseEvent* event);
    */

    private:
        QVector3D m_translate;
        Qt3DCore::QTransform *m_transform;
        /*
        Qt3D::QTranslateTransform *m_translate;
        Qt3D::QScaleTransform *m_scale;
        */
        Qt3DRender::QGeometryRenderer *m_mesh;
        Qt3DExtras::QPhongMaterial *m_material;
        //Qt3D::QMouseController *m_pMouseController;
        //Qt3D::QMouseInput *m_pMouseInput;
};


class FrameGraph : public Qt3DRender::QRenderSettings
{
    Q_OBJECT

    public:
        FrameGraph(Qt3DCore::QNode* parent=0);
        ~FrameGraph();
        void setCamera(Qt3DRender::QCamera* camera);
        inline Qt3DRender::QLayer* layer() { return m_pLayerFilter->layers().at(0); };
        inline Qt3DRender::QViewport* viewport() { return m_pViewport; };

    private:
        Qt3DRender::QViewport* m_pViewport;
        Qt3DRender::QClearBuffers* m_pClearBuffer;
        Qt3DRender::QCameraSelector* m_pCameraSelector;
        Qt3DRender::QLayerFilter* m_pLayerFilter;
        Qt3DRender::QRenderSurfaceSelector* m_pRenderSurfaceSelector;
        Qt3DRender::QRenderStateSet* m_pRenderStateSet;
        //Qt3DRender::QPickingSettings* m_pPickingSettings;
 
};


class Viewport : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(int majorSubDividLevel READ majorSubDividLevel WRITE setMajorSubDividLevel NOTIFY majorSubDividLevelChanged)
    Q_PROPERTY(int minorSubDividLevel READ minorSubDividLevel WRITE setMinorSubDividLevel NOTIFY minorSubDividLevelChanged)
    Q_PROPERTY(bool showGrid READ showGrid WRITE setShowGrid NOTIFY showGridChanged)
    Q_PROPERTY(bool showAxis READ showAxis WRITE setShowAxis NOTIFY showAxisChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
 
    public:
        enum Display {
            Hide,
            BoundingBox,
            Point,
            Edge,
            Wireframe,
            SmoothShaded,
            FlatShaded,
            Rendered
        };

        explicit Viewport(Qt3DCore::QNode *parent = 0);
        ~Viewport();
        int majorSubDividLevel();
        int minorSubDividLevel();
        bool showGrid();
        bool showAxis();
        int width();
        int height();

        FrameGraph* frameGraph() { return m_pFrameGraph; };
        Q_INVOKABLE void doUpdate(){ updateScene(); };
        Q_INVOKABLE void addItems(unsigned int uid);
        Q_INVOKABLE void updateItems(unsigned int uid);
        Q_INVOKABLE void updateChangedNodes();
        Q_INVOKABLE void setCamera(unsigned int uid);
        Q_INVOKABLE void removeItem(unsigned int uid);

    private slots:
        void updateScene();
        void buildScene(feather::draw::DrawItems& items);
        void setMajorSubDividLevel(const int &level);
        void setMinorSubDividLevel(const int &level);
        void setShowGrid(const bool &show);
        void setShowAxis(const bool &show);
        void setWidth(const int &val);
        void setHeight(const int &val);
        void onMouseEntered();
        void onClicked(Qt3DInput::QMouseEvent* event);
        void handleTriggered(float dt);
        void doSpacePressed(Qt3DInput::QKeyEvent* event);
        void wheelEvent(Qt3DInput::QWheelEvent* event);
        void keyEvent(Qt3DInput::QKeyEvent* event);

   signals:
        void majorSubDividLevelChanged();
        void minorSubDividLevelChanged();
        void showGridChanged();
        void showAxisChanged();
        void widthChanged();
        void heightChanged();

    private:
        // use this method to see if there are any items that need to be built
        bool buildItems(feather::draw::DrawItems& items);
        bool m_showGrid;
        bool m_showAxis;
        int m_width;
        int m_height;

        void adjustCamera(QVector3D &translateVec);
        QVector3D findIntersection(const QVector3D &rayOrigin, const QVector3D &ray, float planeOffset, const QVector3D &planeNormal, float &t);
        QVector3D cameraNormal(Qt3DRender::QCamera *camera);
        QVector3D unprojectRay(const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix, int viewPortWidth, int viewPortHeight, const QPoint &pos);
        void handleWheel(Qt3DInput::QWheelEvent *event);

        QList<DrawItem*> m_apDrawItems;
        bool m_ignoreFirstLeftMousePress;
        bool m_ignoreFirstRightMousePress;
        bool m_ignoreFirstMiddleMousePress;
        float m_translateSpeed;
        float m_panSpeed;
        float m_orbitSpeed;
        float m_wheelSpeed;
        bool m_inputEnabled;
        QVector3D m_cameraUp;
        bool m_adjustCameraAtMouseRelease;

        Grid* m_pGrid;
        Axis* m_pAxis;
        // Gone
        //Qt3DInput::QMouseController *m_pMouseController;
        Qt3DInput::QMouseDevice* m_pMouseDevice;
        Qt3DInput::QMouseHandler* m_pMouseHandler;
        Qt3DInput::QKeyboardDevice* m_pKeyboardDevice;
        Qt3DInput::QKeyboardHandler* m_pKeyboardHandler;
        Qt3DInput::QInputSettings* m_pInputSettings;
        Qt3DInput::QAction* m_pLeftMouseButtonAction;
        Qt3DInput::QActionInput* m_pLeftMouseButtonInput;
        Qt3DInput::QAction* m_pRightMouseButtonAction;
        Qt3DInput::QActionInput* m_pRightMouseButtonInput;
        Qt3DInput::QAction* m_pMiddleMouseButtonAction;
        Qt3DInput::QActionInput* m_pMiddleMouseButtonInput;
        Qt3DInput::QAxis* m_pRx;
        Qt3DInput::QAxis* m_pRy;
        Qt3DInput::QAnalogAxisInput* m_pMouseRyInput;
        Qt3DInput::QAnalogAxisInput* m_pMouseRxInput;
        //Qt3DRender::QPickingSettings* m_pPickingSettings;
        Qt3DInput::QLogicalDevice* m_pLogicalDevice;
        Qt3DLogic::QFrameAction* m_pFrameAction;
        Line* m_pLine;
        Qt3DRender::QDirectionalLight* m_pCameraLight;
        Qt3DRender::QClearBuffers m_clearBuffer;
        Qt3DExtras::QTorusMesh* m_pTorus;
        Qt3DRender::QCamera* m_pCamera; // this is qt3d's camera
        //Qt3DRender::QRenderSettings* m_pRenderSettings;
        //Qt3DRender::QRenderSurfaceSelector* m_pRenderSurfaceSelector;
        // GONE
        //Qt3D::Quick::Quick3DConfiguration* m_pConfiguration;
        FrameGraph* m_pFrameGraph;
};

#endif
