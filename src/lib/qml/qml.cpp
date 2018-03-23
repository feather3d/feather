/***********************************************************************
 *
 * Filename: qml.cpp
 *
 * Description: Frontend of the QML wrapper.
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

#include "qml.hpp"
#include "parameter.hpp"
#include "field.hpp"
#include "plugin.hpp"

using namespace feather;

Connection::Connection(QObject* parent)
{
}

Connection::~Connection()
{
}

// SceneGraph
SceneGraph::SceneGraph(QObject* parent)
{
}

SceneGraph::~SceneGraph()
{
}

void SceneGraph::clear()
{
    plugin::clear();

    // we'll add our default nodes that we always need
    status p;
    unsigned int time = plugin::add_node(4,"time",p);
    unsigned int camera = plugin::add_node(2,"camera",p);

    // make connections
    plugin::connect(0,202,time,201);
    plugin::connect(0,202,camera,201);
    
    plugin::update();
    emit cleared();
}

int SceneGraph::add_node(int node, QString name)
{
    int uid = qml::command::add_node(node,name.toStdString());
    emit nodeAdded(uid);
    emit nodeAddDrawItems(uid);
    return uid;
}

bool SceneGraph::remove_node(int uid)
{
    status e;
    qml::command::remove_node(uid,e);
    emit nodeRemoved(uid);
    return e.state;
}

QString SceneGraph::node_name(int uid)
{
    std::string name;
    status e;
    qml::command::get_node_name(uid,name,e);
    return QString(name.c_str());
}

int SceneGraph::get_node_by_name(QString name)
{
    unsigned int uid=0;
    qml::command::get_node_by_name(name.toStdString(),uid);
    return uid;
}

QList<int> SceneGraph::get_nodes_by_type(unsigned int type)
{
    QList<int> list;
    std::vector<unsigned int> uids;
    feather::plugin::get_nodes_by_type(static_cast<node::Type>(type),uids);
    for(auto uid : uids) {
        list.push_back(uid);
    }
    return list;
}

QList<int> SceneGraph::get_nodes_by_id(unsigned int id)
{
    QList<int> list;
    std::vector<unsigned int> uids;
    feather::plugin::get_nodes_by_id(id,uids);
    for(auto uid : uids) {
        list.push_back(uid);
    }
    return list;
}

int SceneGraph::node_id(int uid)
{
    status e;
    return qml::command::get_node_id(uid,e);
}

int SceneGraph::connect_nodes(int n1, int f1, int n2, int f2)
{
    status p = qml::command::connect_nodes(n1,f1,n2,f2);
    //emit nodeUpdateDrawItems(n2);
    if(p.state==FAILED)
        std::cout << p.msg << std::endl;
    return p.state;
}

unsigned int SceneGraph::disconnect_nodes(unsigned int suid, unsigned int sfid, unsigned int tuid, unsigned int tfid)
{
    status p = plugin::disconnect(suid,sfid,tuid,tfid);
    if(p.state==FAILED)
        std::cout << p.msg << std::endl;
    return p.state;
}

int SceneGraph::selected_node()
{
    return qml::command::selected_node();
}
   
int SceneGraph::select_node(unsigned int uid)
{
    plugin::select_node(uid);
    emit nodeSelected();
    return 0;
}

/* 
int SceneGraph::select_node(int type, int uid)
{
    status p = qml::command::select_node(type,uid);
    emit nodeSelected();
    return p.state;
}
*/

int SceneGraph::select_field(unsigned int type, unsigned int uid, unsigned int fid)
{
    plugin::select_node(uid,fid,type);
    emit fieldSelected(uid,fid);
    return 0;
}

void SceneGraph::clear_selection()
{
    qml::command::clear_selection();
}

int SceneGraph::run_command_string(QString str)
{
    status p = qml::command::run_command_string(str.toStdString());
    emit commandMessageChanged(p.state,QString(p.msg.c_str()));
    std::cout << "run command string msg='" << p.msg << "'\n";
    return p.state;
}

void SceneGraph::triggerUpdate()
{
    qml::command::scenegraph_update();
    // some widgets need to know if nodes where added and what was added

    std::vector<unsigned int> uids;
    bool n = feather::qml::command::nodes_added(uids);

    std::cout << "triggerUpdate saw that the following uids where added\n";

    if(n){ 
        for(auto uid : uids)
            emit nodeAdded(uid);
    }

    emit updateGraph();
}

void SceneGraph::add_node_to_layer(int uid, int lid)
{
    qml::command::add_node_to_layer(uid,lid);
}

bool SceneGraph::connected(unsigned int uid, unsigned int fid)
{
    status e;
    bool conn;
    unsigned int nid = qml::command::get_node_id(uid,e);
    qml::command::get_field_connection_status(uid,nid,fid,conn);
    return conn; 
}

QList<int> SceneGraph::connected_uids(unsigned int uid, unsigned int fid)
{
    QList<int> list;
    std::vector<unsigned int> uids;
    feather::plugin::get_node_connected_uids(uid,fid,uids);
    std::cout << "connected_uids(" << uid << "," << fid << ") uids count = " << uids.size() << std::endl;
    for(auto uid : uids) {
        std::cout << "added uid " << uid << " to list\n";
        list.push_back(uid);
    }
    return list;
}


// Key

KeyValue::KeyValue(QObject* parent)
{

}

KeyValue::~KeyValue()
{

}


// Field

Field::Field(QObject* parent): m_uid(0),m_nid(0),m_fid(0),m_boolVal(false),m_intVal(0),m_realVal(0.0),m_connected(false)
{

}

Field::~Field()
{
    m_keyArrayVal.clear();
    m_connections.clear();
}

int Field::type()
{
    int type=0;
    qml::command::get_field_type(m_uid,m_nid,m_fid,type);
    return type;
}

// GET FIELD VALUES

void Field::get_bool_val()
{
    qml::command::get_field_val(m_uid,m_nid,m_fid,m_boolVal);
}

void Field::get_int_val()
{
    qml::command::get_field_val(m_uid,m_nid,m_fid,m_intVal);
}

void Field::get_real_val()
{
    qml::command::get_field_val(m_uid,m_nid,m_fid,m_realVal);
}

// SET FEILD VALUES

void Field::set_bool_val()
{
    qml::command::set_field_val(m_uid,m_nid,m_fid,m_boolVal);
}

void Field::set_int_val()
{
    qml::command::set_field_val(m_uid,m_nid,m_fid,m_intVal);
}

void Field::set_real_val()
{
    qml::command::set_field_val(m_uid,m_nid,m_fid,m_realVal);
}


// Real Array

void Field::set_real_array_val()
{
    FRealArray value;

    int i=0;
    for ( auto val : m_realArrayVal){
        std::cout << "setting weight " << i << " to " << val << std::endl;
        value.push_back(val);
        i++;
    }
    qml::command::set_field_val(m_uid,m_nid,m_fid,value);
}

//QQmlListProperty<double> Field::realArrayVal()
QList<double> Field::realArrayVal()
{
    m_realArrayVal.clear();

    // TODO - put a check here to verify that this is actually a realArray field

    field::Field<FRealArray>* array = static_cast<field::Field<FRealArray>*>(plugin::get_node_field_base(m_uid,m_fid));

    for(auto val : array->value){
        m_realArrayVal.append(val);
    }
 
    return m_realArrayVal;     
    //return QQmlListProperty<double>(this,m_realArrayVal);     

    /*
    std::vector<field::Connection> connections;
    plugin::connections(m_uid,m_fid,connections);
 
    for ( auto connection : connections ) {
        Connection* conn = new Connection();
        conn->setSuid(connection.puid);
        conn->setSfid(connection.pfid);
        conn->setTuid(m_uid);
        conn->setTfid(m_fid);
        m_connections.append(conn);
    }

    return QQmlListProperty<Connection>(this,m_connections);
    */
}


// Key Array

void Field::get_key_array_val()
{
    m_keyArrayVal.clear();

    typedef field::Field<FKeyArray>* KeyArray;
    KeyArray keys = static_cast<KeyArray>(plugin::get_node_field_base(m_uid,m_nid,m_fid));

    for ( auto key : keys->value ) {
        KeyValue* keyvalue = new KeyValue();
        keyvalue->setTime(key.time);
        keyvalue->setValue(key.value);
        m_keyArrayVal.append(keyvalue);
    }

    emit keyArrayValChanged();
}

void Field::set_key_array_val()
{
    FKeyArray value;

    int i=0;
    for ( auto val : m_keyArrayVal ) {
        std::cout << "setting key time:" << val->time() << " value:" << val->value() << std::endl;
        value.push_back(FKey(val->value(),val->time(),static_cast<curve::Type>(val->inCurve()),FPoint2D(val->cp1x(),val->cp1y()),static_cast<curve::Type>(val->outCurve()),FPoint2D(val->cp2x(),val->cp2y())));
        i++;
    }
    qml::command::set_field_val(m_uid,m_nid,m_fid,value);
}

QQmlListProperty<KeyValue> Field::keyArrayVal()
{
    m_keyArrayVal.clear();

    // TODO - put a check here to verify that this is the right type 

    field::Field<FKeyArray>* array = static_cast<field::Field<FKeyArray>*>(plugin::get_node_field_base(m_uid,m_fid));

    //std::cout << "There are " << array->value.size() << " keys in the track\n";

    for ( auto val : array->value ) {
        KeyValue* key = new KeyValue();
        key->setTime(val.time);
        key->setValue(val.value);
        key->setInCurve((int&)val.incurve);
        key->setOutCurve((int&)val.outcurve);
        key->setCp1x(val.incp.x);
        key->setCp1y(val.incp.y);
        key->setCp2x(val.outcp.x);
        key->setCp2y(val.outcp.y);
        /*
        std::cout << "field value - key time:" << val.time << " value:" << val.value << std::endl;
        std::cout << "qml value - key time:" << key->time() << " value:" << key->value() << std::endl;

        std::cout << "Field::keyArrayVal() - time:" << val.time 
            << " value:" << val.value
            << " inCurve:" << val.incurve
            << " cp1x:" << val.incp.x
            << " cp1y:" << val.incp.y
            << " outCurve:" << val.outcurve
            << " cp2x:" << val.outcp.x
            << " cp2y:" << val.outcp.y
            << std::endl
            ;
        */
        connect(key,SIGNAL(timeChanged()),this,SLOT(updateKeyArray()));
        connect(key,SIGNAL(valueChanged()),this,SLOT(updateKeyArray()));
        m_keyArrayVal.append(key);
    }
 
    //return m_keyArrayVal;     
    return QQmlListProperty<KeyValue>(this,m_keyArrayVal);     
}

void Field::updateKeyArray()
{
    std::cout << "updateKeyArray()\n";
    for ( auto key : m_keyArrayVal ) {
        std::cout << "key value:" << key->value() << " time:" << key->time() << std::endl;
    }

    set_key_array_val();
}


// GET CONNECTED

void Field::get_connected()
{
    qml::command::get_field_connection_status(m_uid,m_nid,m_fid,m_connected);
}

QQmlListProperty<Connection> Field::connections()
{
    m_connections.clear();

    std::vector<field::Connection> connections;
    plugin::connections(m_uid,m_fid,connections);
 
    for ( auto connection : connections ) {
        Connection* conn = new Connection();
        conn->setSuid(connection.puid);
        conn->setSfid(connection.pfid);
        conn->setTuid(m_uid);
        conn->setTfid(m_fid);
        m_connections.append(conn);
    }

    return QQmlListProperty<Connection>(this,m_connections);
}

void Field::setFid(unsigned int& fid) {
    if ( m_fid != fid ) {
        m_fid = fid;
    }
}


// Node
Node::Node(QObject* parent):
    m_uid(0),
    m_nid(0)
{
}

Node::~Node()
{
}

/*
QQmlListProperty<Field> Node::inFields()
{
    return QQmlListProperty<Field>(this,0,&Node::append_inField,0,0,0);
}

QQmlListProperty<Field> Node::outFields()
{
    return QQmlListProperty<Field>(this,0,&Node::append_outField,0,0,0);
}

void Node::append_inField(QQmlListProperty<Field> *list, Field *field)
{
    Node *node = qobject_cast<Node*>(list->object);
    if(node) {
        node->m_inFields.append(field);
    }
}

void Node::append_outField(QQmlListProperty<Field> *list, Field *field)
{
    Node *node = qobject_cast<Node*>(list->object);
    if(node) {
        node->m_outFields.append(field);
    }
}
*/

unsigned int Node::field_count()
{
    return qml::command::get_field_count(m_uid); 
}

unsigned int Node::in_field_count()
{
    return qml::command::get_in_field_count(m_uid); 
}

unsigned int Node::out_field_count()
{
    return qml::command::get_out_field_count(m_uid); 
}

QList<unsigned int> Node::in_fields()
{
    std::vector<field::FieldBase*> fids;
    QList<unsigned int> qfids;

    qml::command::get_fid_list(m_uid,m_nid,field::connection::In,fids);
    for(auto f : fids)
        qfids.push_back(f->id);
 
    return qfids;
}

QList<unsigned int> Node::out_fields()
{
    std::vector<field::FieldBase*> fids;
    QList<unsigned int> qfids;

    qml::command::get_fid_list(m_uid,m_nid,field::connection::Out,fids);
    for(auto f : fids)
        qfids.push_back(f->id);
    
    return qfids;
}

// Parameter
Parameter::Parameter(QObject* parent) : QObject(parent)
{
}

Parameter::~Parameter()
{
}


// Command
Command::Command(QObject* parent) : QObject(parent)
{
}

Command::~Command()
{
}

bool Command::exec()
{
    parameter::ParameterList params;
    for(int i=0; i < m_parameters.size(); i++)
    {
        Parameter* p = m_parameters.at(i);

        switch(p->type())
        {
            case Parameter::Bool:
                params.addBoolParameter(p->name().toStdString(), p->boolValue()); 
                break;

            case Parameter::Int:
                params.addIntParameter(p->name().toStdString(), p->intValue()); 
                break;

            case Parameter::Real:
                params.addRealParameter(p->name().toStdString(), p->realValue()); 
                break;

            case Parameter::String:
                params.addStringParameter(p->name().toStdString(), p->stringValue().toStdString()); 
                break;

            default:
                break; 
        }

    }

    qml::command::run_command(m_name.toStdString(), params);  

    return false;
}


QQmlListProperty<Parameter> Command::parameters()
{
    return QQmlListProperty<Parameter>(this,0,&Command::append_parameter,0,0,0);
}

void Command::append_parameter(QQmlListProperty<Parameter> *list, Parameter *parameter)
{
    Command *cmd = qobject_cast<Command*>(list->object);
    if(cmd) {
        cmd->m_parameters.append(parameter);
    }
}

PluginInterface::PluginInterface()
{

}

PluginInterface::~PluginInterface()
{

}

void PluginInterface::load()
{

}

bool PluginInterface::render_globals_interface(const int render_id, QString& path)
{
    return false;
}

// Plugins
Plugins::Plugins(QObject* parent) : QAbstractListModel(parent)
{
    load();
}

Plugins::~Plugins()
{
}

QHash<int, QByteArray> Plugins::roleNames() const
{

    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles.insert(NameRole, QByteArray("name"));
    roles.insert(DescriptionRole, QByteArray("description"));
    roles.insert(AuthorRole, QByteArray("author"));
    return roles;
}

int Plugins::rowCount(const QModelIndex& parent) const
{
    return m_items.size();
}

QVariant Plugins::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant(); // Return Null variant if index is invalid
    if (index.row() > (m_items.size()-1) )
        return QVariant();
    PluginObject *dobj = m_items.at(index.row());
    switch (role) {
        case Qt::DisplayRole: // The default display role now displays the first name as well
        case NameRole:
            return QVariant::fromValue(dobj->name);
        case DescriptionRole:
            return QVariant::fromValue(dobj->description);
        case AuthorRole:
            return QVariant::fromValue(dobj->author);
        default:
            return QVariant();
    }
}

void Plugins::load()
{
    std::vector<PluginInfo> list;
    qml::command::get_plugins(list);

    for(uint i=0; i < list.size(); i++)
        m_items.append(new PluginObject(QString(list[i].name.c_str()), QString(list[i].description.c_str()), QString(list[i].author.c_str())));
}

void Render::render_start(int id) {
    render::RenderProperties props; // this is temp
    qml::command::render_start(id,props);  
}
 
void Render::render_stop(int id) {
    render::RenderProperties props; // this is temp
    qml::command::render_stop(id,props);  
}
 
void Render::render_buffer(int id) {
    // if there is a render image to display, pass it's buffer
    if(m_pImage) {
        qml::command::render_buffer(id,m_pImage->getRenderBuffer());  
        //m_pImage->start_render_update();
        m_pImage->update();
        //m_pImage->stop_render_update();
        /*
        int i = 100;
        while(i<100) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(1));
            i++;
        }
        */
    }
}


// ATTRIBUTE

Attribute::Type Attribute::type() {
    attribute::Type _t;
    status p = qml::command::attribute_type(m_Plugin_Id, m_Attribute_Id, _t);
    return static_cast<Type>(_t);
}


// BOOL

void Attribute::setBval(bool& value) {
    bool bv = bval();
    if(bv != value) {
        status p = qml::command::attribute_set_bool_value(m_Plugin_Id, m_Attribute_Id, value);
        emit bvalChanged(); 
    }
}

bool Attribute::bval() {
    bool value;
    status p = qml::command::attribute_get_bool_value(m_Plugin_Id, m_Attribute_Id, value);
    return value;
}


// UINT

void Attribute::setUival(uint32_t& value) {
    uint32_t uiv = uival();
    if(uiv != value) {
        status p = qml::command::attribute_set_uint_value(m_Plugin_Id, m_Attribute_Id, value);
        emit uivalChanged(); 
    }
}

unsigned int Attribute::uival() {
    uint32_t value;
    status p = qml::command::attribute_get_uint_value(m_Plugin_Id, m_Attribute_Id, value);
    return value;
}

// INT

void Attribute::setIval(int& value) {
    int iv = ival();
    if(iv != value) {
        status p = qml::command::attribute_set_int_value(m_Plugin_Id, m_Attribute_Id, value);
        emit ivalChanged(); 
    }
}

int Attribute::ival() {
    int value;
    status p = qml::command::attribute_get_int_value(m_Plugin_Id, m_Attribute_Id, value);
    return value;
}

// REAL

void Attribute::setRval(double& value) {
    double rv = rval();
    if(rv != value) {
        status p = qml::command::attribute_set_real_value(m_Plugin_Id, m_Attribute_Id, value);
        emit rvalChanged(); 
    }
}

double Attribute::rval() {
    double value;
    status p = qml::command::attribute_get_real_value(m_Plugin_Id, m_Attribute_Id, value);
    return value;
}

// STRING

void Attribute::setSval(QString& value) {
    QString sv = sval();
    if(sv != value) {
        std::string tval = value.toStdString();
        status p = qml::command::attribute_set_string_value(m_Plugin_Id, m_Attribute_Id, tval);
        emit svalChanged(); 
    }
}

QString Attribute::sval() {
    std::string value;
    status p = qml::command::attribute_get_string_value(m_Plugin_Id, m_Attribute_Id, value);
    return QString(value.c_str());
}

