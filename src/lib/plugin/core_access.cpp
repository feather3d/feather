// =====================================================================================
// 
//       Filename:  core_access.cpp
// 
//    Description:  Gives access to core calls. 
// 
//        Version:  1.0
//        Created:  08/08/2016 09:36:19 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Richard Layman (), rlayman2000@yahoo.com
//        Company:  
// 
// =====================================================================================

#include "core_access.hpp"
#include "scenegraph.hpp"
#include "field.hpp"

using namespace feather;

void api::clear()
{
    scenegraph::clear();
}

unsigned int api::get_min_uid()
{
    return scenegraph::get_min_uid();
}

unsigned int api::get_max_uid()
{
    return scenegraph::get_max_uid();
}

bool api::node_exist(unsigned int uid)
{
    return scenegraph::node_exist(uid);
}

unsigned int api::add_node(unsigned int nid, std::string name, status& error)
{
    return scenegraph::add_node(nid,name,error);
}

void api::remove_node(unsigned int uid, status& error)
{
    scenegraph::remove_node(uid,error);
}

void api::get_node_out_connections(unsigned int uid, std::vector<unsigned int>& uids)
{
    scenegraph::get_node_out_connections(uid,uids);
}

void api::get_nodes(std::vector<unsigned int>& uids)
{
    scenegraph::get_nodes(uids);
}

void api::get_node_by_name(std::string name, unsigned int& uid)
{
    scenegraph::get_node_by_name(name,uid);
}

void api::get_node_by_type(node::Type type, std::vector<unsigned int>& uids)
{
    scenegraph::get_node_by_type(type,uids);
}

void api::get_node_name(unsigned int uid, std::string& name, status& error)
{
    scenegraph::get_node_name(uid,name,error);
}

void api::get_node_icon(unsigned int nid, std::string& file, status& error)
{
    scenegraph::get_node_icon(nid,file,error);
}

unsigned int api::get_node_id(unsigned int uid, status& error)
{
    return scenegraph::get_node_id(uid,error);
}

status api::get_node_connected_uids(unsigned int uid, std::vector<int>& uids)
{
    return scenegraph::get_node_connected_uids(uid,uids);
}

status api::get_node_connected_uids(unsigned int uid, unsigned int fid, std::vector<int>& uids)
{
    return scenegraph::get_node_connected_uids(uid,fid,uids);
}

// FIELD DATA

field::FieldBase* api::get_field_base(unsigned int uid, unsigned int nid, unsigned int fid)
{
    return scenegraph::get_fieldBase(uid,nid,fid);
}

field::FieldBase* api::get_field_base(unsigned int uid, unsigned int fid)
{
    return scenegraph::get_fieldBase(uid,fid);
}

field::FieldBase* api::get_node_field_base(unsigned int uid, unsigned int nid, unsigned int fid)
{
    return scenegraph::get_node_fieldBase(uid,nid,fid);
}

field::FieldBase* api::get_node_field_base(unsigned int uid, unsigned int fid)
{
    return scenegraph::get_node_fieldBase(uid,fid);
}

unsigned int api::get_field_count(unsigned int uid)
{
    return scenegraph::get_field_count(uid);
}

unsigned int api::get_in_field_count(unsigned int uid)
{
    return scenegraph::get_in_field_count(uid);
}

unsigned int api::get_out_field_count(unsigned int uid)
{
    return scenegraph::get_out_field_count(uid);
}

status api::get_in_fields(unsigned int uid, std::vector<unsigned int>& fids)
{
    return scenegraph::get_in_fields(uid,fids);
}

status api::get_out_fields(unsigned int uid, std::vector<unsigned int>& fids)
{
    return scenegraph::get_out_fields(uid,fids);
}

field::connection::Type api::get_field_connection_type(unsigned int uid, unsigned int fid)
{
    return scenegraph::get_field_connection_type(uid,fid);
}

