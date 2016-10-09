/***********************************************************************
 *
 * Filename: field.hpp
 *
 * Description: Holds the attribute value inside the node.
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

#ifndef FIELD_HPP
#define FIELD_HPP

namespace feather 
{
  
    namespace field
    {
        namespace connection
        {
            enum Type {
                In,
                Out
            };
        } // namespace connection

        struct Connection {
            unsigned int puid; // parent node id
            unsigned int pnid; // parent node type 
            unsigned int pfid; // parent field
        };

        enum Type {
            N=0,
            Bool=1,
            Int=2,
            Float=3,
            Double=4,
            Real=5,
            Vertex=6,
            Vector=7,
            Mesh=8,
            RGB=9,
            RGBA=10,
            BoolArray=11,
            IntArray=12,
            RealArray=13,
            VertexArray=14,
            VectorArray=15,
            RGBArray=16,
            RGBAArray=17,
            Time=18,
            Node=19,
            NodeArray=20,
            Matrix3x3=21,
            Matrix4x4=22,
            VertexIndiceWeight=23,
            VertexIndiceGroupWeight=24,
            VertexIndiceWeightArray=25,
            VertexIndiceGroupWeightArray=26,
            MeshArray=27,
            START=28
        };

        // TODO
        // The puid will change when nodes are removed, so it's need's to be updated
        struct FieldBase
        {
            FieldBase(): id(0), update(true), locked(false){};
            int id;
            bool update; // this is used to optimize the scenegraph update process - the sg won't call a node's do_it unless one of it's input's fields update flags are set to true.
            bool locked; // this will lock down the current value so that it can't be changed
            // Connections
            int conn_type;
            std::vector<Connection> connections; // this is a vector for array types
            int type;
            inline int connection_count() { std::cout << "connection size for " << id << " is " << connections.size() << std::endl; return connections.size(); };
            inline bool connected() { return (!connection_count()) ? false : true; }; 
        };

        //template <typename _Type, int _Conn>
        template <typename _Type>
        struct Field : public FieldBase
        {
            Field(int _type=0){};//{ conn_type=_Conn; };
            _Type value; // this is the field's value if nothing is connected to it
        };


        typedef std::vector<FieldBase*> Fields;


        // CHECK CONNECTION


        template <int _Type1, int _Type2>
        static bool can_connect() { return false; };

        // NOTE!!!!
        // THIS IS INCOMPLETE!!!!
        // I need to add all the instances for each type.
        // Currently this is just a brief list for testing.
        template <> bool can_connect<Bool,Bool>() { return true; };
        template <> bool can_connect<Bool,BoolArray>() { return true; };
        template <> bool can_connect<Int,Int>() { return true; };
        template <> bool can_connect<Int,IntArray>() { return true; };
        template <> bool can_connect<Double,Double>() { return true; };
        template <> bool can_connect<Float,Float>() { return true; };
        template <> bool can_connect<Real,Real>() { return true; };
        template <> bool can_connect<Real,RealArray>() { return true; };
        template <> bool can_connect<Mesh,Mesh>() { return true; };
        template <> bool can_connect<Mesh,MeshArray>() { return true; };
        template <> bool can_connect<Node,Node>() { return true; };
        template <> bool can_connect<Node,NodeArray>() { return true; };
        template <> bool can_connect<Time,Time>() { return true; };
        template <> bool can_connect<Matrix3x3,Matrix3x3>() { return true; };
        template <> bool can_connect<Matrix4x4,Matrix4x4>() { return true; };
      
        template <int _Type1, int _Type2>
        struct can_types_connect {
            static bool exec(int t1, int t2) {
                if(t1==_Type1 && t2==_Type2)
                    return can_connect<_Type1,_Type2>();
                else {
                    if(t2==_Type2) 
                        return can_types_connect<_Type1-1,_Type2>::exec(t1,t2);
                    else
                        return can_types_connect<_Type1,_Type2-1>::exec(t1,t2);
                }
            };
        };
   
        template <int _Type> struct can_types_connect<_Type,N> {
            static bool exec(int t1, int t2) { return false; };
        };
 
        template <int _Type> struct can_types_connect<N,_Type> {
            static bool exec(int t1, int t2) { return false; };
        };

    } // namespace field

#define ADD_FIELD_TO_NODE(__node,__type,__type_enum,__connection,__default_value,__field_key)\
    namespace feather {\
        template <> struct add_fields<__node,__field_key> {\
            static status exec(field::Fields& fields) {\
                field::Field<__type>* f = new field::Field<__type>();\
                f->id=__field_key;\
                f->value=__default_value;\
                f->type=__type_enum;\
                f->conn_type=__connection;\
                fields.push_back(f);\
                return add_fields<__node,__field_key-1>::exec(fields);\
            };\
        };\
        \
        /*\
        template <> field::FieldBase* field_data<__node,__field_key>(field::Fields& fields)\
        {\
            for(uint i=0; i < fields.size(); i++) {\
                if(fields.at(i)->id == __field_key){\
                    return fields.at(i);\
                }\
            }\
            return NULL;\
        };\
        \
        template <> struct find_field<__node,__field_key> {\
            static field::FieldBase* exec(int fid, field::Fields& fields) {\
                if(fid==__field_key){\
                    return field_data<__node,__field_key>(fields);\
                }else\
                    return find_field<__node,__field_key-1>::exec(fid,fields);\
            };\
        };\
        */\
    }

} // namespace feather 

#define GET_FIELD_DATA(__key,__type,__name,__conn)\
    field::Field<__type> *__name;\
    for(auto f : fields){ if(f->id==__key){ __name = static_cast<field::Field<__type>*>(f); } }\
    if(__conn == field::connection::In && __name->connections.size()){\
        __name->value = static_cast<field::Field<__type>*>(plugin::get_node_field_base(__name->connections.at(0).puid,__name->connections.at(0).pfid))->value;\
        __name->update = static_cast<field::Field<__type>*>(plugin::get_node_field_base(__name->connections.at(0).puid,__name->connections.at(0).pfid))->update;\
    }\

#define GET_FIELD_ARRAY_DATA(__key,__type,__name,__conn)\
    field::Field<__type> *__name;\
    for(auto f : fields){ if(f->id==__key){ __name = static_cast<field::Field<__type>*>(f); } }\


#endif
