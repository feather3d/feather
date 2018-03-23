/***********************************************************************
 *
 * Filename: tools.hpp
 *
 * Description: a collection of various functions to aid developers.
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
 * ***********************************************************************/


#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "deps.hpp"
#include "types.hpp"

namespace feather
{

    namespace tools
    {

        // modify the matrix based on the input translate, rotation, and scale values
        void build_matrix(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz, FMatrix4x4 &matrix);

        void apply_matrix_to_mesh(FMatrix4x4 *matrix, FMesh &mesh);
        void modify_vertex(FReal weight, FMatrix4x4 *matrix, FVertex3D &v);
        FVertex3D get_matrix_translation(FMatrix4x4 *matrix);
        FVertex3D get_matrix_rotation(FMatrix4x4 *matrix);
        FVertex3D get_matrix_scale(FMatrix4x4 *matrix);


        // convert value
        template < typename __Source, typename __Target >
        void convert(__Source s, __Target& t) { t=s; };
        // bool
        template <> void convert<bool,uint32_t>(bool,uint32_t&);
        template <> void convert<bool,int>(bool,int&);
        template <> void convert<bool,double>(bool,double&);
        template <> void convert<bool,std::string>(bool,std::string&);
        // uint32_t
        template <> void convert<uint32_t,bool>(uint32_t,bool&);
        template <> void convert<uint32_t,int>(uint32_t,int&);
        template <> void convert<uint32_t,double>(uint32_t,double&);
        template <> void convert<uint32_t,std::string>(uint32_t,std::string&);
        // int
        template <> void convert<int,bool>(int,bool&);
        template <> void convert<int,uint32_t>(int,uint32_t&);
        template <> void convert<int,double>(int,double&);
        template <> void convert<int,std::string>(int,std::string&);
        // double
        template <> void convert<double,bool>(double,bool&);
        template <> void convert<double,uint32_t>(double,uint32_t&);
        template <> void convert<double,int>(double,int&);
        template <> void convert<double,std::string>(double,std::string&);
        // std::string
        template <> void convert<std::string,bool>(std::string,bool&);
        template <> void convert<std::string,uint32_t>(std::string,uint32_t&);
        template <> void convert<std::string,int>(std::string,int&);
        template <> void convert<std::string,double>(std::string,double&);

    } // namespace tools

} // namespace feather


#endif
