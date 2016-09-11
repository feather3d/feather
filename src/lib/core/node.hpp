/***********************************************************************
 *
 * Filename: node.hpp
 *
 * Description: Describes the plugin nodes.
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

#ifndef NODE_HPP
#define NODE_HPP

#include "deps.hpp"
#include "types.hpp"
#include "status.hpp"
#include "object.hpp"
#include "field.hpp"

namespace feather
{

    namespace node
    {

        enum Type {
            Null,
            Empty,
            Camera,
            Time,
            Light,
            Texture,
            Animation,
            Modifier,
            Deformer,
            Manipulator,
            Shader,
            Object,
            Curve,
            Polygon,
            Shape,
            Tool,
            N
        };

        /*
         * This is used by the PluginManager to find
         * out if the node is in the plugin.
         */
        template <int _Node>
        static bool exist() { return false; };

    } // namespace node

} // namespace feather


#define DO_IT(__node_enum)\
    template <> status node_update_properties<__node_enum>(field::Fields& fields)\
    {\
        typedef field::Field<FReal>*  RealField;\
        typedef field::Field<FMatrix4x4>*  MatrixField;\
        RealField txIn;\
        RealField tyIn;\
        RealField tzIn;\
        RealField rxIn;\
        RealField ryIn;\
        RealField rzIn;\
        RealField sxIn;\
        RealField syIn;\
        RealField szIn;\
        MatrixField localMatrixOut;\
        MatrixField worldMatrixOut;\
        for(auto f : fields){\
            if(f->id==203)\
                txIn= static_cast<RealField>(f);\
            if(f->id==204)\
                tyIn= static_cast<RealField>(f);\
            if(f->id==205)\
                tzIn= static_cast<RealField>(f);\
            if(f->id==206)\
                rxIn= static_cast<RealField>(f);\
            if(f->id==207)\
                ryIn= static_cast<RealField>(f);\
            if(f->id==208)\
                rzIn= static_cast<RealField>(f);\
            if(f->id==209)\
                sxIn= static_cast<RealField>(f);\
            if(f->id==210)\
                syIn= static_cast<RealField>(f);\
            if(f->id==211)\
                szIn= static_cast<RealField>(f);\
            if(f->id==212)\
                localMatrixOut= static_cast<MatrixField>(f);\
            if(f->id==213)\
                worldMatrixOut= static_cast<MatrixField>(f);\
        }\
        FMatrix4x4 matrix;\
        tools::build_matrix(\
                txIn->value,\
                tyIn->value,\
                tzIn->value,\
                rxIn->value,\
                ryIn->value,\
                rzIn->value,\
                sxIn->value,\
                syIn->value,\
                szIn->value,\
                matrix\
                );\
        localMatrixOut->value = matrix;\
        return status();\
    };\
    \
    template <> status node_do_it<__node_enum>(field::Fields& fields)


#define NODE_INIT(__node_enum,__node_type,__node_icon)\
    namespace feather {\
        template <> status node_fields_init<__node_enum>(field::Fields& fields)\
        {\
            /* parent */\
            field::Field<FNode>* parent = new field::Field<FNode>();\
            parent->id = 201;\
            parent->value = FNode();\
            parent->type = field::Node;\
            parent->conn_type = field::connection::In;\
            fields.push_back(parent);\
            /* children */\
            field::Field<FNode>* children = new field::Field<FNode>();\
            children->id = 202;\
            children->value = FNode();\
            children->type = field::Node;\
            children->conn_type = field::connection::Out;\
            fields.push_back(children);\
            /* transformation fields */\
            /* translation */\
            /* tx */\
            field::Field<FReal>* tx = new field::Field<FReal>();\
            tx->id = 203;\
            tx->value = 0.0;\
            tx->type = field::Real;\
            tx->conn_type = field::connection::In;\
            fields.push_back(tx);\
            /* ty */\
            field::Field<FReal>* ty = new field::Field<FReal>();\
            ty->id = 204;\
            ty->value = 0.0;\
            ty->type = field::Real;\
            ty->conn_type = field::connection::In;\
            fields.push_back(ty);\
            /* tz */\
            field::Field<FReal>* tz = new field::Field<FReal>();\
            tz->id = 205;\
            tz->value = 0.0;\
            tz->type = field::Real;\
            tz->conn_type = field::connection::In;\
            fields.push_back(tz);\
            /* rotation */\
            /* rx */\
            field::Field<FReal>* rx = new field::Field<FReal>();\
            rx->id = 206;\
            rx->value = 0.0;\
            rx->type = field::Real;\
            rx->conn_type = field::connection::In;\
            fields.push_back(rx);\
            /* ry */\
            field::Field<FReal>* ry = new field::Field<FReal>();\
            ry->id = 207;\
            ry->value = 0.0;\
            ry->type = field::Real;\
            ry->conn_type = field::connection::In;\
            fields.push_back(ry);\
            /* rz */\
            field::Field<FReal>* rz = new field::Field<FReal>();\
            rz->id = 208;\
            rz->value = 0.0;\
            rz->type = field::Real;\
            rz->conn_type = field::connection::In;\
            fields.push_back(rz);\
            /* scale */\
            /* sx */\
            field::Field<FReal>* sx = new field::Field<FReal>();\
            sx->id = 209;\
            sx->value = 1.0;\
            sx->type = field::Real;\
            sx->conn_type = field::connection::In;\
            fields.push_back(sx);\
            /* sy */\
            field::Field<FReal>* sy = new field::Field<FReal>();\
            sy->id = 210;\
            sy->value = 1.0;\
            sy->type = field::Real;\
            sy->conn_type = field::connection::In;\
            fields.push_back(sy);\
            /* sz */\
            field::Field<FReal>* sz = new field::Field<FReal>();\
            sz->id = 211;\
            sz->value = 1.0;\
            sz->type = field::Real;\
            sz->conn_type = field::connection::In;\
            fields.push_back(sz);\
            /* local matrix */\
            field::Field<FMatrix4x4>* local = new field::Field<FMatrix4x4>();\
            local->id = 212;\
            local->value = FMatrix4x4();\
            local->type = field::Matrix4x4;\
            local->conn_type = field::connection::Out;\
            fields.push_back(local);\
            /* world matrix */\
            field::Field<FMatrix4x4>* world = new field::Field<FMatrix4x4>();\
            world->id = 213;\
            world->value = FMatrix4x4();\
            world->type = field::Matrix4x4;\
            world->conn_type = field::connection::Out;\
            fields.push_back(world);\
            return status();\
        };\
        \
        template <> struct call_fields_inits<__node_enum> {\
            static status exec(int id, field::Fields& fields) {\
                if(id==__node_enum){\
                    return node_fields_init<__node_enum>(fields);\
                } else {\
                    return call_fields_inits<__node_enum-1>::exec(id,fields);\
                }\
            };\
        };\
        \
        template <> struct call_update_properties<__node_enum> {\
            static status exec(int id, field::Fields& fields) {\
                if(id==__node_enum){\
                    return node_update_properties<__node_enum>(fields);\
                } else {\
                    return call_update_properties<__node_enum-1>::exec(id,fields);\
                }\
            };\
        };\
        \
        template <> struct call_do_its<__node_enum> {\
            static status exec(int id, field::Fields& fields) {\
                if(id==__node_enum){\
                    return node_do_it<__node_enum>(fields);\
                } else {\
                    return call_do_its<__node_enum-1>::exec(id,fields);\
                }\
            };\
        };\
        \
        template <> struct find_node_drawable<__node_enum> {\
            static bool exec(int id) {\
                if(id==__node_enum){\
                    return true;\
                } else {\
                    return find_node_drawable<__node_enum-1>::exec(id);\
                }\
            };\
        };\
        \
        template <> struct call_draw_items<__node_enum> {\
            static status exec(int id, draw::DrawItems& items) {\
                if(id==__node_enum){\
                    return node_draw_it<__node_enum>(items);\
                } else {\
                    return call_draw_items<__node_enum-1>::exec(id,items);\
                }\
            };\
        };\
        \
        template <> struct find_nodes<__node_enum> {\
            static bool exec(int id) {\
                if(id==__node_enum){\
                    return true;\
                } else {\
                    return find_nodes<__node_enum-1>::exec(id);\
                }\
            };\
        };\
        \
        template <> struct find_node_type<__node_enum> {\
            static status exec(int id,node::Type& type) {\
                if(id==__node_enum){\
                    type = __node_type;\
                    return status();\
                } else {\
                    return find_node_type<__node_enum-1>::exec(id,type);\
                }\
            };\
        };\
        \
        template <> struct find_node_icon<__node_enum> {\
            static bool exec(int id,std::string& file) {\
                if(id==__node_enum){\
                    file=__node_icon;\
                } else {\
                    find_node_icon<__node_enum-1>::exec(id,file);\
                }\
            };\
        };\
        \
        template <> struct find_create_fields<__node_enum> {\
            static  status exec(int id, field::Fields& fields) {\
                if(id==__node_enum){\
                    return add_fields<__node_enum,600>::exec(fields);\
                } else {\
                    return find_create_fields<__node_enum-1>::exec(id,fields);\
                }\
                return status();\
            };\
        };\
    }

#endif
