/***********************************************************************
 *
 * Filename: scenegraph.hpp
 *
 * Description: Holds the nodes and handles how they interact with the ui.
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

#ifndef SCENEGRAPH_HPP
#define SCENEGRAPH_HPP

#include "deps.hpp"
#include "types.hpp"
#include "node.hpp"
#include "field.hpp"
#include "singleton.hpp"
#include "selection.hpp"
#include "data.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "pluginmanager.hpp"
#include "state.hpp"

namespace feather
{

    /*
     * The SceneGraph holds the node tree. There is no node data stored in the
     * SceneGraph; only the node type and id. The id is mapped to the data so 
     * that the scenegraph can lookup it's values and then view or modify them
     * depending on the command. The scenegraph will manage when and in what order
     * the nodes will get called in. The scenegraph will also call the functions
     * for various stages of the update.
     *
     * When working with nodes 3 things are used - the scenegraph, data, and selection.
     * The scenegraph manages the order the nodes are updated.
     * The data holds each nodes value.
     * The selection tells the components and indics of what is to be worked on.
     * 
     * The scenegraph will call the node's doit() which will take the node's data
     * and work on it based on the selection.
     * A selection is not tied to any node. When a user clicks on a node's component,
     * a new SelectionState is added to the SelectionManager and is setup based on that node's
     * data. As the more vertex's are selected, the SelectionState is modified to match it.
     * This SelectionState is also used to draw the selections during the viewport's update.
     *
     * When a new node is added to the scenegraph a scenegraph vertex and datablock is created.
     * The datablock location is kept in sync with the vertex number by the datamanager.
     */

    static state::FState cstate;
    static std::vector<FLayer> layers;
    static FTime time;
    static FSceneGraph sg;
    static PluginManager plugins;
    static std::vector<FNodeDescriptor> node_selection;
    static std::vector<unsigned int> call_stack; // this is used during sg updates and represents the order of node updates

    namespace scenegraph
    {

        // UPDATE QUEUE FUNCTIONS

        void clear_update_queue()
        {
            cstate.update_queue.clear();
        } 

        bool field_updated(unsigned int uid, unsigned int fid)
        {
            for(auto queueinfo : cstate.update_queue){
                if(queueinfo.uid == uid && queueinfo.fid == fid)
                    return true;
            }
            return false; 
        }

        unsigned int update_queue_count()
        {
            return cstate.update_queue.size();
        }

        void add_node_to_update_queue(unsigned int uid, unsigned int fid)
        {
            cstate.update_queue.push_back(state::UpdateQueueInfo(uid,fid));
        }

        /* clear the scenegraph */
        void clear() {
            // clear the selection
            smg::Instance()->clear();

            int v = boost::num_vertices(sg);
            std::cout << "v count " << v << std::endl;
            v-=1;

            while(v>0){
                std::cout  << "clearing v " << v << std::endl;
                // clear the edges
                boost::clear_vertex(v,sg);
                // clear the vertexes
                boost::remove_vertex(v,sg);
                v-=1;
            }

            // this is currently needed to update sg
            smg::Instance()->add_state(static_cast<selection::Type>(sg[0].type),0,sg[0].node);
        };

        int get_min_uid() { return plugins.min_uid(); };
        //int get_max_uid() { return plugins.max_uid(); };
        int get_max_uid() { return num_vertices(sg)-1; };

        status load_plugins() {
            return plugins.load_plugins();
        };


        
        // NODES

        bool node_exist(unsigned int uid) {
            unsigned int count = num_vertices(sg);
            for(unsigned int i=0; i < count; i++) {
                if(i==uid)
                    return true;
            }
            return false;
        }

        void get_nodes(std::vector<unsigned int> &uids);
 
        bool node_name_exist(std::string name) {
            std::vector<uint32_t> uids;
            get_nodes(uids);
            for(auto uid: uids) {
                if(sg[uid].name == name)
                    return true;
            }
            return false;
        }
 
        /* Add Node
         * This function is called during specialization of nodes when
         * a new node is added to the scenegraph. It's called by add_node_to_sg
         * and is specialized by each node.
         */
        unsigned int add_node(const unsigned int n, const std::string name, status& error) {
            //std::cout << "add node: " << n << ", type: " << t << std::endl;
            feather::node::Type ntype;
            plugins.node_type(n,ntype);
            std::cout << "add node: " << n << " type: " << ntype << std::endl;

            // TODO
            // Here I need to ask the plugin manager if the node exists
 
            FNodeDescriptor uid = boost::add_vertex(sg);
            //sg[uid].type = static_cast<feather::node::Type>(t);
            sg[uid].type = ntype;
            sg[uid].uid = uid;
            sg[uid].node = n;
            if(node_name_exist(name)) {
                bool dup=true;
                uint32_t count=1;
                while(dup) {
                    std::stringstream newname;
                    newname << name << "_" << count;
                    if(node_name_exist(newname.str())) {
                        count++;
                    } else {
                        sg[uid].name = newname.str();
                        dup=false;
                    }
                }
            } else {
                sg[uid].name = name;
            }
            sg[uid].layer = 0;
            plugins.fields_init(n,sg[uid].fields); // this creates the base properties that all nodes have
            plugins.create_fields(n,sg[uid].fields);
            // do the selection in a seperate command
            //node_selection.push_back(n); 

            // if the new node's uid is now highest uid, replace the state's maxUid
            if(static_cast<int>(uid) > cstate.sgState.maxUid)
                cstate.sgState.maxUid = uid;
            
            // several nodes can be added at a time so we'll add this node
            // to the list of nodes that have already been added but haven't
            // been updated.
            // An example of this is the viewport editor which needs to create

            // TODO - glinfo is not used anymore
            // glinfo for each node. If a command adds several nodes, the
            // viewport wll use these uids in the state to generate the glinfo
            cstate.add_uid_to_update(uid);

            // Return the node number
            return static_cast<int>(uid);
        };

        /* Remove node from scenegraph */
        void remove_node(const unsigned int uid, status& error) {
             // this is currently needed to update sg
            smg::Instance()->clear();
            smg::Instance()->add_state(static_cast<selection::Type>(sg[0].type),0,sg[0].node);
            boost::clear_vertex(uid,sg);
            boost::remove_vertex(uid,sg);
        };

        /* This gets called when all the nodes have been updated.
         * This currently being used by the viewports to update their
         * glinfo
         */
        void nodes_updated() {
            cstate.clear_uid_update();
        };

        /*  Get Node Connections 
     *  will add all the nodes connected to the
     *  uid to the nodes reference
     */
    void get_node_out_connections(const unsigned int uid, std::vector<unsigned int>& uids) {
        typedef boost::graph_traits<FSceneGraph>::out_edge_iterator OutConn;
        std::pair<OutConn,OutConn> out = boost::out_edges(uid,sg);

        // this is a simple hack to check if a edge exist, I'm sure there's a better way to do this
        if(out.first==out.second){
            std::cout << "first and second edges match for " << uid << std::endl; 
            return;
        } else {
            std::cout << "edges don't match for " << uid << std::endl;
            boost::graph_traits<FSceneGraph>::edge_descriptor c = *out.first;
            uids.push_back(sg[boost::target(c,sg)].uid);
        }
    };

    /*
     * Return a vector containing all the uids in the scenegraph
     */
    void get_nodes(std::vector<unsigned int> &uids) {
        int count = num_vertices(sg);
        for(int i=0; i < count; i++)
            uids.push_back(i);
    }

    void get_node_by_name(std::string name, unsigned int& uid) {
        int count = num_vertices(sg);
        for(int i=0; i < count; i++){
            if(sg[i].name == name)
                uid = i;
        }
    }

    unsigned int get_node_type(unsigned int uid) {
        return sg[uid].type;
    };

    // this will return all nodes base on type (polygons, animation, etc.)
    void get_node_by_type(node::Type type, std::vector<unsigned int>& uids) {
        unsigned int count = num_vertices(sg);
        for(int i=0; i < count; i++){
            if(sg[i].type == type)
                uids.push_back(i);
        }
    }

    // this will return all nodes base on it's id (polycube, polysphere, etc.)
    void get_node_by_id(unsigned int id, std::vector<unsigned int>& uids) {
        unsigned int count = num_vertices(sg);
        for(int i=0; i < count; i++){
            if(sg[i].node == id)
                uids.push_back(i);
        }
    }

    void get_node_name(const unsigned int uid, std::string& name, status& error) {
        // TODO verify that uid exist and set the error if it doesn't
        name = sg[uid].name;
    };


    void get_node_icon(const unsigned int nid, std::string& file, status& error) {
        error = plugins.node_icon_file(nid,file);
    };


    unsigned int get_node_id(const unsigned int uid, status& error) {
        return sg[uid].node;
    };


    /* This will return all the node uids connected to node */
    status get_node_connected_uids(int uid, std::vector<unsigned int>& uids) {
        typedef boost::graph_traits<FSceneGraph>::out_edge_iterator ei;
        std::pair<ei,ei> p = boost::out_edges(uid,sg);

        //std::cout << "TEST edge iter " << *p.first << std::endl;

        for(;p.first!=p.second;++p.first){
            std::cout << "edge iter " << *p.first << std::endl;
            uids.push_back(target(*p.first,sg));
        }

        return status();
    };


    /* This will return all the node uids connected to node's fid
     * IMPORTANT NOTE !!!!
     * This only works for OUT fid's; IN fids will return nothing.
     * Instead use get_fieldBase_array() to get the all the uids
     * connected to a input field.
     */
    status get_node_connected_uids(unsigned int uid, unsigned int fid, std::vector<unsigned int>& uids) {
        typedef boost::graph_traits<FSceneGraph>::out_edge_iterator ei;

        std::pair<ei,ei> p = boost::out_edges(uid,sg);

        std::cout << "get_node_connected_uids(" << uid << "," << fid << ")\n";
        for(;p.first!=p.second;++p.first){
            std::cout << "source=" << source(*p.first, sg) << std::endl;
            std::cout << "target=" << target(*p.first, sg) << std::endl;
            if(sg[*p.first].f1 == fid)
                uids.push_back(target(*p.first,sg));
        }

        return status();
    }; 

    /* return a description of how the node is to be draw, if at all */
    status get_node_draw_items(int nid, draw::DrawItems& items) {
        plugins.get_draw_items(nid,items);
        // TODO add a fail status if there is no draw call or failed to get the draw items
        return status();
    };


    // FIELDS


    /*!
     * Returns a node's field that holds the value for the fid.
     * NOTE! if the field is connected it will return the field of the parent that's connected to it.
     * If you want to get the base of the node's fid, even if it's connected, use get_node_fieldBase().
     */
    field::FieldBase* get_fieldBase(unsigned int uid, unsigned int nid, unsigned int fid, unsigned int conn=0 ) {
        std::cout << "uid:" << uid << ", nid:" << nid << ", fid:" << fid << " has " << sg[uid].fields.size() << " fields\n";
        field::FieldBase* f = nullptr;
        for(auto field : sg[uid].fields){
            if(field->id == fid)
                f = field;
        }
        //field::FieldBase* f = plugins.get_fieldBase(uid,nid,fid,sg[uid].fields);
        if(!f)
            std::cout << "uid " << uid << " nid " << nid << " fid " << fid << " is null\n";
        std::cout << "CALLING get_fieldBase - uid:" << uid << " nid:" << nid << " fid:" << fid << " connected:" << f->connected() << std::endl;
        if(!f || f->connected()) {
            if(f->connected()){
                field::Connection connection = f->connections.at(conn);
                std::cout << "field is connected to uid:" << connection.puid << ", node type:" << connection.pnid << ", field " << connection.pfid << std::endl;
                f = get_fieldBase(connection.puid,connection.pnid,connection.pfid);
            } 
        }
        return f;
    };

    // like the above function except it returns all the field bases attached to it
    std::vector<field::FieldBase*> get_fieldBase_array(unsigned int uid, unsigned int nid, unsigned int fid, unsigned int conn=0 ) {
        // Replaced
        //field::FieldBase* f = plugins.get_fieldBase(uid,nid,fid,sg[uid].fields); 
        // With this
        field::FieldBase* f = nullptr;
        for(auto field : sg[uid].fields){
            if(field->id == fid)
                f = field;
        }
        std::cout << "CALLING get_fieldBase - uid:" << uid << " nid:" << nid << " fid:" << fid << " connected:" << f->connected() << std::endl;
        std::vector<field::FieldBase*> fields;
        if(!f || f->connected()) {
            if(f->connected()){
                for ( auto connection : f->connections ) {
                    std::cout << "field is connected to uid:" << connection.puid << ", node type:" << connection.pnid << ", field " << connection.pfid << std::endl;
                    fields.push_back(get_fieldBase(connection.puid,connection.pnid,connection.pfid));
                }
            } 
        }
        return fields;
    };


    /*!
     * Returns a node's field that holds the value for the fid.
     * NOTE! if the field is connected it will return the field of the parent that's connected to it.
     * If you want to get the base of the node's fid, even if it's connected, use get_node_fieldBase().
     */
    field::FieldBase* get_fieldBase(unsigned int uid, unsigned int fid, unsigned int conn=0 ) {
        status e;
        unsigned int nid = get_node_id(uid,e);
        return get_fieldBase(uid,nid,fid,conn);
    };


    /*!
     * Same as get_fieldBase() except it will return the base of the node field even if it's connected 
     */
    field::FieldBase* get_node_fieldBase(unsigned int uid, unsigned int nid, unsigned int fid) {
        // Replaced
        //field::FieldBase* f = plugins.get_fieldBase(uid,nid,fid,sg[uid].fields); 
        // With this
        std::cout << "uid:" << uid << ", nid:" << nid << ", fid:" << fid << " has " << sg[uid].fields.size() << " fields\n";
        field::FieldBase* f = nullptr;
        for(auto field : sg[uid].fields){
            if(field->id == fid)
                f = field;
        }
 
        if(!f) {
            for(auto field : sg[uid].fields){
                //std::cout << "looking for " << fid << " field id=" << field->id << std::endl;
                if(field->id == fid)
                    f = field;
            }
            if(!f)
                return nullptr;
        }
        return f;
    };

    field::FieldBase* get_node_fieldBase(int uid, int fid) {
        status e;
        unsigned int nid = get_node_id(uid,e);
        return get_node_fieldBase(uid,nid,fid);
    };


    int get_field_count(int uid) {
        return sg[uid].fields.size(); 
    }

    int get_in_field_count(int uid) {
        int i=0;
        std::for_each(sg[uid].fields.begin(), sg[uid].fields.end(),[&i](field::FieldBase* f){ if(f->conn_type==field::connection::In){i++;} });
        return i; 
    }

    int get_out_field_count(int uid) {
        int i=0;
        std::for_each(sg[uid].fields.begin(), sg[uid].fields.end(),[&i](field::FieldBase* f){ if(f->conn_type==field::connection::Out){i++;} });
        return i; 
    }

    status get_in_fields(int uid, std::vector<unsigned int> &fids) {
         std::for_each(sg[uid].fields.begin(), sg[uid].fields.end(),[&fids](field::FieldBase* f){ if(f->conn_type==field::connection::In){fids.push_back(f->id);} });
         return status(); // need to test if the uid exist
    }

    status get_out_fields(int uid, std::vector<unsigned int> &fids) {
         std::for_each(sg[uid].fields.begin(), sg[uid].fields.end(),[&fids](field::FieldBase* f){ if(f->conn_type==field::connection::Out){fids.push_back(f->id);} });
         return status(); // need to test if the uid exist
    }

    field::connection::Type get_field_connection_type(int uid, int fid) {
        for(uint i=0; i < sg[uid].fields.size(); i++){
            if(sg[uid].fields.at(i)->id==fid)
                return static_cast<field::connection::Type>(sg[uid].fields.at(i)->conn_type);
        }
        //std::for_each(sg[uid].fields.begin(), sg[uid].fields.end(),[&fid](field::FieldBase* f){ if(f->id==fid){ return f->conn_type; } });
        return static_cast<field::connection::Type>(0); // this should never be returned
        
    }


    // LAYER
    
    int layer_count() { return layers.size(); };

    FLayer* layer(int lid) { return &layers.at(lid); };

    bool layer(int lid, FLayer& l) { l=layers.at(lid); return true; };

    FLayer* node_layer(int uid) { return &layers.at(sg[uid].layer); };

    void set_layer(int uid, int layer) {
        sg[uid].layer = layer;
    };

    bool remove_layer(int lid) {
        if(lid>=(int)layers.size() || lid==0)
            return false;

        layers.erase(layers.begin()+lid);
        return true;
    };

    void move_layer(int sid, int tid) {
        // check
        if(sid < 0 || sid >= (int)layers.size() || tid < 0 || tid >= (int)layers.size() || sid == tid)
            return;

        // move
        if(sid < tid){
            layers.insert(layers.begin()+(tid+1),std::move(layers.at(sid)));
            layers.erase(layers.begin()+sid);
        } else {
            layers.insert(layers.begin()+(sid+1),std::move(layers.at(tid)));
            layers.erase(layers.begin()+tid);

        }
    }

    void add_layer(FLayer layer) { layers.push_back(layer); };

    void add_layer(std::string _name, FColorRGB _color=FColorRGB(1,1,1), bool _visible=true, bool _locked=false) {
        layers.push_back(FLayer(_name,_color,_visible,_locked));
    };

    FLayer get_layer(int uid) {
        return layers.at(sg[uid].layer);
    };

    void add_node_to_layer(int uid, int lid) {
        sg[uid].layer=lid;
    }

    // SELECTION

    /*
    int get_selected_node() {
        if(smg::Instance()->count()>0)
            return smg::Instance()->get_selected_uids(smg::Instance()->count()-1);
        else 
            return -1; // nothing selected
    };
    */

    // add selections

    status add_selection(unsigned int uid) {
        smg::Instance()->add_state(selection::Object,uid,sg[uid].node);
        return status();
    };

    status add_selection(unsigned int uid, unsigned int fid, unsigned int type, std::vector<unsigned int> ids) {
        // status was returned here because we'll probably use it later
        smg::Instance()->add_state(static_cast<selection::Type>(type),uid,sg[uid].node,fid,ids);
        return status();
    };

    status add_selection(unsigned int type, unsigned int uid, unsigned int nid, unsigned int fid) {
        // status was returned here because we'll probably use it later
        smg::Instance()->add_state(static_cast<selection::Type>(type),uid,nid,fid);
        return status();
    };

    // remove selections

    void clear_selection() {
        smg::Instance()->clear();
    };

    void remove_selection(unsigned int uid) {
        smg::Instance()->remove_selection(uid);
    };

    // get selection data

    status get_selected_nodes(std::vector<unsigned int>& uids) {
        //for(uint i=0; i < smg::Instance()->count(); i++)
        //    uids.push_back(smg::Instance()->get_uid(i));
        smg::Instance()->get_selected_uids(uids);
        return status();
    };

    bool node_selected(unsigned int uid, unsigned int fid=0) {
        return smg::Instance()->selected(uid,fid);
    };

    selection::SelectionState* get_selection_state(unsigned int uid, unsigned int fid=0) {
        return smg::Instance()->get_selection_state(uid,fid); 
    };
 
    status get_fid_list(int uid, int nid, field::connection::Type conn, std::vector<field::FieldBase*>& list) {
        return plugins.get_fid_list(nid,conn,sg[uid].fields,list);
    }

} // namespace scenegraph

class node_visitor : public boost::default_bfs_visitor
{
    public:
        //typedef boost::on_finish_vertex event_filter;

        /*
         *  The order of update for a graph like this:
         *      A
         *     / \
         *  ->B-->C
         *  |  \ /
         *  ----D
         *
         *  would be this: 
         *      init vertex A // do any setup that is needed for the node
         *      init vertex B
         *      init vertex C
         *      init vertex D
         *      discover vertex A // discover will call the parent and it's children nodes
         *      examine edge A->B // send the field value to the target node's fields - this will probably not be needed since the target will just hold a pointer to the source field  
         *      tree edge A->B
         *      discover vertex B
         *      examine edge A->C
         *      tree edge A->C
         *      discover vertex C
         *      finish vertex A // call the nodes do_it() 
         *      examine edge A->B
         *      examine edge B->C
         *      examine edge B->D
         *      tree edge B->D
         *      discover vertex D
         *      examine edge D->B
         *      finish vertex B
         *      examine edge A->C
         *      examine edge B->C
         *      examine edge C->D
         *      finish vertex C
         *      examine edge B->D
         *      examine edge C->D
         *      examine edge D->B
         *      finish vertex D
         */

        /*
         * The way node fields are handled are like this:
         *  ________                ________
         * | NODE A |  bgl edge    | NODE B |
         * |        |------------->|        |
         * |________|              |________|
         *
         * When a node is adjusted it will call Boost::BGL and tell
         * it that it has changed and to start the update at that node.
         * BGL will first call all the node::init() functions for
         * every node connected to the start node.
         * Then the graph walking begins.
         *      1: the node::discover() gets called for parent node followed by:
         *              node::examine() [parent to child]
         *              node::tree() [parent to child]
         *              node::discover() [child node]
         *              {THE ABOVE THREE STEPS ARE REPEATED FOR EACH CHILD}
         *      2: the node::do_it() for the parent is called since everything is safe for it to update
         *      3: move on to the next child node and repeat steps 1 and 2
         *
         * The opengl update is done the same except it always starts at
         * the root node and node::finish() is where the specialized draw()
         * function will be found.
         *
         * To handle these different states the scenegraph holds a State value
         * that will tell wheither the graph is in UPDATE, DRAW, or RENDER.
         *
         */

        /*
         * Update Process:
         */

        // VERTEX


        // Init Vertex
        /*
         * This gets called by all vertex at the beginning of an update.
         */
        template < typename Vertex, typename Graph >
            void initialize_vertex(Vertex u, const Graph & g) const
            {
                std::cout << "init nid " << sg[u].node << std::endl;
            }

        // Start Vertex
        /*
         * This should be called by the source vertex at the start but I haven't seen it yet.
         */
        template < typename Vertex, typename Graph >
            void start_vertex(Vertex u, const Graph & g) const
            {
                std::cout << "start nid " << sg[u].node << std::endl;
            }

        // Discover Vertex
        /*
         * This gets called each time a vertex is encountered
         */
        template < typename Vertex, typename Graph >
            void discover_vertex(Vertex u, const Graph & g) const
            {
                std::cout << "discover vertex(uid):" << u << " nid:" << sg[u].node << std::endl;
            }

        // Finish Vertex
        /*
         * This gets called each time a vertex's child vertex are finished
         */
        template < typename Vertex, typename Graph >
            void finish_vertex(Vertex u, const Graph & g) const
            {
                std::cout << "finish vertex(uid) " << sg[u].node << std::endl;
            }

        // EDGES

        // Examine Edge
        /*
         * This gets called after a vertex has been discovered and after a node has finished
         */
        template < typename Edge, typename Graph >
            void examine_edge(Edge u, const Graph & g) const
            {
                std::cout << "examine edge - uid1:" << sg[u].n1 
                    << " fid1:" << sg[u].f1
                    << " uid2:" << sg[u].n2
                    << " fid2:" << sg[u].f2
                    << std::endl;

                // remove all instances of the uid from the stack
                // and then push into it
                call_stack.erase(std::remove(call_stack.begin(),call_stack.end(),sg[u].n2),call_stack.end());
                call_stack.push_back(sg[u].n2);
            }



        // Tree Edge
        /*
         * This gets called each after the examine edge and only for edges that will make a tree
         * This would be used to draw the Outliner graph
         */
        template < typename Edge, typename Graph >
            void tree_edge(Edge u, const Graph & g) const
            {
                std::cout << "tree edge - uid1:" << sg[u].n1 
                    << " fid1:" << sg[u].f1
                    << " uid2:" << sg[u].n2
                    << " fid2:" << sg[u].f2
                    << std::endl;
            }

        // Forward or Cross  Edge
        /*
         * This should be called in forward or cross edges but I never saw it. 
         */
        template < typename Edge, typename Graph >
            void forward_or_cross_edge(Edge u, const Graph & g) const
            {
                std::cout << "forward or cross edge nid:" << sg[u].node << std::endl;
            }


};


namespace scenegraph
{

    /*!
     * Update all the scenegraph based on the current state mode.
     * State Modes = { None, DoIt, DrawIt, DrawGL, DrawSelection } 
     */
    status update()
    {
        // Temporary turn off do_it updating for testing
        // set the state node update 
        //cstate.sgMode = state::DoIt;
        cstate.clear_uid_update();
 
        node_visitor vis;
        //node_d_visitor vis;

        std::cout << "\n*****GRAPH UPDATE*****\n";

        // clear out the update queue first
        clear_update_queue();
        // clear the call stack
        call_stack.clear();

        // walk the scenegraph
        breadth_first_search(sg, vertex(0, sg), visitor(vis));

        // update the nodes based on the uid order in the call_stack
        for(auto uid : call_stack) {
                status p = plugins.update_properties(sg[uid].node,sg[uid].fields);
                cstate.add_uid_to_update(uid);
                p = plugins.do_it(sg[uid].node,sg[uid].fields);
        }

        // add the all updated fields to the update queue and clear out
        // the field update flag
        std::vector<unsigned int> uids;
        get_nodes(uids);
        for(auto uid : uids) {
            std::cout << "update queue for uid:" << uid << std::endl;
            for(auto field : sg[uid].fields) {
                if(field->update){
                    add_node_to_update_queue(uid,field->id);
                    field->update=false;
                    std::cout << "uid:" << uid << " fid:" << field->id << " has been added to update queue\n";
                }
            }
        }

        //FNodeDescriptor s = vertex(0, scenegraph);
           
           //dijkstra_shortest_paths(scenegraph, s,
           //predecessor_map(boost::make_iterator_property_map(p.begin(),
           //get(boost::vertex_index, scenegraph))).distance_map(boost::make_iterator_property_map(d.begin(),
           //get(boost::vertex_index, scenegraph))));
        std::cout << "*****UPDATE COMPLETE*****\n";

        return status();
    };


    /*!
     * Disconnect node n1 for n2
     */
    status disconnect(int suid, int sfid, int tuid, int tfid)
    {
        // verify that the disconnect rules are meet
        if(suid == tuid)
            return status(FAILED,"Node 1 can't be the same as Node 2");

        // still need to test that sfid is an input and tfid is an output
       
        // get the out edges of the source node
        typedef typename boost::graph_traits<FSceneGraph>::out_edge_iterator eo;
        std::pair<eo,eo> p = boost::out_edges(suid,sg);

        for(;p.first!=p.second;++p.first){
            if(sg[*p.first].f1 == sfid && sg[*p.first].f2 == tfid) {
                // TODO - there might be some cleanup to do here
                boost::remove_edge(p.first,sg);
                // remove the connection from the fields connections vector

                field::FieldBase* tfield = get_node_fieldBase(tuid,tfid);
                std::cout << "the target field has " << tfield->connections.size() << " connections\n";

                unsigned int i=0;
                for(auto conn : tfield->connections){
                    if(conn.puid == suid && conn.pfid == sfid) {
                        std::cout << "removed existing connection in the fields.connections array for parent uid:" << suid << " fid:" << sfid << " and target uid:" << tuid << " fid:" << tfid << std::endl;
                        tfield->connections.erase(tfield->connections.begin() + i);
                    }
                    i++;
                }
                std::cout << "there are now " << tfield->connections.size() << " connections\n";
                std::cout << "found connection to disconnect!\n";
            }
        }

        return status();   
    }


    /*!
     * Connect two node fields together.
     * For these fields to be connected they need to be of the same type and not in the same node.
     * If the input field already has a connection, it's input connection will be deleted and
     * replaced with a new between the passed fields
     */
    status connect(FNodeDescriptor n1, int f1, FNodeDescriptor n2, int f2)
    {
        std::cout << "Trying to connect nid: " << n1 << " fid: " << f1 << " to nid: " << n2 << " fid: " << f2 << std::endl;

        // can't connect two fields from the same node
        if(n1==n2)
            return status(FAILED,"Can't connect two fields from the same node");

        // see if the two types can be connected
        int src_node = sg[n1].node;
        int tgt_node = sg[n2].node;
        field::FieldBase* sfield = get_node_fieldBase(n1,src_node,f1);
        field::FieldBase* tfield = get_node_fieldBase(n2,tgt_node,f2);

        if(!sfield)
            return status(FAILED,"Can't connect nodes, no source field found");
        if(!tfield)
            return status(FAILED,"Can't connect nodes, no target field found");

        std::cout 
            << "*** CONNECT NODES *** sconn_type=" << sfield->conn_type
            << ", tconn_type=" << tfield->conn_type
            << ", sn=" << src_node 
            << ", sfield=" << sfield->id 
            << ", sfieldtype=" << sfield->type
            << ", tn=" << tgt_node 
            << ", tfield=" << tfield->id 
            << ", tfieldtype=" << tfield->type
            << std::endl;

        // verify the the n1's fid is an output and n2's fid is an input
        if(sfield->conn_type != field::connection::Out || tfield->conn_type != field::connection::In){
            std::cout << "could not connect - not OUT -> IN\n";
            return status(FAILED,"Mismatched connection types");
        }

        // can the fields be connected
        if(!field::can_types_connect<field::START,field::START>::exec(sfield->type,tfield->type)){
             std::cout << "could not connect - mismatched field types\n";
             return status(FAILED,"Field's types mismatched - could not connect");
        }

        // is the input field already connected to something else
        if(tfield->connected()){
            // if the tfield is any of the below types, then the connection needs to be removed
            if(tfield->type == field::Bool ||
                    tfield->type == field::Int ||
                    tfield->type == field::Float ||
                    tfield->type == field::Double ||
                    tfield->type == field::Real ||
                    tfield->type == field::Vertex ||
                    tfield->type == field::Vector ||
                    tfield->type == field::Mesh ||
                    tfield->type == field::RGB ||
                    tfield->type == field::RGBA ||
                    tfield->type == field::Time ||
                    tfield->type == field::Node ||
                    tfield->type == field::Matrix3x3 ||
                    tfield->type == field::Matrix4x4
              ) {
                // TODO - currently, for safety, I'm going through all the connections but there should only be one.
                // Later, after things seem more stable, I'll make it so that only the one connection is removed
                for(auto conn : tfield->connections)
                    disconnect(conn.puid, conn.pfid, n2, f2);
            }

            /*
            typedef typename boost::graph_traits<FSceneGraph>::out_edge_iterator ei;
            std::pair<ei,ei> p = boost::out_edges(n2,sg);

            //std::cout << "TEST edge iter " << *p.first << std::endl;

            for(;p.first!=p.second;++p.first){
                std::cout << "edge iter " << *p.first << " fid1:" << sg[*p.first].f1  << " fid2:" << sg[*p.first].f2 << std::endl;
                //uids.push_back(target(*p.first,sg));
            }
            */

        } 
             
        // check to see if another field is already connected
        if(field::can_types_connect<field::START,field::START>::exec(sfield->type,tfield->type)) {
            FFieldConnection connection = boost::add_edge(n1, n2, sg);
            sg[connection.first].n1 = n1;
            sg[connection.first].f1 = f1;
            sg[connection.first].n2 = n2;
            sg[connection.first].f2 = f2;
            sg[connection.first].sfield = sfield;
            sg[connection.first].tfield = tfield;
            field::Connection conn;
            conn.puid = n1;
            conn.pnid = src_node;
            conn.pfid = f1;
            tfield->connections.push_back(conn);

            std::cout << "connection add between " << n1  << ":" << f1 << " to " << n2 << ":" << f2 << " number of edges:" << boost::num_edges(sg) << std::endl;
        } else {
            std::cout << "could not connect nid " << n1 << " and nid " << n2 << std::endl;
            return status(FAILED,"field types can not be connected");
        }

        return status();
    };

 
    std::vector<unsigned int>* get_updated_nodes()
    {
        return &cstate.uid_update;
    }

    FTime get_time() { return time; };
 
    void set_time(FTime t) { time=t; };

    } // namespace scenegraph

    #define GET_NODE_DATA(nodedata)\
    template <> nodedata* DataObject::get_data<nodedata>(FNodeDescriptor node) { return static_cast<nodedata*>(sg[node].data); };
 
} // namespace feather

#endif
