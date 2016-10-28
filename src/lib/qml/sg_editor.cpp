/***********************************************************************
 *
 * Filename: sg_editor.cpp
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

#include "sg_editor.hpp"
#include "commands.hpp"
#include "selection.hpp"
#include "field.hpp"
#include "state.hpp"
#include "plugin.hpp"
#include "qml_status.hpp"

#define BACKGROUND_COLOR "#444444"
#define NODE_TEXT_COLOR "#000000"
#define SELECTED_NODE_COLOR "#FF007F"
#define DESELECTED_NODE_COLOR "#666666"
#define NODE_TITLE_BLOCK_COLOR "#888888"
#define HOVER_NODE_COLOR "#FF8C00"
#define SELECTED_CONNECTOR_COLOR "#FFFF00" // TODO
#define DESELECTED_IN_CONNECTOR_COLOR "#50C878"
#define HOVER_CONNECTOR_COLOR "#FFBF00"
#define DESELECTED_OUT_CONNECTOR_COLOR "#9400D3"
#define SELECTED_CONNECTION_COLOR "#FFEF00"
#define DESELECTED_CONNECTION_COLOR "#99BADD"
#define HOVER_CONNECTION_COLOR ""  // TODO


int MouseInfo::clickX=0;
int MouseInfo::clickY=0;

SGState::Mode SGState::mode=SGState::Normal;
int SGState::srcUid=0;
int SGState::srcNid=0;
int SGState::srcFid=0;
int SGState::tgtUid=0;
int SGState::tgtNid=0;
int SGState::tgtFid=0;
SceneGraphEditor* SGState::pSge=NULL;
std::vector<SceneGraphConnection*> SGState::selectedConnections = std::vector<SceneGraphConnection*>();

// SCENEGRAPH

SceneGraphConnection::SceneGraphConnection(SceneGraphNode* node, SceneGraphConnection::Connection type, QQuickItem* parent) :
    QQuickPaintedItem(parent),
    m_selected(false),
    m_type(type),
    m_node(node)
{
    setWidth(CONNECTION_WIDTH);
    setHeight(CONNECTION_HEIGHT);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);

    if(m_type == In)
        m_connFillBrush = QBrush(QColor(DESELECTED_IN_CONNECTOR_COLOR));
    else
        m_connFillBrush = QBrush(QColor(DESELECTED_OUT_CONNECTOR_COLOR));
}

SceneGraphConnection::~SceneGraphConnection()
{
}

void SceneGraphConnection::paint(QPainter* painter)
{
    painter->setRenderHints(QPainter::Antialiasing, true);
    
    if(!m_selected){
        if(m_type == In)
            m_connFillBrush = QBrush(QColor(DESELECTED_IN_CONNECTOR_COLOR));
        else
            m_connFillBrush = QBrush(QColor(DESELECTED_OUT_CONNECTOR_COLOR));
    } else {
        m_connFillBrush = QBrush(QColor(SELECTED_CONNECTOR_COLOR));
    }
   
    painter->setBrush(m_connFillBrush);


    painter->drawRect(0,0,CONNECTION_WIDTH,CONNECTION_HEIGHT);
}

void SceneGraphConnection::mousePressEvent(QMouseEvent* event)
{
    std::cout << "connector mouse press event\n";
    MouseInfo::clickX = event->windowPos().x();
    MouseInfo::clickY = event->windowPos().y();

    // add to selected connections
    /*
    if(!m_selected) {
        m_selected=true;
        SGState::selectedConnections.push_back(this);
    } else {
        m_selected=false;
        SGState::remove(this);
    }
    */

    connClicked(event->button(),m_type,event->screenPos().x(),event->screenPos().y());
    update();
}

void SceneGraphConnection::mouseReleaseEvent(QMouseEvent* event)
{
    SGState::mode = SGState::Normal;
    //update();
}

void SceneGraphConnection::hoverEnterEvent(QHoverEvent* event)
{
    m_connFillBrush.setColor(QColor(HOVER_CONNECTOR_COLOR));
    //update();
}

void SceneGraphConnection::hoverLeaveEvent(QHoverEvent* event)
{
    if(m_type == In)
        m_connFillBrush = QBrush(QColor(DESELECTED_IN_CONNECTOR_COLOR));
    else
        m_connFillBrush = QBrush(QColor(DESELECTED_OUT_CONNECTOR_COLOR));
    //update();
}

void SceneGraphConnection::mouseMoveEvent(QMouseEvent* event)
{
    std::cout << "connection mouse move event\n";
    MouseInfo::clickX = event->windowPos().x(); 
    MouseInfo::clickY = event->windowPos().y(); 
    //parentItem()->update();
    //SGState::pSge->update();
}


// Node
SceneGraphNode::SceneGraphNode(int uid, int nid, QQuickItem* parent) : 
    QQuickPaintedItem(parent),
    m_uid(uid),
    m_nid(nid),
    m_x(0),
    m_y(0),
    m_imgDir("ui/icons/"),
    m_nodeFillBrush(QBrush(QColor(DESELECTED_NODE_COLOR))),
    m_layerFillBrush(QBrush(QColor("#DDDDDD"))),
    m_nodeTitleBrush(QBrush(QColor(NODE_TITLE_BLOCK_COLOR))),
    m_pInConn(new SceneGraphConnection(this,SceneGraphConnection::In,this)),
    m_pOutConn(new SceneGraphConnection(this,SceneGraphConnection::Out,this))
{
    feather::status e;

    connect(m_pInConn,SIGNAL(connClicked(Qt::MouseButton,SceneGraphConnection::Connection,int,int)),this,SLOT(ConnPressed(Qt::MouseButton,SceneGraphConnection::Connection,int,int)));
    connect(m_pOutConn,SIGNAL(connClicked(Qt::MouseButton,SceneGraphConnection::Connection,int,int)),this,SLOT(ConnPressed(Qt::MouseButton,SceneGraphConnection::Connection,int,int)));

    if(feather::smg::Instance()->selected(m_uid))
        m_nodeFillBrush.setColor(QColor(SELECTED_NODE_COLOR));
    else
        m_nodeFillBrush.setColor(QColor(DESELECTED_NODE_COLOR));

    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);

    m_inConnCount = feather::qml::command::get_in_field_count(m_uid);
    m_outConnCount = feather::qml::command::get_out_field_count(m_uid);
    m_connCount = feather::qml::command::get_field_count(m_uid);

    // set the location of the in and out connections
    m_pInConn->setX(0);
    m_pInConn->setY((NODE_HEIGHT/2)-(CONNECTION_HEIGHT/2));
    m_pOutConn->setX(NODE_WIDTH-CONNECTION_HEIGHT);
    m_pOutConn->setY((NODE_HEIGHT/2)-(CONNECTION_HEIGHT/2));

    feather::qml::command::get_node_icon(m_nid,m_imgFile,e);
    m_imgPath << m_imgDir << m_imgFile;

    setWidth(NODE_WIDTH);
    setHeight(NODE_HEIGHT);
}

SceneGraphNode::~SceneGraphNode()
{
    delete m_pInConn;
    delete m_pOutConn;
}

void SceneGraphNode::ConnPressed(Qt::MouseButton button, SceneGraphConnection::Connection conn, int x, int y)
{
    std::cout << "node got connector event\n";
    ConnClicked(button,conn,x,y,m_uid,m_nid); 
}

void SceneGraphNode::paint(QPainter* painter)
{
    feather::status e;
    painter->setRenderHints(QPainter::Antialiasing, true);
 
    QPen trimPen = QPen(QColor(0,0,0),1);
    QPen textPen = QPen(QColor(NODE_TEXT_COLOR),2);
    QFont textFont("DejaVuSans",9);
    painter->setPen(textPen);
    painter->setFont(textFont);
 
    if(feather::smg::Instance()->selected(m_uid))
        m_nodeFillBrush.setColor(QColor(SELECTED_NODE_COLOR));
    else
        m_nodeFillBrush.setColor(QColor(DESELECTED_NODE_COLOR));

    textFont.setBold((feather::smg::Instance()->selected(m_uid)) ? true : false);

    QBrush connInFillBrush = QBrush(QColor("#FF4500"));
    QBrush connOutFillBrush = QBrush(QColor("#DA70D6"));

    // node trim 
    painter->setPen(trimPen);

    // draw layer bar
    painter->setBrush(m_layerFillBrush);

    // draw the node block
    painter->setBrush(m_nodeFillBrush);
    painter->drawRect(0,0,NODE_WIDTH,NODE_HEIGHT);

    // node icon
    QRectF tgt(NODE_WIDTH - (NODE_HEIGHT+CONNECTION_WIDTH),2,NODE_HEIGHT-4,NODE_HEIGHT-4);
    QImage img(m_imgPath.str().c_str());
    painter->drawImage(tgt,img);

    // node label 
    painter->setPen(textPen);
    painter->setFont(textFont);
    std::string name;
    feather::qml::command::get_node_name(m_uid,name,e);
    painter->drawText(QRect(CONNECTION_WIDTH,0,NODE_WIDTH-(NODE_HEIGHT+CONNECTION_WIDTH),NODE_HEIGHT),Qt::AlignHCenter|Qt::AlignVCenter,name.c_str());

}

void SceneGraphNode::mousePressEvent(QMouseEvent* event)
{
    m_x = event->screenPos().x();
    m_y = event->screenPos().y();
}

void SceneGraphNode::mouseDoubleClickEvent(QMouseEvent* event)
{
    emit nodePressed(event->button(),m_uid,m_nid);
}

void SceneGraphNode::mouseReleaseEvent(QMouseEvent* event)
{
    // this is triggered but not currently used
}

void SceneGraphNode::hoverEnterEvent(QHoverEvent* event)
{
    m_nodeFillBrush.setColor(QColor(HOVER_NODE_COLOR));
    update();
}

void SceneGraphNode::hoverLeaveEvent(QHoverEvent* event)
{
    if(feather::smg::Instance()->selected(m_uid)){
        m_nodeFillBrush.setColor(QColor(SELECTED_NODE_COLOR));
    } else {
        m_nodeFillBrush.setColor(QColor(DESELECTED_NODE_COLOR));
    }
    update();
}

void SceneGraphNode::mouseMoveEvent(QMouseEvent* event)
{
    setX(x() + (event->screenPos().x() - m_x));
    setY(y() + (event->screenPos().y() - m_y));
    m_x = event->screenPos().x();
    m_y = event->screenPos().y();
    parentItem()->update();
}

void SceneGraphNode::inConnectionPoint(QPointF& point)
{
    point = mapToItem(parentItem(),QPoint(0,(NODE_HEIGHT/2)));
}

void SceneGraphNode::outConnectionPoint(QPointF& point)
{
    point = mapToItem(parentItem(),QPoint(NODE_WIDTH,(NODE_HEIGHT/2)));
}


void SceneGraphNode::getConnectionPoint(feather::field::connection::Type conn, QPoint& npoint, QPoint& cpoint)
{
    if(conn == feather::field::connection::In)
    {
        cpoint.setX(npoint.x());
        cpoint.setY((npoint.y())+((NODE_HEIGHT)/2));
 
    }
    else
    {
        cpoint.setX((npoint.x())+(NODE_WIDTH));
        cpoint.setY((npoint.y())+((NODE_HEIGHT)/2));
    }
}

SceneGraphConnection* SceneGraphNode::inConnection()
{
    return m_pInConn;
}

SceneGraphConnection* SceneGraphNode::outConnection()
{
    return m_pOutConn;
}

// Link

SceneGraphLink::SceneGraphLink(SceneGraphConnection* sconn, SceneGraphConnection* tconn, QQuickItem* parent) :
QQuickPaintedItem(parent),
m_sconnection(sconn),
m_tconnection(tconn)
{

}

SceneGraphLink::~SceneGraphLink()
{

}

void SceneGraphLink::paint(QPainter* painter)
{
    QPainterPath path;
    QBrush brush = painter->brush();
    brush.setStyle(Qt::NoBrush);

    QPointF sp;
    QPointF tp;
    m_sconnection->node()->outConnectionPoint(sp);
    m_tconnection->node()->inConnectionPoint(tp);

    QPen pathPen;
    if(SGState::mode==SGState::Normal)
        pathPen = QPen(QColor(DESELECTED_CONNECTION_COLOR),1);
    else
        pathPen = QPen(QColor(SELECTED_CONNECTION_COLOR),2);

    path.moveTo(sp.x(),sp.y());

    int midSX = sp.x() + (abs(tp.x()-sp.x())/2);
    int midTX = tp.x() - (abs(tp.x()-sp.x())/2);

    if(SGState::mode==SGState::Normal)
        path.cubicTo(midSX,sp.y(),
                midTX,tp.y(),
                tp.x(),tp.y());
    else 
        path.cubicTo(MouseInfo::clickX,sp.y(),
                midTX,MouseInfo::clickY-35,
                MouseInfo::clickX-2,MouseInfo::clickY-35);

    painter->setPen(pathPen);
    painter->drawPath(path);
}


// Editor

SceneGraphEditor::SceneGraphEditor(QQuickItem* parent) : 
    QQuickPaintedItem(parent),
    m_scale(100),
    m_nodeWidth(80),
    m_nodeHeight(30),
    m_nodeGapWidth(NODE_WIDTH+40),
    m_nodeGapHeight(10)
{
    SGState::pSge = this;
    setAcceptedMouseButtons(Qt::AllButtons);

    // for testing purposes I'm selecting the node from here.
    // later this will be done from the viewport or outliner
    feather::qml::command::select_node(0);
    updateGraph();
}

SceneGraphEditor::~SceneGraphEditor()
{
    clearGraph();
}

void SceneGraphEditor::ConnOption(Qt::MouseButton button, SceneGraphConnection::Connection conn, int x, int y, int uid, int nid)
{
    std::cout << "sg editor got node connector event\n";
    connectorClicked(button,conn,x,y,uid,nid);
}

void SceneGraphEditor::nodePressed(Qt::MouseButton button, int uid, int nid)
{
    // for now we'll have it so only one node can be selected at a time
    feather::qml::command::clear_selection();
    feather::qml::command::select_node(uid);
    emit nodeSelection(0,uid,nid);
    updateNodes();
}

void SceneGraphEditor::connectionMousePressed(int button, unsigned int uid, unsigned int nid, unsigned int fid)
{
    std::cout << "connection mouse pressed, button " << button << " uid " << uid << " nid " << nid << " fid " << fid << std::endl;
}

void SceneGraphEditor::connectionMouseReleased(int button, unsigned int uid, unsigned int nid, unsigned int fid)
{
    std::cout << "connection mouse released, button " << button << " uid " << uid << " nid " << nid << " fid " << fid << std::endl;
}

void SceneGraphEditor::connectionMouseClicked(int button, unsigned int uid, unsigned int nid, unsigned int fid)
{
    std::cout << "connection mouse clicked, button " << button << " uid " << uid << " nid " << nid << " fid " << fid << std::endl;
    
    if(SGState::mode==SGState::Normal)
        SGState::mode=SGState::FieldConnection;
    else
        SGState::mode=SGState::Normal;
}

void SceneGraphEditor::clearGraph()
{
    std::for_each(m_links.begin(), m_links.end(), [](SceneGraphLink* link){ delete link; });
    m_links.clear();
    std::for_each(m_nodes.begin(), m_nodes.end(), [](SceneGraphNode* node){ delete node; });
    m_nodes.clear();
}

void SceneGraphEditor::updateNodes()
{
    for(auto n : m_nodes)
        n->update();
}

bool SceneGraphEditor::connectNodes()
{
    if(SGState::selectedConnections.size() > 1){
        // for now we'll just connect the inputs to the first in connection we see
        SceneGraphConnection* in = nullptr;

        // find the first in connection
        for(auto c : SGState::selectedConnections) {
            if(c->type() == SceneGraphConnection::In) {
                in = c;
                break;
            }
        }

        if(in != nullptr) {
            for(auto c : SGState::selectedConnections) {
                if(c->type() == SceneGraphConnection::Out) {
                    m_links.push_back(new SceneGraphLink(in,c,this));
                 }
            }
        }
    }

    // unselect all the currently selected connections
    for(auto c : SGState::selectedConnections)
        c->setSelected(false);

    SGState::selectedConnections.erase(SGState::selectedConnections.begin(),SGState::selectedConnections.end());

    feather::qml::command::scenegraph_update();

    update();
    return true;
}

bool SceneGraphEditor::disconnectNodes()
{

    return false;
}

// TODO - both of these functions will need to be modified later so that multiple
// source and target fields can be selected so that the user doesn't need to keep
// going back and redoing the process multiple times.
void SceneGraphEditor::setConnectionSource(unsigned int uid, unsigned int fid)
{
    SGState::srcUid = uid;
    SGState::srcFid = fid;
    status p;

    // if there is a target already selected, connect
    if(SGState::tgtUid){
        std::cout << "CONNECTING Source->Target"
            << " suid:" << SGState::srcUid
            << " sfid:" << SGState::srcFid
            << " tuid:" << SGState::tgtUid
            << " tfid:" << SGState::tgtFid
            << std::endl;

        p = feather::plugin::connect(
                SGState::srcUid,
                SGState::srcFid,
                SGState::tgtUid,
                SGState::tgtFid
                );
        // clear out selections
        SGState::srcUid = 0;
        SGState::srcFid = 0;
        SGState::tgtUid = 0;
        SGState::tgtFid = 0;
        Status out;
    }
    statusChanged(p.state,QString(p.msg.c_str()));
};

void SceneGraphEditor::setConnectionTarget(unsigned int uid, unsigned int fid)
{
    SGState::tgtUid = uid;
    SGState::tgtFid = fid;

    status p;

    // if there is a target already selected, connect
    if(SGState::srcUid){
        std::cout << "CONNECTING Target->Source"
            << " suid:" << SGState::srcUid
            << " sfid:" << SGState::srcFid
            << " tuid:" << SGState::tgtUid
            << " tfid:" << SGState::tgtFid
            << std::endl;

        p = feather::plugin::connect(
                SGState::srcUid,
                SGState::srcFid,
                SGState::tgtUid,
                SGState::tgtFid
                );
        // clear out selections
        SGState::srcUid = 0;
        SGState::srcFid = 0;
        SGState::tgtUid = 0;
        SGState::tgtFid = 0;
    }
    std::cout << "STATUS MESSAGE:" << p.msg << std::endl;

    statusChanged(p.state,QString(p.msg.c_str()));
};

void SceneGraphEditor::paint(QPainter* painter)
{
    setFillColor(QColor(BACKGROUND_COLOR));
    painter->setRenderHints(QPainter::Antialiasing, true);

    std::for_each(m_links.begin(),m_links.end(),[painter](SceneGraphLink* l){ l->paint(painter); });
}

void SceneGraphEditor::updateGraph()
{
    int xpos = 50;
    int ypos = 50;

    clearGraph();

    std::vector<int> uids;

    // disabled selection as root for testing
    //feather::qml::command::get_selected_nodes(uids);
    uids.push_back(0);

    std::cout << uids.size() << " nodes are selected\n";

    std::for_each(uids.begin(),uids.end(),[](int& n){ std::cout << n << ","; });

    updateNode(nullptr,0,xpos,ypos);
    for(auto n : m_nodes) 
        updateLinks(n->uid());
}


void SceneGraphEditor::updateNode(SceneGraphNode* pnode, int uid, int xpos, int ypos)
{
    feather::status e;
    unsigned int nid = feather::qml::command::get_node_id(uid,e);

    // if the node is already in the draw list, don't add a new one
    SceneGraphNode *node = getNode(uid);
    if(!node){
        std::cout << "ADDING NODE TO SG EDITOR\n";
        node = new SceneGraphNode(uid,nid,this);
        m_nodes.push_back(node);
        // setup the node qt connections
        connect(node,SIGNAL(ConnClicked(Qt::MouseButton,SceneGraphConnection::Connection,int,int,int,int)),this,SLOT(ConnOption(Qt::MouseButton,SceneGraphConnection::Connection,int,int,int,int)));
        connect(node,SIGNAL(nodePressed(Qt::MouseButton,int,int)),this,SLOT(nodePressed(Qt::MouseButton,int,int)));

        // place the node in the scenegraph
        node->setX(xpos);
        node->setY(ypos);

        // get the connected nodes
        std::vector<unsigned int> cuids;
        feather::qml::command::get_node_connected_uids(uid,cuids);

        // add a link for each connection between the two nodes
        int ystep=0;
        for(auto c : cuids) {
            // add the child node to draw list and get it's links
            updateNode(node, c, xpos+m_nodeGapWidth, ypos+ystep);
            ystep+=node->height()+m_nodeGapHeight;
        } 

    }

}

void SceneGraphEditor::updateLinks(int uid)
{
    // draw all links between the uid and it's children nodes
    
    // get the node
    SceneGraphNode* pnode = nullptr;
    for(auto n : m_nodes){
        if(n->uid() == uid)
            pnode=n;
    }

    // no point in going any further if no node was found
    if(!pnode)
        return;

    // get all the node's output connections
    std::vector<unsigned int> uids;
    feather::qml::command::get_node_connected_uids(uid,uids); 

    // if there is no connected nodes, no need to go any further
    if(!uids.size())
        return;

    // some of the connected uids are not shown in sg editor, so remove them from the list
    std::vector<int> shownuids;

    for(auto tuid : uids){
        for(auto n : m_nodes){
            if(n->uid()==tuid)
                shownuids.push_back(tuid);
        } 
    }

    // if none of the uids are displayed, no need to go any further
    if(!shownuids.size())
        return;

    // draw links between the nodes that connected to it
    for(auto tuid : shownuids){
        // is the field connected
        bool connected = feather::plugin::get_node_connection_status(uid,tuid);
        if(connected){
            // add the link
            m_links.push_back(new SceneGraphLink(pnode->outConnection(),getNode(tuid)->inConnection(),this));
        }
    }
}

void SceneGraphEditor::mousePressEvent(QMouseEvent* event){ std::cout << "mouse press\n"; };
void SceneGraphEditor::mouseReleaseEvent(QMouseEvent* event){ std::cout << "mouse release\n"; };
void SceneGraphEditor::hoverEnterEvent(QHoverEvent* event){ std::cout << "hover enter\n"; };
void SceneGraphEditor::hoverLeaveEvent(QHoverEvent* event){ std::cout << "hover leave\n"; };
void SceneGraphEditor::hoverMoveEvent(QHoverEvent* event){ std::cout << "hover move\n"; };
void SceneGraphEditor::mouseMoveEvent(QMouseEvent* event){ std::cout << "mouse move\n"; };
