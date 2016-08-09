// =====================================================================================
// 
//       Filename:  core_access.hpp
// 
//    Description:  Gives access to core calls. 
// 
//        Version:  1.0
//        Created:  08/08/2016 09:34:47 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Richard Layman (), rlayman2000@yahoo.com
//        Company:  
// 
// =====================================================================================

#ifndef CORE_ACCESS_HPP
#define CORE_ACCESS_HPP

#include "deps.hpp"

namespace feather
{

    namespace api 
    {

        unsigned int get_node_by_name(std::string name);

    } // namespace api

} // namespace feather

#endif
