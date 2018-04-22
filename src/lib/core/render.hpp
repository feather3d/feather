/***********************************************************************
 *
 * Filename: render.hpp
 *
 * Description: Render calls for render plugins.
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

#ifndef RENDER_HPP
#define RENDER_HPP

#include "deps.hpp"
#include "types.hpp"
#include "status.hpp"
#include "object.hpp"
#include "field.hpp"

namespace feather
{

    namespace render 
    {

        enum BufferLayout {
            RGB888,
            RGB161616,
            RGB323232,
            GRAYSCALE8,
            GRAYSCALE16,
            GRAYSCALE32
        };


        struct RenderProperties {
            int id;
            std::string name;
        };


        struct RenderBuffer {
            RenderBuffer():layout(RGB888),width(0),height(0),data(nullptr){};
            BufferLayout layout;
            uint16_t width;
            uint16_t height;
            char* data;
        };

        // THIS IS NOT USED AT THIS POINT
        /*
         * This is used by the PluginManager to find
         * out if there is a render buffer for the plugin.
         */
        template <int _Id>
        static bool render_buffer_exist() { return false; };

        template <int _Id>
        static bool render_modify_exist() { return false; };

    }

}

#define RENDER_START(__render_enum)\
    template <> status render_start<__render_enum>(render::RenderProperties& props)
 
#define RENDER_STOP(__render_enum)\
    template <> status render_stop<__render_enum>(render::RenderProperties& props)
 
#define RENDER_BUFFER(__render_enum)\
    template <> status render_buffer<__render_enum>(render::RenderBuffer& buffer)
 
#define RENDER_MODIFY(__render_enum)\
    template <> status render_modify<__render_enum>(uint32_t uid, uint32_t nid, uint32_t fid)

#define RENDER_INIT(__render_enum, __name)\
    namespace feather {\
        template <> struct call_render_starts<__render_enum> {\
            static status exec(int id, render::RenderProperties& props) {\
                if(id==__render_enum){\
                    return render_start<__render_enum>(props);\
                } else {\
                    call_render_starts<__render_enum-1>::exec(id,props);\
                }\
            };\
        };\
        \
        template <> struct call_render_stops<__render_enum> {\
            static status exec(int id, render::RenderProperties& props) {\
                if(id==__render_enum){\
                    return render_stop<__render_enum>(props);\
                } else {\
                    call_render_stops<__render_enum-1>::exec(id,props);\
                }\
            };\
        };\
        \
       template <> status render_properties<__render_enum>(render::RenderProperties& props)\
        {\
            props.id = __render_enum;\
            props.name = __name;\
        };\
        \
        template <> struct call_render_properties<__render_enum> {\
            static status exec(int id, render::RenderProperties& props) {\
                if(id==__render_enum){\
                    return render_properties<__render_enum>(props);\
                } else {\
                    return call_render_properties<__render_enum-1>::exec(id,props);\
                }\
            };\
        };\
        \
        template <> struct find_renderers<__render_enum> {\
            static bool exec(int id) {\
                if(id==__render_enum)\
                    return true;\
                else\
                    find_renderers<__render_enum-1>::exec(id);\
            };\
        };\
        \
        template <> struct find_render_buffers<__render_enum> {\
            static bool exec(int id) {\
                if(id==__render_enum)\
                    return true;\
                else\
                    find_render_buffers<__render_enum-1>::exec(id);\
            };\
        };\
        \
        template <> struct call_render_buffers<__render_enum> {\
            static status exec(int id, render::RenderBuffer& buffer) {\
                if(id==__render_enum)\
                    return render_buffer<__render_enum>(buffer);\
                else\
                    call_render_buffers<__render_enum-1>::exec(id,buffer);\
            };\
        };\
        \
        template <> struct find_render_modifys<__render_enum> {\
            static bool exec(int id) {\
                if(id==__render_enum)\
                    return true;\
                else\
                    find_render_modifys<__render_enum-1>::exec(id);\
            };\
        };\
        \
        template <> struct call_render_modifys<__render_enum> {\
            static status exec(int id, uint32_t uid, uint32_t nid, uint32_t fid) {\
                if(id==__render_enum)\
                    return render_modify<__render_enum>(uid,nid,fid);\
                else\
                    call_render_modifys<__render_enum-1>::exec(id,uid,nid,fid);\
            };\
        };\
        \
    }\
 
#endif
