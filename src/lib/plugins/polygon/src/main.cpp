// =====================================================================================
// 
//       Filename:  main.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  11/03/2014 04:44:11 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Richard Layman (), rlayman2000@yahoo.com
//        Company:  
// 
// =====================================================================================
#include "deps.hpp"
#include "pluginmanager.hpp"
#include "field.hpp"


#ifdef __cplusplus
extern "C" {
#endif

    int get_id();
    bool call_node(int *);
    feather::status do_it(int, feather::PluginNodeFields*);
    bool node_match(int,int);
    feather::status add_node(int, feather::PluginNodeFields*);
    feather::status remove_node(int, feather::PluginNodeFields*);
    feather::field::FieldBase* get_field(int,int);

#ifdef __cplusplus
}
#endif

using namespace feather;

#define POLYGON_PLANE 325
#define POLYGON_CUBE 326

int get_id() { return 325; };

bool call_node(int *) {
    std::cout << "plugin called\n"; return true;
};

// call node do_it()'s
feather::status do_it(int id, feather::PluginNodeFields* fields) {
    return call_do_its<MAX_NODE_ID>::exec(id,fields);
};

// see if the node is in the plugin
bool node_match(int id) {
    return find_nodes<MAX_NODE_ID>::exec(id);
};

feather::status add_node(int id, feather::PluginNodeFields* fields) {
    return feather::status(FAILED, "function not yet working");
};

// get the field
feather::field::FieldBase* get_field(int nid, int fid, PluginNodeFields* fields) {
    switch(nid) {
        case POLYGON_PLANE:
            return find_field<POLYGON_PLANE,2>::exec(fid,fields);
        case POLYGON_CUBE:
            return find_field<POLYGON_CUBE,2>::exec(fid,fields);
        default:
            return NULL;
    }
};

namespace feather {

    // POLYGON PLANE NODE SETUP

    // FIELDS
    struct PolygonPlaneFields : public PluginNodeFields
    {
        field::Field<int> *subX;
        field::Field<int> *subY;
    };

    // FIELD DATA
    template <> field::FieldBase* field_data<POLYGON_PLANE,1>(PluginNodeFields* fields)
    {
        PolygonPlaneFields* f = static_cast<PolygonPlaneFields*>(fields);
        return f->subX;   
    };

    // FIND THE FIELD DATA
    template <> struct find_field<POLYGON_PLANE,1> {
        static field::FieldBase* exec(int fid, PluginNodeFields* fields) {
            if(fid==1)
                return field_data<POLYGON_PLANE,1>(fields);
            else
                return field_data<POLYGON_PLANE,1-1>(fields);
        };
    };

    // functions
 
    // do_it
    template <> status node_do_it<POLYGON_PLANE>(PluginNodeFields* fields) {
        PolygonPlaneFields* plane = static_cast<PolygonPlaneFields*>(fields);
        std::cout << "plane: subX:" << plane->subX << std::endl;
        return status();
    };

    // POLYGON PLANE NODE CALLS

    // plugin call do it()
    template <> struct call_do_its<POLYGON_PLANE> {
        static status exec(int id, PluginNodeFields* fields) {
            if(id==POLYGON_PLANE){
                return node_do_it<POLYGON_PLANE>(fields);
            } else {
                return call_do_its<POLYGON_PLANE-1>::exec(id,fields);
            }
        };
    };

    // plugin find node
    template <> struct find_nodes<POLYGON_PLANE> {
        static bool exec(int id) {
            if(id==POLYGON_PLANE){
                return true; 
            } else {
                return find_nodes<POLYGON_PLANE-1>::exec(id);
            }
        };
    };

    
    // CUBE NODE

    struct PolygonCubeFields : public PluginNodeFields
    {
        int subX;
        int subY;
        int subZ;
    };

    // functions

    // do_it
    template <> status node_do_it<POLYGON_CUBE>(PluginNodeFields* fields) {
        PolygonCubeFields* cube = static_cast<PolygonCubeFields*>(fields);
        std::cout << "cube: subX:" << cube->subX << std::endl;
        return status();
    };

    // plugin call do_it()
    template <> struct call_do_its<POLYGON_CUBE> {
        static status exec(int id, PluginNodeFields* fields) {
            if(id==POLYGON_CUBE){
                return node_do_it<POLYGON_CUBE>(fields);
            } else {
                return call_do_its<POLYGON_CUBE-1>::exec(id,fields);
            }
        };
    }; 

    // plugin find node
    template <> struct find_nodes<POLYGON_CUBE> {
        static bool exec(int id) {
            if(id==POLYGON_CUBE){
                return true; 
            } else {
                return find_nodes<POLYGON_CUBE-1>::exec(id);
            }
        };
    };

}


