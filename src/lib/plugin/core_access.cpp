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
#include "types.hpp"
#include "scenegraph.hpp"
#include "field.hpp"

using namespace feather;

unsigned int api::get_node_by_name(std::string name)
{
    unsigned int uid;
    scenegraph::get_node_by_name(name,uid);
    return uid;
}
