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

// TODO - once things get stable, add a check for this so that it's not updating every scenegraph update
#define DO_IT(__node_enum)\
    template <> status node_update_properties<__node_enum>(field::Fields& fields)\
    {\
        typedef field::Field<FReal>*  RealField;\
        typedef field::Field<FMatrix4x4>*  MatrixField;\
        typedef field::Field<FVertex3D>*  Vertex3DField;\
        typedef field::Field<FNode>*  NodeField;\
        RealField txIn;\
        RealField tyIn;\
        RealField tzIn;\
        RealField rxIn;\
        RealField ryIn;\
        RealField rzIn;\
        RealField sxIn;\
        RealField syIn;\
        RealField szIn;\
        Vertex3DField cpIn;\
        MatrixField localMatrixOut;\
        MatrixField worldMatrixOut;\
        MatrixField rotationMatrixOut;\
        RealField txOut;\
        RealField tyOut;\
        RealField tzOut;\
        RealField rxOut;\
        RealField ryOut;\
        RealField rzOut;\
        RealField sxOut;\
        RealField syOut;\
        RealField szOut;\
        \
        /* get the parent node's transformation */\
        NodeField parentIn;\
        for(auto f : fields){\
            if(f->id==201)\
                parentIn = static_cast<NodeField>(f);\
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
                cpIn= static_cast<Vertex3DField>(f);\
            if(f->id==213)\
                localMatrixOut= static_cast<MatrixField>(f);\
            if(f->id==214)\
                worldMatrixOut= static_cast<MatrixField>(f);\
            if(f->id==215)\
                rotationMatrixOut= static_cast<MatrixField>(f);\
            if(f->id==216)\
                txOut= static_cast<RealField>(f);\
            if(f->id==217)\
                tyOut= static_cast<RealField>(f);\
            if(f->id==218)\
                tzOut= static_cast<RealField>(f);\
            if(f->id==219)\
                rxOut= static_cast<RealField>(f);\
            if(f->id==220)\
                ryOut= static_cast<RealField>(f);\
            if(f->id==221)\
                rzOut= static_cast<RealField>(f);\
            if(f->id==222)\
                sxOut= static_cast<RealField>(f);\
            if(f->id==223)\
                syOut= static_cast<RealField>(f);\
            if(f->id==224)\
                szOut= static_cast<RealField>(f);\
        }\
        /* if any of the inputs are connected, use their value instead */\
        FMatrix4x4 parentWorldMatrix;\
        FMatrix4x4 parentRotationMatrix;\
        if(parentIn->connections.size()){\
            parentWorldMatrix = static_cast<MatrixField>(plugin::get_node_field_base(parentIn->connections.at(0).puid,214))->value;\
            std::cout << "parent matrix tx:" << parentWorldMatrix.value[0][3] << " ty:" << parentWorldMatrix.value[1][3]  << " tz:" << parentWorldMatrix.value[2][3] << std::endl;\
        }\
        if(txIn->connections.size())\
            txIn->value = static_cast<RealField>(plugin::get_field_base(txIn->connections.at(0).puid,txIn->connections.at(0).pfid))->value;\
        if(tyIn->connections.size())\
            tyIn->value = static_cast<RealField>(plugin::get_field_base(tyIn->connections.at(0).puid,tyIn->connections.at(0).pfid))->value;\
        if(tzIn->connections.size())\
            tzIn->value = static_cast<RealField>(plugin::get_field_base(tzIn->connections.at(0).puid,tzIn->connections.at(0).pfid))->value;\
        if(sxIn->connections.size())\
            sxIn->value = static_cast<RealField>(plugin::get_field_base(sxIn->connections.at(0).puid,sxIn->connections.at(0).pfid))->value;\
        if(syIn->connections.size())\
            syIn->value = static_cast<RealField>(plugin::get_field_base(syIn->connections.at(0).puid,syIn->connections.at(0).pfid))->value;\
        if(szIn->connections.size())\
            szIn->value = static_cast<RealField>(plugin::get_field_base(szIn->connections.at(0).puid,szIn->connections.at(0).pfid))->value;\
        if(rxIn->connections.size())\
            rxIn->value = static_cast<RealField>(plugin::get_field_base(rxIn->connections.at(0).puid,rxIn->connections.at(0).pfid))->value;\
        if(ryIn->connections.size())\
            ryIn->value = static_cast<RealField>(plugin::get_field_base(ryIn->connections.at(0).puid,ryIn->connections.at(0).pfid))->value;\
        if(rzIn->connections.size())\
            rzIn->value = static_cast<RealField>(plugin::get_field_base(rzIn->connections.at(0).puid,rzIn->connections.at(0).pfid))->value;\
        if(cpIn->connections.size())\
            cpIn->value = static_cast<Vertex3DField>(plugin::get_field_base(cpIn->connections.at(0).puid,cpIn->connections.at(0).pfid))->value;\
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
        /*localMatrixOut->value = matrix + parentWorldMatrix;\
        std::cout << "SETTTING THE NEW MATRIX\n";\
        std::cout << "matrix tx:" << matrix.value[0][3] << " ty:" << matrix.value[1][3]  << " tz:" << matrix.value[2][3] << std::endl;\
        std::cout << "parentWorldMatrix tx:" << parentWorldMatrix.value[0][3] << " ty:" << parentWorldMatrix.value[1][3]  << " tz:" << parentWorldMatrix.value[2][3] << std::endl;\
        std::cout << "localMatrixOut tx:" << localMatrixOut->value.value[0][3] << " ty:" << localMatrixOut->value.value[1][3]  << " tz:" << localMatrixOut->value.value[2][3] << std::endl;\
        */\
        localMatrixOut->value = matrix;\
        worldMatrixOut->value = matrix + parentWorldMatrix;\
        txOut->value = txIn->value;\
        tyOut->value = tyIn->value;\
        tzOut->value = tzIn->value;\
        rxOut->value = rxIn->value;\
        ryOut->value = ryIn->value;\
        rzOut->value = rzIn->value;\
        sxOut->value = sxIn->value;\
        syOut->value = syIn->value;\
        szOut->value = szIn->value;\
        localMatrixOut->update = true;\
        worldMatrixOut->update = true;\
        txOut->update = true;\
        tyOut->update = true;\
        tzOut->update = true;\
        rxOut->update = true;\
        ryOut->update = true;\
        rzOut->update = true;\
        sxOut->update = true;\
        syOut->update = true;\
        szOut->update = true;\
        std::cout << __node_enum << " NODE BASE OUT - x:" << txOut->value << " y:" << tyOut->value << " z:" << tzOut->value << std::endl;\
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
            /* IN FIELDS */\
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
            /* center point */\
            field::Field<FVertex3D>* cp = new field::Field<FVertex3D>();\
            cp->id = 212;\
            cp->value = FVertex3D();\
            cp->type = field::Vertex;\
            cp->conn_type = field::connection::In;\
            fields.push_back(cp);\
            /* OUT FIELDS */\
            /* local matrix */\
            field::Field<FMatrix4x4>* local = new field::Field<FMatrix4x4>();\
            local->id = 213;\
            local->value = FMatrix4x4();\
            local->type = field::Matrix4x4;\
            local->conn_type = field::connection::Out;\
            fields.push_back(local);\
            /* world matrix */\
            field::Field<FMatrix4x4>* world = new field::Field<FMatrix4x4>();\
            world->id = 214;\
            world->value = FMatrix4x4();\
            world->type = field::Matrix4x4;\
            world->conn_type = field::connection::Out;\
            fields.push_back(world);\
            /* rotation matrix */\
            field::Field<FMatrix4x4>* rotation = new field::Field<FMatrix4x4>();\
            rotation->id = 215;\
            rotation->value = FMatrix4x4();\
            rotation->type = field::Matrix4x4;\
            rotation->conn_type = field::connection::Out;\
            fields.push_back(rotation);\
            /* translation */\
            /* txOut */\
            field::Field<FReal>* txOut = new field::Field<FReal>();\
            txOut->id = 216;\
            txOut->value = 0.0;\
            txOut->type = field::Real;\
            txOut->conn_type = field::connection::Out;\
            fields.push_back(txOut);\
            /* tyOut */\
            field::Field<FReal>* tyOut = new field::Field<FReal>();\
            tyOut->id = 217;\
            tyOut->value = 0.0;\
            tyOut->type = field::Real;\
            tyOut->conn_type = field::connection::Out;\
            fields.push_back(tyOut);\
            /* tzOut */\
            field::Field<FReal>* tzOut = new field::Field<FReal>();\
            tzOut->id = 218;\
            tzOut->value = 0.0;\
            tzOut->type = field::Real;\
            tzOut->conn_type = field::connection::Out;\
            fields.push_back(tzOut);\
            /* rotation */\
            /* rxOut */\
            field::Field<FReal>* rxOut = new field::Field<FReal>();\
            rxOut->id = 219;\
            rxOut->value = 0.0;\
            rxOut->type = field::Real;\
            rxOut->conn_type = field::connection::Out;\
            fields.push_back(rxOut);\
            /* ryOut */\
            field::Field<FReal>* ryOut = new field::Field<FReal>();\
            ryOut->id = 220;\
            ryOut->value = 0.0;\
            ryOut->type = field::Real;\
            ryOut->conn_type = field::connection::Out;\
            fields.push_back(ryOut);\
            /* rzOut */\
            field::Field<FReal>* rzOut = new field::Field<FReal>();\
            rzOut->id = 221;\
            rzOut->value = 0.0;\
            rzOut->type = field::Real;\
            rzOut->conn_type = field::connection::Out;\
            fields.push_back(rzOut);\
            /* scale */\
            /* sxOut */\
            field::Field<FReal>* sxOut = new field::Field<FReal>();\
            sxOut->id = 222;\
            sxOut->value = 1.0;\
            sxOut->type = field::Real;\
            sxOut->conn_type = field::connection::Out;\
            fields.push_back(sxOut);\
            /* syOut */\
            field::Field<FReal>* syOut = new field::Field<FReal>();\
            syOut->id = 223;\
            syOut->value = 1.0;\
            syOut->type = field::Real;\
            syOut->conn_type = field::connection::Out;\
            fields.push_back(syOut);\
            /* szOut */\
            field::Field<FReal>* szOut = new field::Field<FReal>();\
            szOut->id = 224;\
            szOut->value = 1.0;\
            szOut->type = field::Real;\
            szOut->conn_type = field::connection::Out;\
            fields.push_back(szOut);\
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
