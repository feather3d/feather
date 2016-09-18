/***********************************************************************
 *
 * Filename: plugin.cpp
 *
 * Description: Gives plugin functions easy access to scenegraph data.
 *
 * Copyright (C) 2016 Richard Layman, rlayman2000@yahoo.com 
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

#include "plugin.hpp"
#include "core_access.hpp"

using namespace feather;

void plugin::clear()
{
    api::clear();
}

void plugin::update()
{
    api::update();
}

unsigned int plugin::get_min_uid()
{
    return api::get_min_uid();
}

unsigned int plugin::get_max_uid()
{
    return api::get_max_uid();
}

bool plugin::node_exist(unsigned int uid)
{
    return api::node_exist(uid);
}

unsigned int plugin::add_node(unsigned int nid, std::string name, status& error)
{
    return api::add_node(nid,name,error);
}

void plugin::remove_node(unsigned int uid, status& error)
{
    api::remove_node(uid,error);
}

void plugin::get_node_out_connections(unsigned int uid, std::vector<unsigned int>& uids)
{
    api::get_node_out_connections(uid,uids);
}

void plugin::get_nodes(std::vector<unsigned int>& uids)
{
    api::get_nodes(uids);
}

void plugin::get_node_by_name(std::string name, unsigned int& uid)
{
    api::get_node_by_name(name,uid);
}

void plugin::get_node_by_type(node::Type type, std::vector<unsigned int>& uids)
{
    api::get_node_by_type(type,uids);
}

void plugin::get_node_name(unsigned int uid, std::string& name, status& error)
{
    api::get_node_name(uid,name,error);
}

void plugin::get_node_icon(unsigned int nid, std::string& file, status& error)
{
    api::get_node_icon(nid,file,error);
}

unsigned int plugin::get_node_id(unsigned int uid, status& error)
{
    return api::get_node_id(uid,error);
}

std::vector<unsigned int> plugin::get_selected_nodes()
{
    return api::get_selected_nodes();
}

unsigned int plugin::get_node_type(unsigned int uid)
{
    return api::get_node_type(uid);
}

status plugin::get_node_connected_uids(unsigned int uid, std::vector<int>& uids)
{
    return api::get_node_connected_uids(uid,uids);
}

status plugin::get_node_connected_uids(unsigned int uid, unsigned int fid, std::vector<int>& uids)
{
    return api::get_node_connected_uids(uid,fid,uids);
}

bool plugin::get_node_connection_status(unsigned int suid, unsigned int tuid)
{
    return api::get_node_connection_status(suid,tuid);
}

// FIELD DATA

field::FieldBase* plugin::get_field_base(unsigned int uid, unsigned int nid, unsigned int fid, unsigned int conn)
{
    return api::get_field_base(uid,nid,fid,conn);
}

std::vector<field::FieldBase*> plugin::get_field_base_array(unsigned int uid, unsigned int nid, unsigned int fid)
{
    return api::get_field_base_array(uid,nid,fid);
}


field::FieldBase* plugin::get_field_base(unsigned int uid, unsigned int fid)
{
    return api::get_field_base(uid,fid);
}

field::FieldBase* plugin::get_node_field_base(unsigned int uid, unsigned int nid, unsigned int fid)
{
    return api::get_node_field_base(uid,nid,fid);
}

field::FieldBase* plugin::get_node_field_base(unsigned int uid, unsigned int fid)
{
    return api::get_node_field_base(uid,fid);
}

unsigned int plugin::get_field_count(unsigned int uid)
{
    return api::get_field_count(uid);
}

unsigned int plugin::get_in_field_count(unsigned int uid)
{
    return api::get_in_field_count(uid);
}

unsigned int plugin::get_out_field_count(unsigned int uid)
{
    return api::get_out_field_count(uid);
}

status plugin::get_in_fields(unsigned int uid, std::vector<unsigned int>& fids)
{
    return api::get_in_fields(uid,fids);
}

status plugin::get_out_fields(unsigned int uid, std::vector<unsigned int>& fids)
{
    return api::get_out_fields(uid,fids);
}

field::connection::Type plugin::get_field_connection_type(unsigned int uid, unsigned int fid)
{
    return api::get_field_connection_type(uid,fid);
}

status plugin::connect(unsigned int suid, unsigned int sfid, unsigned int tuid, unsigned int tfid)
{
    return api::connect(suid,sfid,tuid,tfid);
}

status plugin::connections(unsigned int uid, unsigned int fid, std::vector<field::Connection> &connections)
{
    return api::connections(uid,fid,connections);
}
