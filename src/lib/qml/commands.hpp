/***********************************************************************
 *
 * Filename: commands.hpp
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

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "deps.hpp"
#include "qml_deps.hpp"
#include "types.hpp"
#include "status.hpp"
#include "command.hpp"
#include "pluginmanager.hpp"
#include "field.hpp"
#include "draw.hpp"

namespace feather
{

    namespace qml
    {

        namespace command
        {

            status init();

            // Node

            unsigned int add_node(const unsigned int node, const std::string name); 
            bool nodes_added(std::vector<unsigned int>& uids);
            void remove_node(const unsigned int uid, status& error);
            void nodes_updated();
            bool node_exists(int uid);
            status connect_nodes(int n1, int f1, int n2, int f2); 
            void get_node_icon(const unsigned int nid, std::string& file, status& error);
            void get_node_by_name(std::string name, unsigned int& uid);
            void get_node_by_type(node::Type type, std::vector<unsigned int>& uids);
            unsigned int get_node_id(const unsigned int uid, status& error);
            status get_node_connected_uids(unsigned int uid, std::vector<unsigned int>& uids);
            status get_node_connected_uids(unsigned int uid, unsigned int fid, std::vector<unsigned int>& uids);
            status get_node_draw_items(int nid, draw::DrawItems& items);
            status load_plugins();
            status run_command(std::string cmd, feather::parameter::ParameterList params);
            status run_command_string(std::string str);

            
            // Layer
            status add_layer(FLayer layer);
            status remove_layer(int lid);
            status move_layer(int sid, int tid);
            status get_layer(int id, FLayer &layer);
            void set_layer_name(std::string name, int lid);
            void set_layer_color(int r, int g, int b, int lid);
            void set_layer_visible(bool v, int lid);
            void set_layer_locked(bool l, int lid);
            int layer_count();
            void add_node_to_layer(int lid, int uid);


            // Fields
 
            // get field base 
            /*!
             * Returns a node's field that holds the value for the fid.
             * NOTE! if the field is connected it will return the field of the parent that's connected to it.
             * If you want to get the base of the node's fid, even if it's connected, use get_node_fieldBase().
             */
            status get_field_base(unsigned int uid, unsigned int fid, feather::field::FieldBase* &f, unsigned int conn=0);
            status get_field_base_array(unsigned int uid, unsigned int fid, std::vector<feather::field::FieldBase*> &f);
            /*!
             * Same as get_fieldBase() except it will return the base of the node field even if it's connected 
             */
            status get_node_field_base(unsigned int uid, unsigned int fid, feather::field::FieldBase* &f);

            status get_field_type(unsigned int uid, unsigned int nid, unsigned int fid, int& val);
 
            // get the field value
            status get_field_val(unsigned int uid, unsigned int nid, unsigned int fid, bool& val, unsigned int conn=0);
            status get_field_val(unsigned int uid, unsigned int nid, unsigned int fid, int& val, unsigned int conn=0);
            status get_field_val(unsigned int uid, unsigned int nid, unsigned int fid, FReal& val, unsigned int conn=0);
            status get_field_val(unsigned int uid, unsigned int nid, unsigned int fid, FMesh& val, unsigned int conn=0);
            // set the field value
            status set_field_val(unsigned int uid, unsigned int nid, unsigned int fid, bool& val);
            status set_field_val(unsigned int uid, unsigned int nid, unsigned int fid, int& val);
            status set_field_val(unsigned int uid, unsigned int nid, unsigned int fid, FReal& val);
            status set_field_val(unsigned int uid, unsigned int nid, unsigned int fid, FRealArray& val);
            status set_field_val(unsigned int uid, unsigned int nid, unsigned int fid, FKeyArray& val);

            status get_field_connection_status(int uid, int field, bool& val);
            status get_field_connection_status(int uid, int node, int field, bool& val);
            status get_field_connection_status(int suid, int sfid, int tuid, int tfid, bool& val);
            status get_connected_fid(unsigned int uid, unsigned int fid, unsigned int& suid, unsigned int& sfid, unsigned int conn=0); // uid and fid are from the node's input
            status get_fid_list(int uid, int nid, field::connection::Type conn, std::vector<field::FieldBase*>& list);
            int get_field_count(int uid);
            int get_in_field_count(int uid);
            int get_out_field_count(int uid);
            field::connection::Type get_field_connection_type(int uid, int fid);

            // SG EDITOR
            void clear();
            void get_node_out_connections(const unsigned int uid, std::vector<unsigned int>& uids);
            int get_node_connection_count(int uid);
            void get_node_name(const unsigned int uid, std::string& name, status& error);
            void scenegraph_update();
            int get_min_uid();
            int get_max_uid();
            void get_plugins(std::vector<PluginInfo>& list);

            // SG SELECTION
            int selected_node();
            status select_node(int uid);

            /* 
            status select_node(int type, int uid);
            status select_node(int type, int uid, int nid);
            status select_node(int type, int uid, int nid, int fid);
            */

            status get_selected_nodes(std::vector<unsigned int>& uids);
            void clear_selection();


            // RENDER

            void render_buffer(int id);

  
        } // namespace command

    } // namespace qml

} // namespace feather

#endif
