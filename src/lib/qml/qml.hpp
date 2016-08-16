/***********************************************************************
 *
 * Filename: qml.hpp
 *
 * Description: Frontend of the QML wrapper.
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

#ifndef QML_HPP
#define QML_HPP

#include "deps.hpp"
#include "qml_deps.hpp"
#include "types.hpp"
#include "selection.hpp"
#include "status.hpp"
#include "commands.hpp"
#include "node.hpp"
#include "object.hpp"
#include "field.hpp"
#include <QtQuick/QQuickPaintedItem>

using namespace feather;

// SELECTION 
class Selection: public QObject
{
    Q_OBJECT
        Q_ENUMS(Type)
    public:
        Selection(){};
        ~Selection(){};
        enum Type {
            Vertex = selection::Vertex,
            Edge = selection::Edge,
            Face = selection::Face,
            Object = selection::Object,
            Field = selection::Field,
        };
};

// SCENEGRAPH 
class SceneGraph : public QObject
{
    Q_OBJECT

    public:
        SceneGraph(QObject* parent=0);
        ~SceneGraph();

        // commands
        Q_INVOKABLE void clear();
        Q_INVOKABLE int add_node(int node, QString name);
        Q_INVOKABLE bool remove_node(int uid);
        Q_INVOKABLE QString node_name(int uid);
        Q_INVOKABLE int get_node_by_name(QString name);
        Q_INVOKABLE int node_id(int uid);
        Q_INVOKABLE int connect_nodes(int n1, int f1, int n2, int f2);
        Q_INVOKABLE int selected_node();
        Q_INVOKABLE int select_node(int uid);
        Q_INVOKABLE int select_node(int type, int uid);
        Q_INVOKABLE int select_field(int type, int uid, int fid);
        Q_INVOKABLE void clear_selection();
        Q_INVOKABLE int run_command_string(QString str);
        Q_INVOKABLE void triggerUpdate();
        Q_INVOKABLE void add_node_to_layer(int uid, int lid);
        Q_INVOKABLE bool connected(unsigned int uid, unsigned int fid);
        Q_INVOKABLE QList<unsigned int> connected_fields(unsigned int uid, unsigned int fid);

    signals:
        void nodeSelected(); // this will inform the widget to update it's selection from the selection manager
        void fieldSelected(int uid, int fid);
        void commandMessageChanged(int code, QString msg);
        void updateGraph();
        void nodeAdded(unsigned int uid);
        //void nodesAdded();
        //void nodesAdded(QList<unsigned int> uids);
        void nodeAddDrawItems(unsigned int uid);
        void nodeUpdateDrawItems(unsigned int uid);
        void nodeRemoved(int uid);
        void nodesRemoved();
        void cleared();
        void nodeFieldChanged(unsigned int uid, unsigned int nid, unsigned int fid);
};

// FIELD 
class Field: public QObject
{
    Q_OBJECT
        Q_ENUMS(Type)
        Q_PROPERTY(int uid READ uid WRITE setUid)
        Q_PROPERTY(int node READ node WRITE setNode)
        Q_PROPERTY(int field READ field WRITE setField)
        Q_PROPERTY(int type READ type NOTIFY typeChanged)
        Q_PROPERTY(bool boolVal READ boolVal WRITE setBoolVal NOTIFY boolValChanged)
        Q_PROPERTY(int intVal READ intVal WRITE setIntVal NOTIFY intValChanged)
        Q_PROPERTY(double realVal READ realVal WRITE setRealVal NOTIFY realValChanged)
        Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
 
    public:
        Field(QObject* parent=0);
        ~Field();

        // uid 
        void setUid(int& i) {
            if(m_uid != i) {
                m_uid=i;
            }
        }

        int uid() { return m_uid; }

        // node 
        void setNode(int& n) {
            if(m_node != n) {
                m_node=n;
            }
        }

        int node() { return m_node; }

        // field 
        void setField(int& f) {
            if(m_field != f) {
                m_field=f;
            }
        }

        int field() { return m_field; }

        // type 
        int type();

        // boolVal 
        void setBoolVal(bool& v) {
            if(m_boolVal != v) {
                m_boolVal = v;
                set_bool_val();
                emit boolValChanged();
            }
        }

        bool boolVal() { get_bool_val(); return m_boolVal; }

        // intVal 
        void setIntVal(int& v) {
            if(m_intVal != v) {
                //std::cout << "int changed\n";
                m_intVal = v;
                set_int_val(); 
                emit intValChanged();
            }
        }

        int intVal() { get_int_val(); return m_intVal; }

        // realVal 
        void setRealVal(double& v) {
            if(m_realVal != v) {
                //std::cout << "real changed\n";
                m_realVal = v;
                set_real_val();
                emit realValChanged();
            }
        }

        FReal realVal() { get_real_val(); return m_realVal; };

        bool connected() { get_connected(); return m_connected; };

        enum Type {
            Bool=field::Bool,
            Int=field::Int,
            Float=field::Float,
            Double=field::Double,
            Real=field::Real,
            Vertex=field::Vertex,
            Vector=field::Vector,
            Mesh=field::Mesh,
            RGB=field::RGB,
            RGBA=field::RGBA,
            BoolArray=field::BoolArray,
            IntArray=field::IntArray,
            FloatArray=field::FloatArray,
            VertexArray=field::VertexArray,
            VectorArray=field::VectorArray,
            RGBArray=field::RGBArray,
            RGBAArray=field::RGBAArray,
            Time=field::Time
        };

    signals:
        void typeChanged();
        void boolValChanged();
        void intValChanged();
        void realValChanged();
        void connectedChanged();
        
    private:
        // get field value
        void get_bool_val();
        void get_int_val();
        void get_real_val();

        // set field value
        void set_bool_val();
        void set_int_val();
        void set_real_val();

        void get_connected();

        int m_uid; // unique number of the node in the sg
        int m_node; // node key
        int m_field; // field key
        Type m_type; // type
        bool m_boolVal;
        int m_intVal;
        FReal m_realVal;
        bool m_connected;
};


// NODE
class Node: public QObject
{
    Q_OBJECT
        Q_ENUMS(Type)
        //Q_PROPERTY(QQmlListProperty<Field> inFields READ inFields)
        //Q_PROPERTY(QQmlListProperty<Field> outFields READ outFields)
        Q_PROPERTY(unsigned int uid READ uid WRITE setUid NOTIFY uidChanged)
        Q_PROPERTY(unsigned int nid READ nid WRITE setNid NOTIFY nidChanged)
 
    public:
        Node(QObject* parent=0);
        ~Node();
        //QQmlListProperty<Field> inFields();
        //QQmlListProperty<Field> outFields();

        // uid 
        void setUid(unsigned int& i) {
            if(m_uid != i) {
                m_uid = i;
                emit uidChanged();
            }
        };

        unsigned int uid() { return m_uid; };

        // nid 
        void setNid(unsigned int& i) {
            if(m_nid != i) {
                m_nid = i;
                emit nidChanged();
            }
        };

        unsigned int nid() { return m_nid; };

        Q_INVOKABLE unsigned int field_count();
        Q_INVOKABLE unsigned int in_field_count();
        Q_INVOKABLE unsigned int out_field_count();
        Q_INVOKABLE QList<unsigned int> in_fields();
        Q_INVOKABLE QList<unsigned int> out_fields();
 
        enum Type {
            Empty = node::Empty,
            Camera = node::Camera,
            Time = node::Time,
            Light = node::Light,
            Texture = node::Texture,
            Animation = node::Animation,
            Modifier = node::Modifier,
            Deformer = node::Deformer,
            Manipulator = node::Manipulator,
            Shader = node::Shader,
            Object = node::Object,
            Curve = node::Curve,
            Polygon = node::Polygon,
            Shape = node::Shape,
            Tool = node::Tool
        };

    signals:
       void uidChanged();
       void nidChanged();

    private:
        unsigned int m_uid; // node id
        unsigned int m_nid; // node id
        //int m_id; // node id
        //int m_uid; // scenegraph vertex
        //static void append_inField(QQmlListProperty<Field> *list, Field *field);
        //static void append_outField(QQmlListProperty<Field> *list, Field *field);
        //QList<Field *> m_inFields;
        //QList<Field *> m_outFields;
};


// Parameter
class Parameter : public QObject
{
    Q_OBJECT
        Q_ENUMS(Type)
        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(Type type READ type WRITE setType)
        Q_PROPERTY(bool boolValue READ boolValue WRITE setBoolValue)
        Q_PROPERTY(int intValue READ intValue WRITE setIntValue)
        Q_PROPERTY(double realValue READ realValue WRITE setRealValue)
        Q_PROPERTY(QString stringValue READ stringValue WRITE setStringValue)

    public:
        Parameter(QObject* parent=0);
        ~Parameter();

        enum Type {
            Bool=parameter::Bool,
            Int=parameter::Int,
            Real=parameter::Real,
            String=parameter::String
        };

        // name
        void setName(const QString& n) {
            if(m_name != n) {
                m_name=n;
            }
        }

        QString name() const { return m_name; }

        // type 
        void setType(Type& t) {
            if(m_type != t) {
                m_type=t;
            }
        }

        Type type() { return m_type; }

        // boolValue 
        void setBoolValue(bool& v) {
            if(m_bool != v) {
                m_bool=v;
            }
        }

        bool boolValue() { return m_bool; }

        // intValue 
        void setIntValue(int& v) {
            if(m_int != v) {
                m_int=v;
            }
        }

        int intValue() { return m_int; }

        // realValue 
        void setRealValue(double& v) {
            if(m_real != v) {
                m_real=v;
            }
        }

        double realValue() { return m_real; }

        // stringValue 
        void setStringValue(QString& v) {
            if(m_string != v) {
                m_string=v;
            }
        }

        QString stringValue() { return m_string; }

    private:
        QString m_name;
        Type m_type;
        bool m_bool;
        int m_int;
        double m_real;
        QString m_string;
};

// Command
class Command : public QObject
{
    Q_OBJECT
        Q_PROPERTY(QQmlListProperty<Parameter> parameters READ parameters)
        Q_PROPERTY(QString name READ name WRITE setName)

    public:
        Command(QObject* parent=0);
        ~Command();

        void setName( const QString& n) {
            m_name=n;
        };

        QString name() const { return m_name; };

        QQmlListProperty<Parameter> parameters();

        Q_INVOKABLE bool exec();

    private:
        QString m_name;
        static void append_parameter(QQmlListProperty<Parameter> *list, Parameter *parameter);
        QList<Parameter *> m_parameters;
};


class PluginObject { 
    public:
        PluginObject(const QString &_name,
                const QString &_description,
                const QString &_author):
            name(_name),
            description(_description),
            author(_author) {}
        QString name;
        QString description;
        QString author;
};


class Plugins : public QAbstractListModel
{
    Q_OBJECT

    public:
        Plugins(QObject* parent=0);
        ~Plugins();

        enum ERoles
        {
            NameRole = Qt::UserRole + 1,
            DescriptionRole = Qt::UserRole + 2,
            AuthorRole = Qt::UserRole + 3
        };

        QHash<int, QByteArray> roleNames() const;
        int rowCount(const QModelIndex& parent = QModelIndex()) const;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
        Q_INVOKABLE void load();

    private:
        Q_DISABLE_COPY(Plugins);
        QList<PluginObject*> m_items;
};

class Tools : public QObject
{
    Q_OBJECT

    public:
        Tools(QObject* parent=0){};
        ~Tools(){};
        Q_INVOKABLE QString urlToString(QUrl url) { return url.path(); };
};

#endif
