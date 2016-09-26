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

    } // namespace tools

} // namespace feather


#endif
