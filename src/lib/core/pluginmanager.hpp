/***********************************************************************
 *
 * Filename: pluginmanager.hpp
 *
 * Description: Loads plugins and manages interaction with the scenegraph.
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

#ifndef PLUGINMANAGER_HPP
#define PLUGINMANAGER_HPP

#include "deps.hpp"
#include "types.hpp"
#include "command.hpp"
#include "parameter.hpp"
#include "field.hpp"
#include "draw.hpp"

#define MAX_NODE_ID 800

namespace feather
{

    struct PluginInfo {
        std::string name;
        std::string description;
        std::string author;
    };

    struct PluginData {
        std::string path;
        void *handle;
        std::string (*name)();
        std::string (*description)();
        std::string (*author)();
        status (*fields_init)(int,field::Fields&);
        status (*update_properties)(int,field::Fields&);
        status (*do_it)(int,field::Fields&);
        //status (*draw_it)(int,draw::DrawItems&);
        bool (*node_exist)(int); // is there a node with the given type and id in this plugin
        bool (*node_drawable)(int); // can the node be drawn in the viewport
        status (*node_draw_items)(int,draw::DrawItems&); // can the node be drawn in the viewport
        status (*node_type)(int,node::Type&);
        bool (*node_icon)(int,std::string&); // name of icon image in ui/icons path
        status(*create_fields)(int,field::Fields&); // creates a new instance of the nodes fields which will get deleted by the scenegraph when the node is removed.
        field::FieldBase* (*get_field)(int,int,field::Fields&);
        status (*get_fid_list)(int,field::connection::Type,field::Fields&,std::vector<field::FieldBase*>&);
        bool (*command_exist)(std::string cmd);
        status (*command)(std::string cmd, parameter::ParameterList);
        status (*parameter_name)(std::string, int, std::string&);
        status (*parameter_type)(std::string, int, parameter::Type&);
    };

    struct get_name {
        get_name(std::vector<PluginInfo>& list) : m_list(list) {};
        void operator()(PluginData n) { 
            PluginInfo info;
            info.name = n.name();
            info.description = n.description();
            info.author = n.author();
            m_list.push_back(info);
        };

        private:
            std::vector<PluginInfo>& m_list; 
    };

    struct get_fids {
        get_fids(int nid, field::connection::Type conn, field::Fields& fields, std::vector<field::FieldBase*>& list) : m_nid(nid),m_conn(conn),m_fields(fields),m_list(list) {};
        void operator()(PluginData n) {
            if(n.node_exist(m_nid))
                n.get_fid_list(m_nid,m_conn,m_fields,m_list); 
            else
                std::cout << "didn't find " << m_nid << std::endl;
        };

        private:
            int m_nid;
            field::connection::Type m_conn;
            field::Fields& m_fields;
            std::vector<field::FieldBase*>& m_list; 
    };


    // FIELD_INIT()

    template <int _Id>
    struct call_fields_inits{
        static status exec(int id, field::Fields& fields) { return call_fields_inits<_Id-1>::exec(id,fields); };
    };

    template <> struct call_fields_inits<0> { static status exec(int id, field::Fields& fields) { return status(FAILED,"could not find node"); }; };

    template <int _Id> status node_fields_init(field::Fields& fields) { return status(FAILED,"no node found"); };
   
    struct call_fields_init {
        call_fields_init(int node,field::Fields& fields): m_node(node), m_fields(fields){};
        void operator()(PluginData n) { if(n.node_exist(m_node)) { n.fields_init(m_node,m_fields); } };
        private:
            int m_node;
            field::Fields& m_fields;
    };


    // UPDATE_PROPERTIES()

    template <int _Id>
    struct call_update_properties {
        static status exec(int id, field::Fields& fields) { return call_update_properties<_Id-1>::exec(id,fields); };
    };

    template <> struct call_update_properties<0> { static status exec(int id, field::Fields& fields) { return status(FAILED,"could not find node"); }; };

    template <int _Id> status node_update_properties(field::Fields& fields) { return status(FAILED,"no node found"); };
   
    struct call_update_property {
        call_update_property(int node,field::Fields& fields): m_node(node), m_fields(fields){};
        void operator()(PluginData n) { if(n.node_exist(m_node)) { n.update_properties(m_node,m_fields); } };
        private:
            int m_node;
            field::Fields& m_fields;
    };


    // DO_IT()

    template <int _Id>
    struct call_do_its {
        static status exec(int id, field::Fields& fields) { return call_do_its<_Id-1>::exec(id,fields); };
    };

    template <> struct call_do_its<0> { static status exec(int id, field::Fields& fields) { return status(FAILED,"could not find node"); }; };

    template <int _Id> status node_do_it(field::Fields& fields) { return status(FAILED,"no node found"); };
   
    struct call_do_it {
        call_do_it(int node,field::Fields& fields): m_node(node), m_fields(fields){};
        void operator()(PluginData n) { if(n.node_exist(m_node)) { n.do_it(m_node,m_fields); } };
        private:
            int m_node;
            field::Fields& m_fields;
    };


    // DRAW_IT()
    /*
    template <int _Id>
    struct call_draw_its {
        static status exec(int id, draw::DrawItems& items) { return call_draw_its<_Id-1>::exec(id,items); };
    };

    template <> struct call_draw_its<0> { static status exec(int id, draw::DrawItems& items) { return status(FAILED,"could not find node"); }; };

    template <int _Id> status node_draw_it(draw::DrawItems& items) { return status(FAILED,"no node found"); };
   
    struct call_draw_it {
        call_draw_it(int node,draw::DrawItems& items): m_node(node), m_items(items){};
        void operator()(PluginData n) { if(n.node_exist(m_node)) { n.draw_it(m_node,m_items); } };
        private:
            int m_node;
            draw::DrawItems& m_items;
    };
    */


    // NODE MATCHING

    template <int _Id>
    struct find_nodes {
        static bool exec(int id) { return find_nodes<_Id-1>::exec(id); };
    };

    template <> struct find_nodes<0> { static bool exec(int id) { return false; }; };


    // NODE DRAWABLE 

    template <int _Id>
    struct find_node_drawable {
        static bool exec(int id) { return find_node_drawable<_Id-1>::exec(id); };
    };

    template <> struct find_node_drawable<0> { static bool exec(int id) { return false; }; };


    // CALL NODE DRAW ITEM

    template <int _Id>
    struct call_draw_items {
        static status exec(int nid, draw::DrawItems& items) { return call_draw_items<_Id-1>::exec(nid,items); };
    };

    template <> struct call_draw_items<0> { static status exec(int nid, draw::DrawItems& items) { return status(FAILED,"found no draw item for node"); }; };
 
    template <int _Id> status node_draw_it(draw::DrawItems& items) { return status(FAILED,"no node found"); };
 
    struct call_draw_item {
        call_draw_item(int nid, draw::DrawItems& items): m_nid(nid), m_items(items){};
        void operator()(PluginData n) { if(n.node_exist(m_nid)) { n.node_draw_items(m_nid,m_items); } };

        private:
            int m_nid;
            draw::DrawItems& m_items;
    };


    // NODE TYPE

    template <int _Id>
    struct find_node_type {
        static status exec(int id, node::Type& type) { return find_node_type<_Id-1>::exec(id,type); };
    };

    template <> struct find_node_type<0> { static status exec(int id, node::Type& type) { return status(FAILED,"failed to find any matching nodes"); }; };


    // NODE ICON 

    template <int _Id>
    struct find_node_icon {
        static bool exec(int id,std::string& file) { return find_node_icon<_Id-1>::exec(id,file); };
    };

    template <> struct find_node_icon<0> { static bool exec(int id,std::string& file) { return false; }; };



    // GET FIELD DATA

    template <int _NodeId, int _FieldId>
    field::FieldBase* field_data(field::Fields& fields) { return NULL; };  

    // remember that the StartFieldId has to be the last field id + 1
    template <int _NodeId, int _StartFieldId>
    struct find_field {
        static field::FieldBase* exec(int fid, field::Fields& fields) {
            return find_field<_NodeId,_StartFieldId-1>::exec(fid,fields);
        };
    };

    template <int _NodeId>
    struct find_field<_NodeId,0> {
        static field::FieldBase* exec(int fid, field::Fields& fields) {
            return NULL;
        };
    };

    template <int _EndNode, int _StartNode, int _StartField>
        struct find_node_field {
            static field::FieldBase* exec(int nid, int fid, field::Fields& fields) {
                if(nid==_StartNode) {
                    return find_field<_StartNode,_StartField>::exec(fid,fields);
                }
                else
                    return find_node_field<_EndNode,_StartNode-1,_StartField>::exec(nid,fid,fields);
            };
        };

    template <int _StartNode, int _StartField>
        struct find_node_field<_StartNode,_StartNode,_StartField> {
            static field::FieldBase* exec(int nid, int fid, field::Fields& fields) {
                if(nid==_StartNode)
                    return find_field<_StartNode,_StartField>::exec(fid,fields);
                else 
                    return NULL;
            }; 
        };


    // GET NODE'S FIDs
    // later connection direction needs to be added
    template <int _Nid>
    struct get_fid_list {
        static bool exec(int nid, feather::field::connection::Type conn, field::Fields& fields, std::vector<field::FieldBase*>& list) {
            if(nid==_Nid) {
                for(uint i=0; i < fields.size(); i++) {
                    field::FieldBase* f = fields.at(fields.size()-(i+1));

                    if(!f) {
                        std::cout << "FID NULL\n";
                    }
                    else {
                        if(f->conn_type==conn)
                            list.push_back(f);
                    }
                }
                return true;
            }
            return false;
        };
    };

    template <>
    struct get_fid_list<0> {
        static bool exec(int nid, field::connection::Type conn, field::Fields& fields, std::vector<field::FieldBase*>& list) {
            return false;
        };
    };

    template <int _EndNid, int _StartNid>
    struct find_node_fid_list {
        static bool exec(int nid, field::connection::Type conn, field::Fields& fields, std::vector<field::FieldBase*>& list) {
            if(nid==_StartNid) {
                return get_fid_list<_StartNid>::exec(nid,conn,fields,list);
            }
            else
                return find_node_fid_list<_EndNid,_StartNid-1>::exec(nid,conn,fields,list);
        };
    };

    template <int _StartNid>
    struct find_node_fid_list<_StartNid,_StartNid> {
        static bool exec(int nid, field::connection::Type conn, field::Fields& fields, std::vector<field::FieldBase*>& list) {
            if(nid==_StartNid)
                return get_fid_list<_StartNid>::exec(nid,conn,fields,list);
            else
                return false;
        };
    };

    // CREATE FIELDS

    // Add Field is used to setup the Fields vector 
    template <int _Node, int _StartKey>
    struct add_fields {
        static status exec(field::Fields& fields) {
            return add_fields<_Node,_StartKey-1>::exec(fields);
        };
    };

    template <int _Node> struct add_fields<_Node,0> { static status exec(field::Fields& fields) { return status(); }; };

    // Create Fields is used to get the field into the sg node container

    template <int _Id>
    struct find_create_fields {
        static  status exec(int id, field::Fields& fields) { return find_create_fields<_Id-1>::exec(id,fields); };
    };

    template <> struct find_create_fields<0> { static status exec(int id, field::Fields& fields) { return status(FAILED,"No matching node found to create fields in"); }; };


    // NODE ICON IMAGE 

    template <int _Id>
    struct call_node_icons {
        static void exec(int nid, std::string& file) { call_node_icons<_Id-1>::exec(nid,file); };
    };

    template <> struct call_node_icons<0> { static void exec(int nid, std::string& file) {}; };
 
    template <int _Id> void node_icon(int nid, std::string& file) {};

    struct call_node_icon {
        call_node_icon(int nid, std::string& file) : m_nid(nid),m_file(file){};
        void operator()(PluginData n) { if(n.node_exist(m_nid)) { n.node_icon(m_nid,m_file); } };

        private:
            int m_nid;
            std::string& m_file;
    };


    // NODE TYPE 

    template <int _Id>
    struct call_node_types {
        static void exec(int nid, feather::node::Type& type) { call_node_types<_Id-1>::exec(nid,type); };
    };

    template <> struct call_node_types<0> { static void exec(int nid, feather::node::Type& type) {}; };
 
    template <int _Id> void node_type(int nid, feather::node::Type& type) {};

    struct call_node_type {
        call_node_type(int nid, feather::node::Type& type) : m_nid(nid),m_type(type){};
        void operator()(PluginData n) { if(n.node_exist(m_nid)) { n.node_type(m_nid,m_type); } };

        private:
            int m_nid;
            feather::node::Type& m_type;
    };


    // PLUGIN MANAGER

    class PluginManager
    {
        public:
            PluginManager();
            ~PluginManager();
            status load_plugins();
            status fields_init(int node,field::Fields& fields); // this is called by the scenegraph
            status update_properties(int node,field::Fields& fields); // this is called by the scenegraph
            status do_it(int node,field::Fields& fields); // this is called by the scenegraph
            //status draw_it(int node,draw::DrawItems& items); // this is called by the scenegraph
            status create_fields(int node, field::Fields& fields); // this will return a new instance of the node's fields 
            void get_draw_items(const int nid, draw::DrawItems& items);
            field::FieldBase* get_fieldBase(int uid, int node, int field, field::Fields& fields);
            status run_command(std::string cmd, parameter::ParameterList);
            status run_command_string(std::string str);
            int min_uid();
            int max_uid();
            status node_icon_file(int nid, std::string& file);
            status node_type(int nid, feather::node::Type& type);
            void loaded_plugins(std::vector<PluginInfo>& list);
            status get_fid_list(int nid, field::connection::Type conn, field::Fields& fields, std::vector<field::FieldBase*>& list);

        private:
            bool add_parameter_to_list(std::string cmd, int key, std::string val, parameter::ParameterList& list);
            status load_node(PluginData &node);
            status load_command(PluginData &command);
            std::string m_pluginPath;
            std::vector<PluginData> m_plugins;
    };

} // namespace feather

#define C_PLUGIN_WRAPPER()\
    std::string name();\
    std::string description();\
    std::string author();\
    feather::status fields_init(int, feather::field::Fields&);\
    feather::status update_properties(int, feather::field::Fields&);\
    feather::status do_it(int, feather::field::Fields&);\
    /*feather::status draw_it(int, feather::draw::DrawItems&);*/\
    bool node_exist(int);\
    bool node_drawable(int);\
    feather::status node_draw_items(int,feather::draw::DrawItems&);\
    feather::status node_type(int,feather::node::Type&);\
    bool node_icon(int,std::string&);\
    feather::status create_fields(int, feather::field::Fields&);\
    feather::field::FieldBase* get_field(int,int,feather::field::Fields&);\
    feather::status get_fid_list(int,feather::field::connection::Type,feather::field::Fields&,std::vector<feather::field::FieldBase*>&);\
    bool command_exist(std::string cmd);\
    feather::status command(std::string cmd, feather::parameter::ParameterList);\
    feather::status parameter_name(std::string cmd, int key, std::string& name);\
    feather::status parameter_type(std::string cmd, int key, feather::parameter::Type& type);\


#define PLUGIN_INIT(__name,__description,__author,startnode,endnode)\
    /* plugin name */\
    std::string name() { return __name; };\
    /* plugin description */\
    std::string description() { return __description; };\
    /* plugin name */\
    std::string author() { return __author; };\
    \
    /* call node update_properties() */\
    feather::status fields_init(int id, feather::field::Fields& fields) {\
        return call_fields_inits<MAX_NODE_ID>::exec(id,fields);\
    };\
    \
    \
    /* call node update_properties() */\
    feather::status update_properties(int id, feather::field::Fields& fields) {\
        return call_update_properties<MAX_NODE_ID>::exec(id,fields);\
    };\
    \
    /* call node do_it() */\
    feather::status do_it(int id, feather::field::Fields& fields) {\
        return call_do_its<MAX_NODE_ID>::exec(id,fields);\
    };\
    \
    /* call node draw_it() */\
    /*feather::status draw_it(int id, feather::draw::DrawItems& items) {*/\
    /*    return call_draw_its<MAX_NODE_ID>::exec(id,items);*/\
    /*};*/\
   \
    /* see if the node is in the plugin */\
    bool node_exist(int id) {\
        return find_nodes<MAX_NODE_ID>::exec(id);\
    };\
    \
    /* see if the node can be drawn */\
    bool node_drawable(int id) {\
        return find_node_drawable<MAX_NODE_ID>::exec(id);\
    };\
    \
    \
    /* get the draw items of a node */\
    status node_draw_items(int id, feather::draw::DrawItems& items) {\
        return call_draw_items<MAX_NODE_ID>::exec(id,items);\
    };\
    \
    /* get the node type */\
    feather::status node_type(int id, feather::node::Type& type) {\
        return find_node_type<MAX_NODE_ID>::exec(id,type);\
    };\
    /* get the node icon */\
    bool node_icon(int id, std::string& file) {\
        return find_node_icon<MAX_NODE_ID>::exec(id,file);\
    };\
    \
    /* create a node field */\
    feather::status create_fields(int id,feather::field::Fields& fields) {\
        return find_create_fields<MAX_NODE_ID>::exec(id,fields);\
    };\
    \
    /* find the node's field */\
    feather::field::FieldBase* get_field(int nid, int fid, field::Fields& fields) {\
        return find_node_field<startnode,endnode,50>::exec(nid,fid,fields);\
    };\
    /* find the node's fid's*/\
    feather::status get_fid_list(int nid, feather::field::connection::Type conn, feather::field::Fields& fields, std::vector<feather::field::FieldBase*>& list) {\
        find_node_fid_list<startnode,endnode>::exec(nid,conn,fields,list);\
        return status();\
    };\
    \
    /* find if command exist */\
    bool command_exist(std::string cmd) { return feather::command::exist<800>::exec(cmd); };\
    \
    /* call the command */\
    feather::status command(std::string cmd, parameter::ParameterList params) {\
        return feather::command::run<800>::exec(cmd,params);\
    };\
    \
    /* get the parameter name */\
    feather::status parameter_name(std::string c, int k, std::string& n) {\
    return feather::command::get_parameter_name<20,800>::exec(c,k,n);\
    };\

#endif
