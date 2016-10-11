/***********************************************************************
 *
 * Filename: selection.hpp
 *
 * Description: Manages selection of nodes in the scenegraph.
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

#ifndef SELECTION_HPP
#define SELECTION_HPP
#include "singleton.hpp"

namespace feather
{

    namespace selection {

        enum Type { Vertex, Edge, Face, Object, Node, Field };

        // Selection State
        struct SelectionState {
            SelectionState(
                    Type _type,
                    unsigned int _uid,
                    unsigned int _nid,
                    unsigned int _fid=0,
                    std::vector<unsigned int> _ids=std::vector<unsigned int>()
                    ) :
                type(_type),
                uid(_uid),
                nid(_nid),
                fid(_fid),
                ids(_ids)
            { };

            Type type;
            unsigned int uid;
            unsigned int nid;
            unsigned int fid;
            std::vector<unsigned int> ids;
        };

        // Selection Manager
        class SelectionManager {

            public:

                SelectionManager(){};

                ~SelectionManager(){};

                inline void clear() { m_aStates.clear(); };

                inline uint count() { return m_aStates.size(); };

                inline void add_state(Type _type, int _uid, int _nid, int _fid=0, std::vector<unsigned int> _ids=std::vector<unsigned int>()) {
                    // don't add the state if the uid is already selected
                    bool found=false;
                    std::for_each(m_aStates.begin(), m_aStates.end(), [_uid,&found](SelectionState s){if(s.uid==_uid){found=true;}});
                    if(!found)
                        m_aStates.push_back(SelectionState(_type,_uid,_nid,_fid,_ids));
                };

                //inline SelectionState& get_state(int _i) { return m_aStates[_i]; };

                inline selection::SelectionState* get_selection_state(unsigned int _uid, unsigned int _fid=0) {
                    // if there is no fid, just return the object or node if it exist
                    if(!_fid){
                        for ( auto state : m_aStates ) {
                            if(state.uid == _uid && (state.type == selection::Object || selection::Node))
                                return &state;
                        }
                    }
                    // look for a node selection with a matching fid
                    for ( auto state : m_aStates ) {
                        if ( state.uid == _uid && state.type == selection::Field )
                            return &state;
                    }

                    // if we made it this far, there is no selection
                    return nullptr; 
                };

                void get_selected_uids(std::vector<unsigned int>& uids) {
                    for ( auto state : m_aStates ) {
                        bool add = true;
                        for ( auto uid : uids ) {
                            if ( uid == state.uid )
                                add = false;
                        }
                        if ( add )
                            uids.push_back(state.uid);
                    }
                };

                //inline int get_uid(int _i) { return m_aStates.at(_i).uid; };

                inline bool selected(unsigned int _uid, unsigned int _fid=0) {
                    for ( auto state : m_aStates ) {
                        if ( state.uid == _uid && ( state.fid == _fid || _fid == 0 ) )
                            return true;
                    }
                    return false;
                };

                inline void remove_selection(unsigned int _uid) {
                    //m_aStates.remove_if ( [] (auto state) { return state.uid == _uid; } );
                    std::list<SelectionState>::iterator it;
                    it = m_aStates.begin();
                    // TODO - This seq faults if you let it continue after it removes
                    // an object, so I changed it so that it returns after the first
                    // match but it's possible to have more than one match so I need
                    // make it that all states with the uid are removed.
                    for ( auto state : m_aStates ) {
                        if ( state.uid == _uid ) {
                            m_aStates.erase(it);
                            return;
                        } else {
                            it++;
                        }
                    }
                };

            private:
                std::list<SelectionState> m_aStates;
        };

    } // namespace selection


    //static selection::SelectionManager smg;
    // Singleton
    typedef Singleton<selection::SelectionManager> smg;

} // namespace feather

#endif
