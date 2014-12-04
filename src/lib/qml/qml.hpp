// =====================================================================================
// 
//       Filename:  qml.hpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  07/14/2014 05:41:06 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Richard Layman (), rlayman2000@yahoo.com
//        Company:  
// 
// =====================================================================================
#ifndef QML_HPP
#define QML_HPP

#include "deps.hpp"
#include "qml_deps.hpp"
#include "types.hpp"
#include "status.hpp"
#include "commands.hpp"
#include "node.hpp"
#include "object.hpp"
#include "field.hpp"

namespace feather
{

    namespace qml
    {

        class SceneGraph : public QObject
        {
            Q_OBJECT

            public:
                SceneGraph(QObject* parent=0);
                ~SceneGraph();

                // commands
                Q_INVOKABLE void add_node(int type, int node, int id) { command::add_node(type,node,id); };
        };

        // NODE
        class Node: public QObject
        {
            Q_OBJECT

                Q_ENUMS(Type)

            public:
                Node(QObject* parent=0);
                ~Node();

                enum Type {
                    Camera = node::Camera,
                    Light = node::Light,
                    Texture = node::Texture,
                    Shader = node::Shader,
                    Object = node::Object 
                };

            private:
                int m_id; // node id
                int m_uid; // scenegraph vertex
        };

        // FIELD 
        class Field: public QObject
        {
            Q_OBJECT
            Q_ENUMS(Type)

            public:
                Field(QObject* parent=0);
                ~Field();

                enum Type {
                    Bool=field::Bool,
                    Int=field::Int,
                    Float=field::Float,
                    Vertex=field::Vertex,
                    Vector=field::Vector,
                    Mesh=field::Mesh,
                    RGB=field::RGB,
                    RGBA=field::RGBA,
                    BoolArray=field::BoolArray,
                    IntArray=field::IntArray,
                    FloatArray=field::FloatArray,
                    VertexArray=field::VertexArray,
                    VectoryArray=field::VectoryArray,
                    RGBArray=field::RGBArray,
                    RGBAArray=field::RGBAArray
                };

            private:
                int m_id;
        };

    } // namespace qml

} // namespace feather

#endif
