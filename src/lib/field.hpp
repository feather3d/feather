// =====================================================================================
// 
//       Filename:  field.hpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  07/13/2014 05:56:46 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Richard Layman (), rlayman2000@yahoo.com
//        Company:  
// 
// =====================================================================================
#ifndef FIELD_HPP
#define FIELD_HPP

#include "deps.hpp"
#include "types.hpp"
#include "status.hpp"

namespace feather
{

    namespace field
    {

        enum Type {
            EndField,
            Bool,
            Int,
            Double,
            String,
            ColorRGB,
            ColorRGBA,
            Vertex2D,
            Vertex3D,
            Normal3D,
            TextureCoord,
            Matrix4x4,
            IntArray,
            DoubleArray,
            ColorRGBArray,
            ColorRGBAArray,
            Vertex2DArray,
            Vertex3DArray,
            NormalArray,
            TextureCoordArray,
            Matrix4x4Array,
            StartField
        };

        template <int _Node, typename _T>
            struct Field
            {
                static status get_value(_T& val) { return status(FAILED,"No matching Node and Type for Field"); };
            };

        typedef struct {} FieldContainer;


    } // namespace field

#define MAKE_FIELD(_type,_name,_default,_min,_max)\
    typedef Field<_type> _name;\
    template <> _type _name::value=_default;

#define ADD_FIELD_TO_NODE(_name,_type,_node)\
    typedef Field<_node,get_type<_type>()> _name;\
    template <> status _name::get_value(get_type<_type>()& val) { return status(); };
    
} // namespace feather

#endif
