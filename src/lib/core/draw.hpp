/***********************************************************************
 *
 * Filename: draw.hpp
 *
 * Description: Used to draw items in the viewport.
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

#ifndef DRAW_HPP
#define DRAW_HPP

#include "deps.hpp"
#include "types.hpp"

namespace feather {

    namespace draw {

        struct Item {
            enum Type { None, PerspCamera, OrthoCamera, ShadedMesh, ComponentMesh, Line };
            Item(Type _type=None):type(_type){}; 
            Type type;
            unsigned int uid;
            unsigned int nid;
        };

        struct Line : public Item
        {
            enum Type { Solid, Dashed };
            Line(FVertex3D _sp, FVertex3D _ep, FColorRGB _color, Type _type=Solid) : Item(Item::Line), sp(_sp), ep(_ep), color(_color), linetype(_type) {};
            FVertex3D sp;
            FVertex3D ep;
            FColorRGB color;
            Type linetype;
        };

        struct ShadedMesh : public Item
        {
            ShadedMesh(unsigned int _fid) : Item(Item::ShadedMesh),fid(_fid) {};
            unsigned int fid;
        };

        struct ComponentMesh : public Item
        {
            ComponentMesh(unsigned int _fid) : Item(Item::ComponentMesh),fid(_fid) {};
            unsigned int fid;
        };

        struct PerspCamera : public Item
        {
            PerspCamera(
                    unsigned int _typefid,
                    unsigned int _fovfid,
                    unsigned int _nearfid,
                    unsigned int _farfid
                    ) :
                Item(Item::PerspCamera),
                typefid(_typefid),
                fovfid(_fovfid),
                nearfid(_nearfid),
                farfid(_farfid) {};

            unsigned int typefid;
            unsigned int fovfid;
            unsigned int nearfid;
            unsigned int farfid;
        };

        typedef std::vector<Item*> DrawItems;

    } // namespace draw

} // namespace feather

#define DRAW_IT(__node_enum)\
    template <> status node_draw_it<__node_enum>(draw::DrawItems& items)

#define ADD_LINE(__startpoint,__endpoint,__color,__type)\
    items.push_back(new draw::Line(__startpoint,__endpoint,__color,__type));
 
#define ADD_SHADED_MESH(__fid)\
    items.push_back(new draw::ShadedMesh(__fid));
  
#define ADD_COMPONENT_MESH(__fid)\
    items.push_back(new draw::ComponentMesh(__fid));
 
#define ADD_PERSP_CAMERA(__typefid,__fovfid,__nearfid,__farfid)\
    items.push_back(new draw::PerspCamera(__typefid,__fovfid,__nearfid,__farfid));
    
#endif
