/***********************************************************************
 *
 * Filename: import.hpp
 *
 * Description: Holds import descriptions.
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

#ifndef IMPORT_HPP
#define IMPORT_HPP

#include "types.hpp"
#include "deps.hpp"

namespace feather
{

    namespace import
    {

        enum Format {
            Obj,
            3ds,
            Dxf,
            Wings,
            Blender
        };

        template <int _Format> status import() { return Status(Failed,"format not supported"); };

    } // namespace import

} // namespace feather

#endif
