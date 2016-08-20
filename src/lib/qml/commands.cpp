/***********************************************************************
 *
 * Filename: commands.cpp
 *
 * Description: Used to give qml access to the core.
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

#include "commands.hpp"
#include "field.hpp"
#include "scenegraph.hpp"
#include "selection.hpp"

using namespace feather;

static PluginManager plugins;

status qml::command::init() {
    load_plugins();
    cstate.sgState.minUid=0;
    cstate.sgState.maxUid=0;

    status e;
    int root = add_node(1,"root"); // Empty 
    int time = add_node(4,"time"); // Time 
    int camera = add_node(2,"camera"); // Camera 
    connect_nodes(root,2,time,1);
    connect_nodes(root,2,camera,1);

    //int uid1 = add_node(320,"CubeShape"); // PolyShape
    //scenegraph::connect(0,4,uid1,1); // connect PolyCube.out to PolyShape.in

    smg::Instance()->add_state(selection::Node,0,0,0);
 
    // just testing the do_it plugin calls
    cstate.sgMode = state::DoIt;
    scenegraph::update();
    scenegraph::nodes_updated(); // clear out the uids to update
    cstate.sgMode = state::None;
    return status();
}

unsigned int qml::command::add_node(const unsigned int nid, const std::string name)
{
    status e;
    unsigned int uid =  scenegraph::add_node(nid,name,e);
    cstate.sgState.maxUid = uid;
    return uid;
    /*
    switch(type)
    {
        case node::Null:
            return scenegraph::add_node_to_sg<node::Null,null::N>::exec(node,id);
        case node::Camera:
            return scenegraph::add_node_to_sg<node::Camera,camera::N>::exec(node,id);
        case node::Light:
            return scenegraph::add_node_to_sg<node::Light,light::N>::exec(node,id);
        case node::Texture:
            return scenegraph::add_node_to_sg<node::Texture,texture::N>::exec(node,id);
        case node::Shader:
            return scenegraph::add_node_to_sg<node::Shader,shader::N>::exec(node,id);
        case node::Object:
            return scenegraph::add_node_to_sg<node::Object,object::N>::exec(node,id);
        default:
            break;
    }
    */

    // need to put a check here 
    //return status();
}

bool qml::command::nodes_added(std::vector<unsigned int>& uids)
{
    uids.assign(cstate.uid_update.begin(),cstate.uid_update.end());
    //cstate.clear_uid_update();

    std::cout << "there are " << cstate.uid_update.size() << "  in the added nodes\n";

    if(!uids.size())
        return false;
    
    return true;
}

void qml::command::remove_node(const unsigned int uid, status& error)
{
    scenegraph::remove_node(uid,error);
}

void qml::command::nodes_updated()
{
    scenegraph::nodes_updated();
}

bool qml::command::node_exists(int uid)
{
    // TODO - this is a temp fix for the minute
    // This will need to be fixed in the future since nodes will be taken away
    // so the indics could jump
    return ( scenegraph::get_max_uid() >= uid) ? true : false;
}

status qml::command::connect_nodes(int n1, int f1, int n2, int f2)
{
    // TODO - check to see if update() is needed
    status p = scenegraph::connect(n1,f1,n2,f2);
    if(!p.state)
        std::cout << "CONNECTION FAILURE:\n" << p.msg << std::endl;
    //scenegraph::update();
    return p;
}

void qml::command::get_node_icon(const unsigned int nid, std::string& file, status& e)
{
    scenegraph::get_node_icon(nid,file,e);
}

void qml::command::get_node_by_name(std::string name, unsigned int& uid)
{
    scenegraph::get_node_by_name(name,uid);
}

void qml::command::get_node_by_type(node::Type type, std::vector<unsigned int>& uids)
{
    scenegraph::get_node_by_type(type,uids);
}

unsigned int qml::command::get_node_id(const unsigned int uid, status& e)
{
    return scenegraph::get_node_id(uid,e);
}

status qml::command::get_node_connected_uids(int uid, std::vector<int>& uids)
{
    return scenegraph::get_node_connected_uids(uid,uids);
}

status qml::command::get_node_connected_uids(int uid, int fid, std::vector<int>& uids)
{
    return scenegraph::get_node_connected_uids(uid,fid,uids);
}

status qml::command::get_node_draw_items(int nid, draw::DrawItems& items)
{
    return scenegraph::get_node_draw_items(nid,items);
}

status qml::command::load_plugins()
{
    return scenegraph::load_plugins(); 
}

status qml::command::run_command(std::string cmd, feather::parameter::ParameterList params)
{
    return plugins.run_command(cmd, params);
}

status qml::command::run_command_string(std::string str)
{
    return plugins.run_command_string(str);
}


// Layer

status qml::command::add_layer(feather::FLayer layer)
{
    scenegraph::add_layer(layer);
    return status();
}

status qml::command::remove_layer(int lid)
{
    bool p = scenegraph::remove_layer(lid);
    return (!p) ? status(FAILED,"failed to remover layer: id out of range") : status();
}

status qml::command::move_layer(int sid, int tid)
{
    scenegraph::move_layer(sid,tid);
    return status();
}

status qml::command::get_layer(int lid, feather::FLayer &layer)
{
    bool p = scenegraph::layer(lid,layer);
    return (!p) ? status(FAILED,"failed to get layer: id out of range") :status();
}

void qml::command::set_layer_name(std::string name, int lid)
{
    scenegraph::layer(lid)->name = name;
}

void qml::command::set_layer_color(int r, int g, int b, int lid)
{
    scenegraph::layer(lid)->color = FColorRGB(static_cast<float>(r)/255.0,static_cast<float>(g)/255.0,static_cast<float>(b)/255.0);
}

void qml::command::set_layer_visible(bool v, int lid)
{
    scenegraph::layer(lid)->visible = v;
}

void qml::command::set_layer_locked(bool l, int lid)
{
    scenegraph::layer(lid)->locked = l;
}

int qml::command::layer_count()
{
    return scenegraph::layer_count();
}

void qml::command::add_node_to_layer(int uid, int lid)
{
    scenegraph::add_node_to_layer(uid,lid);
}


// FIELDS

// GET FIELD BASE
// This returns the nodes field base class if it's not connected.
// If the node is connected, the parent field base is returned.
status qml::command::get_field_base(unsigned int uid, unsigned int fid, feather::field::FieldBase* &f, unsigned int conn)
{
    f = scenegraph::get_fieldBase(uid,sg[uid].node,fid,conn);
    if(!f) {
        return status(FAILED,"null field base\n");
    }
    return status();
} 

// same as above but is used to get all the fields connected to an array input
status qml::command::get_field_base_array(unsigned int uid, unsigned int fid, std::vector<feather::field::FieldBase*> &f)
{
    f = scenegraph::get_fieldBase_array(uid,sg[uid].node,fid);
    if(!f.size()) {
        return status(FAILED,"no connected fields\n");
    }
    return status();
} 


// This is the same as get_field_base except it always returns the node's field, even if it's connected
status qml::command::get_node_field_base(unsigned int uid, unsigned int fid, feather::field::FieldBase* &f)
{
    //typedef field::Field<int>* fielddata;
    //fielddata f = static_cast<fielddata>(scenegraph::get_fieldBase(uid,node,field));
    f = scenegraph::get_node_fieldBase(uid,sg[uid].node,fid);
    if(!f) {
        return status(FAILED,"null field base\n");
    }
    return status();
} 


status qml::command::get_field_type(unsigned int uid, unsigned int node, unsigned int field, int& val)
{
    val = scenegraph::get_node_fieldBase(uid,node,field)->type;
    return status();
}


// GET FIELD VALUE

// bool
status qml::command::get_field_val(unsigned int uid, unsigned int nid, unsigned int fid, bool& val, unsigned int conn)
{
    val=false;
    
    return status();
}

// int
status qml::command::get_field_val(unsigned int uid, unsigned int nid, unsigned int fid, int& val, unsigned int conn)
{
    typedef field::Field<int>* fielddata;
    fielddata f = static_cast<fielddata>(scenegraph::get_fieldBase(uid,nid,fid,conn));
    if(!f)
        std::cout << uid << "," << nid << "," << fid << " NULL INT FIELD\n";
    else  
        val=f->value;
    return status();
}

// real 
status qml::command::get_field_val(unsigned int uid, unsigned int nid, unsigned int fid, FReal& val, unsigned int conn)
{
    typedef field::Field<FReal>* fielddata;
    fielddata f = static_cast<fielddata>(scenegraph::get_fieldBase(uid,nid,fid,conn));
    if(!f)
        std::cout << uid << "," << nid << "," << fid << " NULL REAL FIELD\n";
    else  
        val=f->value;
    return status();
}

// FMesh
status qml::command::get_field_val(unsigned int uid, unsigned int nid, unsigned int fid, FMesh& val, unsigned int conn)
{
    typedef field::Field<FMesh>* fielddata;
    fielddata f = static_cast<fielddata>(scenegraph::get_fieldBase(uid,nid,fid,conn));
    if(!f)
        std::cout << uid << "," << nid << "," << fid << " NULL FIELD\n";
    else  
        val=f->value;
    return status();
}


// SET FIELD VALUE

// bool
status qml::command::set_field_val(unsigned int uid, unsigned int nid, unsigned int fid, bool& val)
{
    return status();
}

// int
status qml::command::set_field_val(unsigned int uid, unsigned int nid, unsigned int fid, int& val)
{
    typedef field::Field<int>* fielddata;
    fielddata f = static_cast<fielddata>(scenegraph::get_node_fieldBase(uid,nid,fid));
    if(!f)
        std::cout << "NULL INT FIELD\n";
    else { 
        f->value=val;
        f->update=true;
        scenegraph::update();
    } 
    return status();
}

// real 
status qml::command::set_field_val(unsigned int uid, unsigned int nid, unsigned int fid, FReal& val)
{
    typedef field::Field<FReal>* fielddata;
    fielddata f = static_cast<fielddata>(scenegraph::get_node_fieldBase(uid,nid,fid));
    if(!f)
        std::cout << "NULL REAL FIELD\n";
    else {
        std::cout << "setting real value for uid:" << uid << " nid:" << nid << " fid:" << fid << " value:" << val << std::endl; 
        f->value=val;
        f->update=true;
        scenegraph::update();
    }
    return status();
}


status qml::command::get_field_connection_status(int uid, int field, bool& val)
{
    field::FieldBase* f = scenegraph::get_node_fieldBase(uid,field);
    val = f->connected();
    return status();
}

status qml::command::get_field_connection_status(int uid, int node, int field, bool& val)
{
    field::FieldBase* f = scenegraph::get_node_fieldBase(uid,node,field);
    val = f->connected();
    return status();
}

status qml::command::get_field_connection_status(int suid, int sfid, int tuid, int tfid, bool& val)
{
    field::FieldBase* f = scenegraph::get_node_fieldBase(tuid,tfid);

    val = false;

    if(f->connected())
        for(auto conn : f->connections){
            if( conn.puid == suid && conn.pfid == sfid)
                val = true;
        }

    return status();
}

// TODO the suid and sfid needs to be changed to a vector for array type support
status qml::command::get_connected_fid(unsigned int uid, unsigned int fid, unsigned int& suid, unsigned int& sfid, unsigned int conn)
{
    // if nothing is connected, set the source to 0
    field::FieldBase* f = scenegraph::get_node_fieldBase(uid,fid);
    if(!f->connected()){
        suid=0;
        sfid=0;
        return status(FAILED,"nothing connected to node's fid");
    }
    // for the time being just return the first connection
    suid = f->connections.at(conn).puid;
    sfid = f->connections.at(conn).pfid;
    return status();
}

status qml::command::get_fid_list(int uid, int nid, field::connection::Type conn, std::vector<field::FieldBase*>& list)
{
    return scenegraph::get_fid_list(uid,nid,conn,list);
}

int qml::command::get_field_count(int uid)
{
    return scenegraph::get_field_count(uid);
}

int qml::command::get_in_field_count(int uid)
{
    return scenegraph::get_in_field_count(uid);
}

int qml::command::get_out_field_count(int uid)
{
    return scenegraph::get_out_field_count(uid);
}

// Field Connections

feather::field::connection::Type qml::command::get_field_connection_type(int uid, int fid)
{
    return scenegraph::get_field_connection_type(uid,fid);
}


// SCENEGRAPH EDITOR

void qml::command::clear()
{
    scenegraph::clear();
}

void qml::command::get_node_out_connections(const unsigned int uid, std::vector<unsigned int>& uids)
{
    scenegraph::get_node_out_connections(uid,uids);
}

int qml::command::get_node_connection_count(int uid)
{
    // TODO - currently not used but would be helpful
    return 0; 
}

void qml::command::get_node_name(const unsigned int uid, std::string& name, status& error)
{
    scenegraph::get_node_name(uid,name,error);
}

void qml::command::scenegraph_update()
{
    scenegraph::update();
}

int qml::command::get_min_uid()
{
    return cstate.sgState.minUid; 
}

int qml::command::get_max_uid()
{
    return cstate.sgState.maxUid; 
}

void qml::command::get_plugins(std::vector<PluginInfo>& list)
{
    return plugins.loaded_plugins(list); 
}

// SG SELECTION

int qml::command::selected_node()
{
    return scenegraph::get_selected_node();
}

status qml::command::select_node(int uid)
{
    return scenegraph::add_selection(uid);
}

status qml::command::select_node(int type, int uid)
{
    return scenegraph::add_selection(type,uid);
}

status qml::command::select_node(int type, int uid, int nid)
{
    return scenegraph::add_selection(type,uid,nid);
}

status qml::command::select_node(int type, int uid, int nid, int fid)
{
    return scenegraph::add_selection(type,uid,nid,fid);
}

status qml::command::get_selected_nodes(std::vector<int>& uids)
{
    return scenegraph::get_selected_nodes(uids);
}

void qml::command::clear_selection()
{
    scenegraph::clear_selection();
}

