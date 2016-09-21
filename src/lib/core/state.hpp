/***********************************************************************
 *
 * Filename: state.hpp
 *
 * Description: Used for selection of items in the scenegraph.
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

#ifndef STATE_HPP
#define STATE_HPP

#include "deps.hpp"
#include "types.hpp"

namespace feather
{

    namespace state
    {

        enum SGMode { None, DoIt, DrawGL, DrawSelection };
        
        struct FSgState {
            int minUid;
            int maxUid;
        };

        struct FState {
            FState() : sgMode(None) { };
            SGMode sgMode;
            FSgState sgState;
            std::vector<unsigned int> uid_update;
            void clear_uid_update() { uid_update.clear(); };
            void add_uid_to_update(unsigned int uid) { uid_update.push_back(uid); };
        };

    } // namespace state

} // namespace feather

#endif
