/***********************************************************************
 *
 * Filename: tools.cpp
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

#include "tools.hpp"

using namespace feather;

void tools::build_matrix(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz, FMatrix4x4 &matrix)
{
    // scale
    matrix.value[0][0] = sx;
    matrix.value[1][1] = sy;
    matrix.value[2][2] = sz;
    // rotation
    // TODO
    // translation
    matrix.value[0][3] = tx;
    matrix.value[1][3] = ty;
    matrix.value[2][3] = tz;
}

void tools::apply_matrix_to_mesh(FMatrix4x4 *matrix, FMesh &mesh)
{
    for(int i=0; i < mesh.v.size(); i++){
        mesh.v[i].x += matrix->value[0][3];
        mesh.v[i].y += matrix->value[1][3];
        mesh.v[i].z += matrix->value[2][3];
    }
}

FVertex3D tools::get_matrix_translation(FMatrix4x4 *matrix)
{
    return FVertex3D(matrix->value[0][3],
            matrix->value[1][3],
            matrix->value[2][3]);
}

FVertex3D tools::get_matrix_rotation(FMatrix4x4 *matrix)
{
    return FVertex3D();
}

FVertex3D tools::get_matrix_scale(FMatrix4x4 *matrix)
{
    return FVertex3D(matrix->value[0][0],
            matrix->value[1][1],
            matrix->value[2][2]);
}

