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
    /*
    matrix.value[0][0] = matrix.value[0][0] * std::cos(-ry) * std::cos(-rz);
    matrix.value[0][1] = matrix.value[0][1] * std::cos(-ry) * -std::sin(-rz);
    matrix.value[0][2] = matrix.value[0][2] * std::sin(-ry);
    matrix.value[1][0] = matrix.value[1][0] * std::sin(-rz);
    matrix.value[1][1] = matrix.value[1][1] * std::cos(-rx) * std::cos(-rz);
    matrix.value[1][2] = matrix.value[1][2] * -std::sin(-rx);
    matrix.value[2][0] = matrix.value[2][0] * -std::sin(-ry);
    matrix.value[2][1] = matrix.value[2][1] * std::sin(-rx);
    matrix.value[2][2] = matrix.value[2][2] * std::cos(-rx) * std::cos(-ry);
    */
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

void tools::modify_vertex(FReal weight, FMatrix4x4 *matrix, FVertex3D &v)
{
    v.x = v.x + (matrix->value[0][3] * weight);
    v.y = v.y + (matrix->value[1][3] * weight);
    v.z = v.z + (matrix->value[2][3] * weight);
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


// Convert

// source bool
template <> void tools::convert<bool,uint32_t>(bool s, uint32_t& t) {
    if(s)
        t=1;
    else
        t=0;
};
template <> void tools::convert<bool,int>(bool s, int& t) {
    if(s)
        t=1;
    else
        t=0;
};
template <> void tools::convert<bool,double>(bool s, double& t) {
    if(s)
        t=1.0;
    else
        t=0.0;
};
template <> void tools::convert<bool,std::string>(bool s, std::string& t) {
    if(s)
        t="true";
    else
        t="false";
};

// source uint32_t
template <> void tools::convert<uint32_t,bool>(uint32_t s, bool& t) {
    if(s>0)
        t=true;
    else
        t=false;
};
template <> void tools::convert<uint32_t,int>(uint32_t s, int& t) {
    t=static_cast<int>(s);
};
template <> void tools::convert<uint32_t,double>(uint32_t s, double& t) {
    t=static_cast<double>(s);
};
template <> void tools::convert<uint32_t,std::string>(uint32_t s, std::string& t) {
    std::stringstream ss;
    ss << s;
    t=ss.str();
};

// source int
template <> void tools::convert<int,bool>(int s, bool& t) {
    if(s>0)
        t="true";
    else
        t="false";
};
template <> void tools::convert<int,uint32_t>(int s, uint32_t& t) {
    t=static_cast<uint32_t>(s);
};
template <> void tools::convert<int,double>(int s, double& t) {
    t=static_cast<double>(s);
};
template <> void tools::convert<int,std::string>(int s, std::string& t) {
    std::stringstream ss;
    ss << s;
    t=ss.str();
};

// source double
template <> void tools::convert<double,bool>(double s, bool& t) {
    if(s>=1.0)
        t="true";
    else
        t="false";
};
template <> void tools::convert<double,uint32_t>(double s, uint32_t& t) {
    t=static_cast<uint32_t>(s);
};
template <> void tools::convert<double,int>(double s, int& t) {
    t=static_cast<int>(s);
};
template <> void tools::convert<double,std::string>(double s, std::string& t) {
    std::stringstream ss;
    ss << s;
    t=ss.str();
};

// source std::string
template <> void tools::convert<std::string,bool>(std::string s, bool& t) {
    if(s=="true")
        t=true;
    else
        t=false;
};
template <> void tools::convert<std::string,uint32_t>(std::string s, uint32_t& t) {
    t=static_cast<uint32_t>(atoi(s.c_str()));
};
template <> void tools::convert<std::string,int>(std::string s, int& t) {
    t=atoi(s.c_str());
};
template <> void tools::convert<std::string,double>(std::string s, double& t) {
    t=static_cast<double>(atof(s.c_str()));
};


