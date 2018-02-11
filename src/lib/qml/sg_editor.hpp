/***********************************************************************
 *
 * Filename: sg_editor.hpp
 *
 * Description: Editor widget for scenegraph.
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

#ifndef SG_EDITOR_HPP
#define SG_EDITOR_HPP

#include "deps.hpp"
#include "qml_deps.hpp"
#include "field_model.hpp"

#define NODE_WIDTH 120
#define NODE_HEIGHT 20
#define CONNECTION_WIDTH 10
#define CONNECTION_HEIGHT 10 

class SceneGraphEditor;
class SceneGraphNode;
class SceneGraphConnection;

struct SGState {
    // Source Node Info
    // This is used for when connection node fields together
    // and keeping track of the currently selected nodes
    enum Mode { Normal, FieldConnection };
    static Mode mode;
    static int srcUid;
    static int srcNid;
    static int srcFid;
    static int tgtUid;
    static int tgtNid;
    static int tgtFid;
    static SceneGraphEditor* pSge;
    static std::vector<SceneGraphConnection*> selectedConnections;
    static void remove(SceneGraphConnection* conn) {
        int count = 0;
        std::for_each(selectedConnections.begin(), selectedConnections.end(),[&conn,&count](SceneGraphConnection* c){ if(c==conn){ selectedConnections.erase(selectedConnections.begin()+count); } else { count++; } });
    }
};


struct MouseInfo {
        static int clickX;
        static int clickY;
};


class SceneGraphConnection : public QQuickPaintedItem
{
    Q_OBJECT

    public:
        enum Connection { In, Out };

        SceneGraphConnection(SceneGraphNode* node, Connection type, QQuickItem* parent=0);
        ~SceneGraphConnection();
        void paint(QPainter* painter);
        SceneGraphNode* node() { return m_node; };
        inline Connection type() { return m_type; };
        inline void setSelected(bool s) { m_selected=s; };

    protected:
        void mousePressEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);
        void hoverEnterEvent(QHoverEvent* event);
        void hoverLeaveEvent(QHoverEvent* event);
        void mouseMoveEvent(QMouseEvent* event);

    signals:
        void connClicked(Qt::MouseButton button, SceneGraphConnection::Connection conn, int x, int y);

    private:
        bool m_selected;
        Connection m_type;
        QBrush m_connFillBrush;
        SceneGraphNode* m_node;
};

class SceneGraphNode : public QQuickPaintedItem
{
    Q_OBJECT

    public:
        SceneGraphNode(int uid, int nid, QQuickItem* parent=0);
        ~SceneGraphNode();
        void paint(QPainter* painter);
        void inConnectionPoint(QPointF& point);
        void outConnectionPoint(QPointF& point);
        SceneGraphConnection* inConnection();
        SceneGraphConnection* outConnection();
        inline int nid() { return m_nid; };
        inline int uid() { return m_uid; }; /*! Node's unique id assigned by the scenegraph. */
 
    protected slots:
        void ConnPressed(Qt::MouseButton button,SceneGraphConnection::Connection conn, int x, int y);
 
    signals:
        void ConnClicked(Qt::MouseButton button, SceneGraphConnection::Connection conn, int x, int y, int uid, int nid);
        void nodePressed(Qt::MouseButton button, int uid, int nid);

    protected:
        void mousePressEvent(QMouseEvent* event);
        void mouseDoubleClickEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);
        void hoverEnterEvent(QHoverEvent* event);
        void hoverLeaveEvent(QHoverEvent* event);
        void mouseMoveEvent(QMouseEvent* event);

    private:
        void getConnectionPoint(feather::field::connection::Type conn, QPoint& npoint, QPoint& cpoint);
        int m_uid;
        int m_nid;
        int m_x;
        int m_y;
        std::string m_imgDir;
        std::string m_imgFile;
        std::stringstream m_imgPath;
        QBrush m_nodeFillBrush;
        QBrush m_layerFillBrush;
        QBrush m_nodeTitleBrush;
        int m_inConnCount;
        int m_outConnCount;
        int m_connCount;
        int m_nodeHeight;
        SceneGraphConnection* m_pInConn;
        SceneGraphConnection* m_pOutConn;
};


// Link

class SceneGraphLink : public QQuickPaintedItem
{
    Q_OBJECT
    
    public:
        SceneGraphLink(SceneGraphConnection* sconn, SceneGraphConnection* tconn, QQuickItem* parent=0);
        ~SceneGraphLink();
        void paint(QPainter* painter);

    private:
        SceneGraphConnection* m_sconnection; // source connection
        SceneGraphConnection* m_tconnection; // target connection
};


// Editor

class SceneGraphEditor : public QQuickPaintedItem
{
    Q_OBJECT
         Q_PROPERTY(FieldModel* connection READ connection WRITE setConnection NOTIFY connectionChanged)
         Q_PROPERTY(int clickX READ clickX)
         Q_PROPERTY(int clickY READ clickY)
 
    public:
        SceneGraphEditor(QQuickItem* parent=0);
        ~SceneGraphEditor();

        void paint(QPainter* painter);
        Q_INVOKABLE void update_sg() { updateGraph(); update(); };
        Q_INVOKABLE void startConnection() { SGState::mode=SGState::FieldConnection; };
        Q_INVOKABLE void connectionMousePressed(int button, unsigned int uid, unsigned int nid, unsigned int fid);
        Q_INVOKABLE void connectionMouseReleased(int button, unsigned int uid, unsigned int nid, unsigned int fid);
        Q_INVOKABLE void connectionMouseClicked(int button, unsigned int uid, unsigned int nid, unsigned int fid);
        Q_INVOKABLE void clearGraph();
        Q_INVOKABLE void updateNodes();
        Q_INVOKABLE bool connectNodes(); // this only works on selected connections in the SGState
        Q_INVOKABLE bool disconnectNodes(); // this only works on selected connections in the SGState

        // connection calls
        Q_INVOKABLE void setConnectionSource(unsigned int uid, unsigned int fid);
        Q_INVOKABLE void setConnectionTarget(unsigned int uid, unsigned int fid);

        // connection 
        void setConnection(FieldModel* c) {
            if(m_connection!= c) {
                m_connection=c;
                connectionChanged();
            }
        }

        FieldModel* connection() { return m_connection; };
        inline int clickX() { return MouseInfo::clickX; };
        inline int clickY() { return MouseInfo::clickY; };

    protected slots:
        void ConnOption(Qt::MouseButton button, SceneGraphConnection::Connection conn, int x, int y, int uid, int nid);
        void nodePressed(Qt::MouseButton button, int uid, int nid);

    protected:
        void mousePressEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);
        void hoverEnterEvent(QHoverEvent* event);
        void hoverLeaveEvent(QHoverEvent* event);
        void hoverMoveEvent(QHoverEvent* event);
        void mouseMoveEvent(QMouseEvent* event);

    signals:
        void sgUpdated();
        void openConnMenu();
        void closeConnMenu();
        void connectionChanged();
        void nodeSelection(int type, int uid, int nid);
        void updateSelection();
        void connectorClicked(int button, int conn, int x, int y, int uid, int nid);
        void statusChanged(int state, QString msg);

    private:
        void updateGraph();
        void updateNode(SceneGraphNode* pnode, int uid, int xpos, int ypos);
        void updateLinks(int uid);
        inline SceneGraphNode* getNode(int uid) { for(auto n : m_nodes){ if(n->uid()==uid){ return n; } } return nullptr; }; /*! Used to get the pointer of a node already in the node draw list. If the node's uid is not in the list a null pointer is returned. */
        int m_scale;
        int m_nodeWidth;
        int m_nodeHeight;
        int m_nodeGapWidth; // horizontal distance between nodes
        int m_nodeGapHeight; // vertical distance between nodes
        std::vector<SceneGraphNode*> m_nodes;
        std::vector<SceneGraphLink*> m_links;
        FieldModel* m_connection;
};

#endif
