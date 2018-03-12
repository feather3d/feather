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
#include "render_image.hpp"
#include "qml_status.hpp"
#include <QtQuick/QQuickPaintedItem>

using namespace feather;

// SELECTION 
class Selection : public QObject
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
            Node = selection::Node,
            Field = selection::Field
        };
};

// SELECTION 
class Connection : public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned int suid READ suid WRITE setSuid)
    Q_PROPERTY(unsigned int sfid READ sfid WRITE setSfid)
    Q_PROPERTY(unsigned int tuid READ tuid WRITE setSuid)
    Q_PROPERTY(unsigned int tfid READ tfid WRITE setSfid)

    public:
        Connection(QObject* parent=0);
        ~Connection();

        // suid 
        void setSuid(unsigned int& i) {
            if(m_suid != i) {
                m_suid=i;
            }
        }

        unsigned int suid() { return m_suid; }

        // sfid 
        void setSfid(unsigned int& i) {
            if(m_sfid != i) {
                m_sfid=i;
            }
        }

        unsigned int sfid() { return m_tfid; }

        // tuid 
        void setTuid(unsigned int& i) {
            if(m_tuid != i) {
                m_tuid=i;
            }
        }

        unsigned int tuid() { return m_tuid; }

        // sfid 
        void setTfid(unsigned int& i) {
            if(m_tfid != i) {
                m_tfid=i;
            }
        }

        unsigned int tfid() { return m_tfid; }

    private:
        unsigned int m_suid;
        unsigned int m_sfid;
        unsigned int m_tuid;
        unsigned int m_tfid;
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
        Q_INVOKABLE QList<int> get_nodes_by_type(unsigned int type);
        Q_INVOKABLE QList<int> get_nodes_by_id(unsigned int id);
        Q_INVOKABLE int node_id(int uid);
        Q_INVOKABLE int connect_nodes(int n1, int f1, int n2, int f2);
        Q_INVOKABLE unsigned int disconnect_nodes(unsigned int suid, unsigned int sfid, unsigned int tuid, unsigned int tfid);
        Q_INVOKABLE int selected_node();
        Q_INVOKABLE int select_node(unsigned int uid);
        //Q_INVOKABLE int select_node(int type, int uid);
        Q_INVOKABLE int select_field(unsigned int type, unsigned int uid, unsigned int fid);
        Q_INVOKABLE void clear_selection();
        Q_INVOKABLE int run_command_string(QString str);
        Q_INVOKABLE void triggerUpdate();
        Q_INVOKABLE void add_node_to_layer(int uid, int lid);
        Q_INVOKABLE bool connected(unsigned int uid, unsigned int fid);
        Q_INVOKABLE QList<int> connected_uids(unsigned int uid, unsigned int fid);

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
        void keyAdded(unsigned int uid, unsigned int nid, unsigned int fid);

};

class Curve : public QObject
{
    Q_OBJECT
         Q_ENUMS(Type)
    public:
        Curve(){};
        ~Curve(){};
        enum Type {
            Line = curve::Line,
            Bezier = curve::Bezier,
            Quadrtratic = curve::Quadtratic,
            Arc = curve::Arc,
            Svg = curve::Svg
        };
    
};

class KeyValue : public QObject
{
    Q_OBJECT
        Q_PROPERTY(float time READ time WRITE setTime NOTIFY timeChanged)
        Q_PROPERTY(float value READ value WRITE setValue NOTIFY valueChanged)
        Q_PROPERTY(int inCurve READ inCurve WRITE setInCurve NOTIFY inCurveChanged)
        Q_PROPERTY(float cp1x READ cp1x WRITE setCp1x NOTIFY cp1xChanged)
        Q_PROPERTY(float cp1y READ cp1y WRITE setCp1y NOTIFY cp1yChanged)
        Q_PROPERTY(int outCurve READ outCurve WRITE setOutCurve NOTIFY outCurveChanged)
        Q_PROPERTY(float cp2x READ cp2x WRITE setCp2x NOTIFY cp2xChanged)
        Q_PROPERTY(float cp2y READ cp2y WRITE setCp2y NOTIFY cp2yChanged)

    public:
        KeyValue(QObject* parent=0);
        ~KeyValue();

        // time 
        void setTime(float& v) {
            if(m_time != v) {
                m_time = v;
                emit timeChanged();
            }
        }

        float time() { return m_time; }

        // value 
        void setValue(float& v) {
            if(m_value != v) {
                m_value = v;
                emit valueChanged();
            }
        }

        float value() { return m_value; }

        // control point 1

        // in curve 
        void setInCurve(int& v) {
            if(m_incurve != v) {
                m_incurve = v;
                emit inCurveChanged();
            }
        }

        int inCurve() { return m_incurve; }

        void setCp1x(float& v) {
            if(m_cp1x != v) {
                m_cp1x = v;
                emit cp1xChanged();
            }
        }

        float cp1x() { return m_cp1x; }

        void setCp1y(float& v) {
            if(m_cp1y != v) {
                m_cp1y = v;
                emit cp1yChanged();
            }
        }

        float cp1y() { return m_cp1y; }


        // control point 2

        // out curve 
        void setOutCurve(int& v) {
            if(m_outcurve != v) {
                m_outcurve = v;
                emit outCurveChanged();
            }
        }

        int outCurve() { return m_outcurve; }

        void setCp2x(float& v) {
            if(m_cp2x != v) {
                m_cp2x = v;
                emit cp2xChanged();
            }
        }

        float cp2x() { return m_cp2x; }

        void setCp2y(float& v) {
            if(m_cp2y != v) {
                m_cp2y = v;
                emit cp2yChanged();
            }
        }

        float cp2y() { return m_cp2y; };

    signals:
        void timeChanged();
        void valueChanged();
        void inCurveChanged();
        void outCurveChanged();
        void cp1xChanged();
        void cp1yChanged();
        void cp2xChanged();
        void cp2yChanged();

    private:
        float m_time;
        float m_value; 
        int m_incurve; 
        float m_cp1x;
        float m_cp1y;
        int m_outcurve; 
        float m_cp2x;
        float m_cp2y;
};
 
// FIELD 
class Field: public QObject
{
    Q_OBJECT
        Q_ENUMS(Type)
        Q_ENUMS(ConnectionType)
        Q_PROPERTY(unsigned int uid READ uid WRITE setUid)
        Q_PROPERTY(unsigned int nid READ nid WRITE setNid)
        Q_PROPERTY(unsigned int fid READ fid WRITE setFid)
        Q_PROPERTY(int type READ type NOTIFY typeChanged)
        Q_PROPERTY(bool boolVal READ boolVal WRITE setBoolVal NOTIFY boolValChanged)
        Q_PROPERTY(int intVal READ intVal WRITE setIntVal NOTIFY intValChanged)
        Q_PROPERTY(double realVal READ realVal WRITE setRealVal NOTIFY realValChanged)
        //Q_PROPERTY(QQmlListProperty<KeyValue> keyArrayVal READ keyArrayVal WRITE setKeyArrayVal NOTIFY keyArrayValChanged)
        Q_PROPERTY(QQmlListProperty<KeyValue> keyArrayVal READ keyArrayVal NOTIFY keyArrayValChanged)
        //Q_PROPERTY(QList<KeyValue> keyArrayVal READ keyArrayVal WRITE setKeyArrayVal NOTIFY keyArrayValChanged)
        Q_PROPERTY(QList<double> realArrayVal READ realArrayVal WRITE setRealArrayVal NOTIFY realArrayValChanged)
        Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
        Q_PROPERTY(QQmlListProperty<Connection> connections READ connections )
 
    public:
        Field(QObject* parent=0);
        ~Field();

        // uid 
        void setUid(unsigned int& uid) {
            if(m_uid != uid) {
                m_uid = uid;
            }
        }

        unsigned int uid() { return m_uid; }

        // node 
        void setNid(unsigned int& nid) {
            if(m_nid != nid) {
                m_nid = nid;
            }
        }

        unsigned int nid() { return m_nid; }

        // field 
        void setFid(unsigned int& fid);

        unsigned int fid() { return m_fid; }

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

        // keyArrayVal
        KeyValue* key(int i) {
            return m_keyArrayVal[i];
        }

        // keyArrayVal
        Q_INVOKABLE void setKeyArrayValue(int i, float time, float value, int inCurve, float cp1x, float cp1y, int outCurve, float cp2x, float cp2y) {
            m_keyArrayVal[i]->setTime(time);
            m_keyArrayVal[i]->setValue(value);
            m_keyArrayVal[i]->setInCurve(inCurve);
            m_keyArrayVal[i]->setCp1x(cp1x);
            m_keyArrayVal[i]->setCp1y(cp1y);
            m_keyArrayVal[i]->setOutCurve(outCurve);
            m_keyArrayVal[i]->setCp2x(cp2x);
            m_keyArrayVal[i]->setCp2y(cp2y);
            set_key_array_val();
        }
 
        // keyArrayVal
        Q_INVOKABLE void setKeyArrayValue(float time, float value) {
            // set the value of a key at a certain time
            // If the key doesn't exist, make one
            bool found=false;
            int tval = time * 1000;
            for ( auto key : m_keyArrayVal ) {
                // we'll do some rounding just so we don't
                // end up with a lot of keyframes just a few msec
                // apart from eachother
                int keytime = key->time() * 1000;
                if ( tval == keytime ) {
                    // change value
                    key->setValue(value);
                    found = true;
                }
            }
            if ( !found ) {
                // make a new key
                KeyValue* key = new KeyValue();
                key->setTime(time);
                key->setValue(value);
                m_keyArrayVal.append(key);
                set_key_array_val();
            }
            emit keyArrayValChanged();
        }

        // this method will only return a value if there is
        // a key of the same time inside the keyarray. If
        // there is no matching time, a nullptr will be
        // returned. It's up to the caller to check to see
        // if a null has been returned and then look to
        // the track node to get a value if needed.
        Q_INVOKABLE KeyValue* getKey(float time) {
            // we'll do some rounding to keep away from
            // keys off by a few msecs
            int ttime = time * 1000;
            for ( auto key : m_keyArrayVal ) {
                int keytime = key->time() * 1000;
                if ( keytime == ttime )
                    return key;
            }
            return nullptr;
        }

        // This is like above except it only tells if a keyframe
        // already exist in the array at a specific time
        Q_INVOKABLE bool keyExist(float time) {
            // we'll do some rounding to keep away from
            // keys off by a few msecs
            int ttime = time * 1000;
            for ( auto key : m_keyArrayVal ) {
                int keytime = key->time() * 1000;
                if ( keytime == ttime )
                    return true;
            }
            return false;
        };

        QQmlListProperty<KeyValue> keyArrayVal();

        // realArrayVal 
        void setRealArrayVal(QList<double>& v) {
            if(m_realArrayVal != v) {
                //std::cout << "real changed\n";
                m_realArrayVal = v;
                set_real_array_val();
                emit realArrayValChanged();
            }
        }

        QList<double> realArrayVal();

        bool connected() { get_connected(); return m_connected; };

        QQmlListProperty<Connection> connections();

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
            RealArray=field::RealArray,
            VertexArray=field::VertexArray,
            VectorArray=field::VectorArray,
            RGBArray=field::RGBArray,
            RGBAArray=field::RGBAArray,
            Time=field::Time,
            Node=field::Node,
            NodeArray=field::NodeArray,
            Matrix3x3=field::Matrix3x3,
            Matrix4x4=field::Matrix4x4,
            VertexIndiceWeight=field::VertexIndiceWeight,
            VertexIndiceGroupWeight=field::VertexIndiceGroupWeight,
            VertexIndiceWeightArray=field::VertexIndiceWeightArray,
            VertexIndiceGroupWeightArray=field::VertexIndiceGroupWeightArray,
            MeshArray=field::MeshArray,
            Key=field::Key,
            KeyArray=field::KeyArray,
            CurvePoint2D=field::CurvePoint2D,
            CurvePoint3D=field::CurvePoint3D,
            CurvePoint2DArray=field::CurvePoint2DArray,
            CurvePoint3DArray=field::CurvePoint3DArray
        };

        enum ConnectionType {
            In=field::connection::In,
            Out=field::connection::Out,
        };

    signals:
        void typeChanged();
        void boolValChanged();
        void intValChanged();
        void realValChanged();
        void realArrayValChanged();
        void keyArrayValChanged();
        void connectedChanged();
 
    protected slots:
        void updateKeyArray();
        
    private:
        // get field value
        void get_bool_val();
        void get_int_val();
        void get_real_val();
        void get_key_array_val();

        // set field value
        void set_bool_val();
        void set_int_val();
        void set_real_val();
        void set_real_array_val();
        void set_key_array_val();

        void get_connected();
 
        unsigned int m_uid; // unique number of the node in the sg
        unsigned int m_nid; // node key
        unsigned int m_fid; // field key
        Type m_type; // type
        bool m_boolVal;
        int m_intVal;
        FReal m_realVal;
        QList<double> m_realArrayVal;
        QList<KeyValue*> m_keyArrayVal;
        bool m_connected;
        QList<Connection*> m_connections;
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


// PLUGINS
struct PluginInterfaceObject
{
    int render_id;
    std::string path;           // path to qml code
};


class PluginInterface : public QObject
{
    Q_OBJECT

    public:
        PluginInterface();
        ~PluginInterface();

        Q_INVOKABLE void load();
        Q_INVOKABLE bool render_globals_interface(const int render_id, QString& path);

    private:
        QList<PluginInterfaceObject> m_aInterfaces;
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


class Render: public QObject
{
    Q_OBJECT
    Q_PROPERTY(RenderImage* image READ image WRITE setImage NOTIFY imageChanged)
 
    public:
        Render(QObject* parent=0): m_pImage(nullptr) {};
        ~Render(){};
 
        // nid 
        void setImage(RenderImage* i) {
            if(m_pImage != i) {
                m_pImage = i;
                emit imageChanged();
            }
        };

        RenderImage* image() { return m_pImage; };

        Q_INVOKABLE void render_start(int id);
        Q_INVOKABLE void render_stop(int id);
        Q_INVOKABLE void render_buffer(int id);

    signals:
        void imageChanged();

    private:
        RenderImage* m_pImage;
};


#endif
