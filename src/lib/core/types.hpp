/***********************************************************************
 *
 * Filename: types.hpp
 *
 * Description: Holds all the feather types.
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

#ifndef TYPES_HPP
#define TYPES_HPP

#include "deps.hpp"
#include "field.hpp"
#include "node.hpp"
//#include <QtGui/QOpenGLShaderProgram>
//#include <QtGui/QOpenGLShader>

namespace feather
{
    typedef bool FBool;
    typedef int FInt;
    typedef unsigned int FUInt;
    typedef float FFloat;
    typedef double FDouble;
    typedef double FReal;
    typedef std::string FString;

    struct FPoint2D
    {
        FPoint2D(float _x=0, float _y=0) : x(_x),y(_y){};
        float x;
        float y;
    };

    struct FPoint3D
    {
        FPoint3D(float _x=0, float _y=0, float _z=0) : x(_x),y(_y),z(_z){};
        float x;
        float y;
        float z;
    };

    struct FVertex3D
    {
        FVertex3D(FFloat _x=0, FFloat _y=0, FFloat _z=0):x(_x),y(_y),z(_z) {};
        FFloat x;
        FFloat y;
        FFloat z;
    };

    typedef struct{float x; float y; float z;} FNormal3D;
    typedef struct{ float x; float y; float z; float w;} FVector3D;

    struct FColorRGB
    {
        FColorRGB(FFloat _r=1.0, FFloat _g=1.0, FFloat _b=1.0):r(_r),g(_g),b(_b){};
        inline FInt int_red() { return 255 * r; };
        inline FInt int_green() { return 255 * g; };
        inline FInt int_blue() { return 255 * b; };
        inline void set_red(FInt _r) { r = _r / 255.0; };
        inline void set_green(FInt _g) { g = _g / 255.0; };
        inline void set_blue(FInt _b) { b = _b / 255.0; };
        FFloat r;
        FFloat g;
        FFloat b;
    };

    struct FColorRGBA
    {
        FColorRGBA(FFloat _r=1.0, FFloat _g=1.0, FFloat _b=1.0, FFloat _a=1.0):r(_r),g(_g),b(_b),a(_a){};
        inline FInt int_red() { return 255 * r; };
        inline FInt int_green() { return 255 * g; };
        inline FInt int_blue() { return 255 * b; };
        inline FInt int_alpha() { return 255 * a; };
        inline void set_red(FInt _r) { r = _r / 255.0; };
        inline void set_green(FInt _g) { g = _g / 255.0; };
        inline void set_blue(FInt _b) { b = _b / 255.0; };
        inline void set_alpha(FInt _a) { a = _a / 255.0; };
        FFloat r;
        FFloat g;
        FFloat b;
        FFloat a;
    };

    struct FTexture2D
    {
        FTexture2D(std::string _p=""):path(_p){};
        std::string path;
    };

    struct FColor
    {
        enum Type {
            Scalar,
            Color,
            Texture
        };

        FColor(Type _t=Scalar):type(_t){};

        boost::variant<FReal,FColorRGBA,FTexture2D> value;
        Type type;
    };
    
    struct FTransform
    {
        FVertex3D translate;
        FVector3D rotate;
        FVector3D scale;
    };

    struct FTextureCoord
    {
        FTextureCoord(FFloat _s=0, FFloat _t=0) { s=_s; t=_t; };
        FFloat s;
        FFloat t;
    };

    struct FFacePoint
    {
        FFacePoint(FUInt _v=0, FUInt _vt=0, FUInt _vn=0): v(_v), vt(_vt), vn(_vn) {};
        FUInt v;
        FUInt vt;
        FUInt vn;
    };

    //typedef struct{std::vector<FFacePoint> f; } FFace;
    typedef std::vector<FFacePoint> FFace;

    struct FMatrix3x3 {
        FMatrix3x3(){};
        float value[3][3] = {{1,0,0},{0,1,0},{0,0,0}};
    };

    struct FMatrix4x4 {
        FMatrix4x4(){};
        float value[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

        const FMatrix4x4 operator*(const FMatrix4x4& matrix) {
            FMatrix4x4 out = *this;
            this->value[0][0] = matrix.value[0][0] * this->value[0][0];
            this->value[0][1] = matrix.value[0][1] * this->value[0][1];
            this->value[0][2] = matrix.value[0][2] * this->value[0][2];
            this->value[0][3] = matrix.value[0][3] * this->value[0][3];
            this->value[1][0] = matrix.value[1][0] * this->value[1][0];
            this->value[1][1] = matrix.value[1][1] * this->value[1][1];
            this->value[1][2] = matrix.value[1][2] * this->value[1][2];
            this->value[1][3] = matrix.value[1][3] * this->value[1][3];
            this->value[2][0] = matrix.value[2][0] * this->value[2][0];
            this->value[2][1] = matrix.value[2][1] * this->value[2][1];
            this->value[2][2] = matrix.value[2][2] * this->value[2][2];
            this->value[2][3] = matrix.value[2][3] * this->value[2][3];
            this->value[3][0] = matrix.value[3][0] * this->value[3][0];
            this->value[3][1] = matrix.value[3][1] * this->value[3][1];
            this->value[3][2] = matrix.value[3][2] * this->value[3][2];
            this->value[3][3] = matrix.value[3][3] * this->value[3][3];
            return out;
        }

        const FMatrix4x4 operator+=(const FMatrix4x4& matrix) {
            FMatrix4x4 out = *this;
            this->value[0][0] = matrix.value[0][0] + this->value[0][0];
            this->value[0][1] = matrix.value[0][1] + this->value[0][1];
            this->value[0][2] = matrix.value[0][2] + this->value[0][2];
            this->value[0][3] = matrix.value[0][3] + this->value[0][3];
            this->value[1][0] = matrix.value[1][0] + this->value[1][0];
            this->value[1][1] = matrix.value[1][1] + this->value[1][1];
            this->value[1][2] = matrix.value[1][2] + this->value[1][2];
            this->value[1][3] = matrix.value[1][3] + this->value[1][3];
            this->value[2][0] = matrix.value[2][0] + this->value[2][0];
            this->value[2][1] = matrix.value[2][1] + this->value[2][1];
            this->value[2][2] = matrix.value[2][2] + this->value[2][2];
            this->value[2][3] = matrix.value[2][3] + this->value[2][3];
            this->value[3][0] = matrix.value[3][0] + this->value[3][0];
            this->value[3][1] = matrix.value[3][1] + this->value[3][1];
            this->value[3][2] = matrix.value[3][2] + this->value[3][2];
            this->value[3][3] = matrix.value[3][3] + this->value[3][3];
            return out;
        }

        friend FMatrix4x4 operator+(FMatrix4x4 lhs, const FMatrix4x4& rhs) {
            lhs.value[0][0] *= rhs.value[0][0];
            lhs.value[0][1] *= rhs.value[0][1];
            lhs.value[0][2] *= rhs.value[0][2];
            lhs.value[0][3] += rhs.value[0][3];
            lhs.value[1][0] *= rhs.value[1][0];
            lhs.value[1][1] *= rhs.value[1][1];
            lhs.value[1][2] *= rhs.value[1][2];
            lhs.value[1][3] += rhs.value[1][3];
            lhs.value[2][0] *= rhs.value[2][0];
            lhs.value[2][1] *= rhs.value[2][1];
            lhs.value[2][2] *= rhs.value[2][2];
            lhs.value[2][3] += rhs.value[2][3];
            lhs.value[3][0] *= rhs.value[3][0];
            lhs.value[3][1] *= rhs.value[3][1];
            lhs.value[3][2] *= rhs.value[3][2];
            lhs.value[3][3] += rhs.value[3][3];
            return lhs;
        }

        const void print() {
            std::cout << value[0][0] << " " << value[0][1] << " " << value[0][2] << " " << value[0][3] << std::endl;
            std::cout << value[1][0] << " " << value[1][1] << " " << value[1][2] << " " << value[1][3] << std::endl;
            std::cout << value[2][0] << " " << value[2][1] << " " << value[2][2] << " " << value[2][3] << std::endl;
            std::cout << value[3][0] << " " << value[3][1] << " " << value[3][2] << " " << value[3][3] << std::endl;
        }

        /*
        const FMatrix4x4 operator=(const FMatrix4x4& matrix) {
            FMatrix4x4 out = *this;
            this->value[0][0] = matrix.value[0][0];
            this->value[0][1] = matrix.value[0][1];
            this->value[0][2] = matrix.value[0][2];
            this->value[0][3] = matrix.value[0][3];
            this->value[1][0] = matrix.value[1][0];
            this->value[1][1] = matrix.value[1][1];
            this->value[1][2] = matrix.value[1][2];
            this->value[1][3] = matrix.value[1][3];
            this->value[2][0] = matrix.value[2][0];
            this->value[2][1] = matrix.value[2][1];
            this->value[2][2] = matrix.value[2][2];
            this->value[2][3] = matrix.value[2][3];
            this->value[3][0] = matrix.value[3][0];
            this->value[3][1] = matrix.value[3][1];
            this->value[3][2] = matrix.value[3][2];
            this->value[3][3] = matrix.value[3][3];
            return out;
        }
        */
    };

    struct FVertexIndiceWeight {
        FVertexIndiceWeight(){};
        unsigned int v;
        FDouble weight;
    };

    struct FVertexIndiceGroupWeight {
        FVertexIndiceGroupWeight(){};
        std::string name;
        std::vector<unsigned int> v;
        FDouble weight;
    };

    // Arrays
    typedef std::vector<FBool> FBoolArray;
    typedef std::vector<FInt> FIntArray;
    typedef std::vector<FUInt> FUIntArray;
    typedef std::vector<FReal> FRealArray;
    typedef std::vector<FString> FStringArray;
    typedef std::vector<FVertex3D> FVertex3DArray;
    typedef std::vector<FVertex3D> FVertex3DArray;
    typedef std::vector<FColorRGB> FColorRGBArray;
    typedef std::vector<FColorRGBA> FColorRGBAArray;
    typedef std::vector<FTextureCoord> FTextureCoordArray;
    typedef std::vector<FNormal3D> FNormal3DArray;
    typedef std::vector<FFace> FFaceArray;
    typedef std::vector<FVertexIndiceWeight> FVertexIndiceWeightArray;
    typedef std::vector<FVertexIndiceGroupWeight> FVertexIndiceGroupWeightArray;

    /*
    // Curves
    struct CurveProps
    {
        CurveProps(){};
        enum Type { Line, Quad, Cubic, Arc, Svg, CatmullRom };
    };
    */

    namespace curve {

        enum Type {
            Line,
            Bezier,
            Quadtratic,
            Arc,
            Svg
        };

    }

    struct FCurvePoint2D //: public CurveProps
    {
        FCurvePoint2D(){};
        curve::Type type;
        FPoint2D point;
        FPoint2D c1; // control point 1
        FPoint2D c2; // control point 1
    };

    struct FCurvePoint3D //: public CurveProps
    {
        FCurvePoint3D(){};
        curve::Type type;
        FPoint3D point;
        FPoint3D c1; // control point 1
        FPoint3D c2; // control point 1
    };

    typedef std::vector<FCurvePoint2D> FCurvePoint2DArray;
    typedef std::vector<FCurvePoint3D> FCurvePoint3DArray;

    // Keys
    struct FKey
    {
        FKey(
                FFloat _value=0.0,
                FFloat _time=0.0,
                curve::Type _inCurve=curve::Line,
                FPoint2D _inCp=FPoint2D(0,0),
                curve::Type _outCurve=curve::Line,
                FPoint2D _outCp=FPoint2D(0,0)
            ) :
            value(_value),
            time(_time),
            incurve(_inCurve),
            incp(_inCp),
            outcurve(_outCurve),
            outcp(_outCp)
        {};
        FFloat value;
        FFloat time;
        curve::Type incurve; // in curve type
        curve::Type outcurve; // out curve type
        FPoint2D incp; // in control point
        FPoint2D outcp; // out control point
    };

    typedef std::vector<FKey> FKeyArray;


    struct FStatus
    {
        FBool pass;
        FString message;        
    };

    
    // Layer Properties
    struct FLayer
    {
        FLayer(std::string _n="", FColorRGB _c=FColorRGB(), bool _v=true, bool _l=true) : name(_n),color(_c),visible(_v),locked(_l) {};
        std::string name;
        FColorRGB color;
        bool visible;
        bool locked;
    };


    // Time
    struct FTime
    {
        double time; // time in milliseconds
        double fps; // frames per second
    };

    struct FSmpte
    {
        int hour;
        int minute;
        int second;
        int frame;
    };


    // Mesh Components

    typedef struct {
        FInt s;
        FFaceArray f;
    } FSmoothingGroup;

    typedef struct {
        FString g;
        FString usemtl;
        std::vector<FSmoothingGroup> sg;
    } FFaceGroup;

    struct FMesh
    {
        enum Type { TRI, QUAD, VARY } type;
        FVertex3DArray v;
        FTextureCoordArray st;
        FVertex3DArray vn;
        FFaceArray f;
   
        inline void print() {
            for(auto vertex : v){
                std::cout << "x:" << vertex.x << " y:" << vertex.y << " z:" << vertex.z << std::endl;
            }
        }

        inline void apply_matrix(const FMatrix4x4& matrix){
            for(unsigned int i=0; i < v.size(); i++){
                // scale
                v[i].x = v[i].x * matrix.value[0][0];
                v[i].y = v[i].y * matrix.value[1][1];
                v[i].z = v[i].z * matrix.value[2][2];
                // transform
                v[i].x += matrix.value[0][3];
                v[i].y += matrix.value[1][3];
                v[i].z += matrix.value[2][3];
            }
        }

        inline void add_face(const FFace face) { f.push_back(face); };
        inline std::vector<int> verts_per_face() { std::vector<int> fc; for(auto face : f){ fc.push_back(face.size()); } return fc; }; 
        inline std::vector<int> vert_indices_per_face() { std::vector<int> vi; for(auto face : f){ for(auto fp : face){ vi.push_back(fp.v); } } return vi; }; 
        inline void assign_v(const FVertex3DArray& _v) { v.assign(_v.begin(),_v.end()); };
        inline void assign_st(const FTextureCoordArray& _st) { st.assign(_st.begin(),_st.end()); };
        inline void assign_vn(const FVertex3DArray& _vn) { vn.assign(_vn.begin(),_vn.end()); };
        inline void assign_f(const FFaceArray& _f) { f.assign(_f.begin(),_f.end()); };

        // If you are going to get tri data, this will tell you how many tri faces will be given
        inline uint32_t tri_face_count() {
            uint32_t count=0;
            for(auto face : f) {
                if(face.size() > 3)
                    count += face.size() - 2;
                else
                    count++;
            }
        };

        // does the mesh only have triangles
        inline bool is_tri_mesh() {
            for(auto face : f) {
                if(face.size() != 3)
                    return false;
            }
            return true;
        };

        // remove all the vertex, normals, tex coords and faces from the mesh
        inline void clear() { v.clear(); st.clear(); vn.clear(); };

        // cut the face along two edges
        inline bool split_face(const uint face, const uint v1, const uint v2) {
            // verify the face and edges
            if(face >= f.size()) {
                if(v1 >= f.at(face).size() || v2 >= f.at(face).size() || v1==v2 || v1==v2+1 || v1==v2-1)
                    return false;
            }
            
            // get the id of the fp of the face
            FFace f1;
            FFace f2;
            bool sf=false; // true if on face two
            std::for_each(f.at(face).begin(), f.at(face).end(), [&v1,&v2,&f1,&f2,&sf](FFacePoint fp){
                (!sf) ? f1.push_back(fp) : f2.push_back(fp);
                if(fp.v==v1 || fp.v==v2) {
                    if(!sf) {
                        f2.push_back(fp);        
                        sf=true;
                    } else {
                        f1.push_back(fp);
                        sf=false;
                    } 
                }
            } );
            
            // replace the old face and add the new one right after it.
            std::vector<FFace> sface;
            sface.push_back(f1);
            sface.push_back(f2);
            auto it = f.begin();
            // set the iterator to the point where the faces need to be inserted
            it=it+face; // set the iterator to the face that needs to be replaced
            // remove the old face
            f.erase(it);
            // add the two new faces in it's place
            f.insert(it,sface.begin(),sface.end());

            return true;
        };
    };

    typedef std::vector<FMesh*> FMeshArray;

    // SceneGraph Types

    struct FNode;

    struct FConnection;

    typedef struct {} DataObject;

    typedef struct {} FAttributeArray;

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, FNode, FConnection> FSceneGraph;

    typedef FSceneGraph::vertex_descriptor FNodeDescriptor;

    typedef std::pair<FSceneGraph::edge_descriptor, bool> FFieldConnection;

    struct PluginNodeFields;

};

#include "draw.hpp"

namespace feather
{        

    struct FNode
    {
        FNode(feather::node::Type t=feather::node::Empty) : type(t) {};
        int uid; // unique id number
        int node; // node type enum
        field::Fields fields; // this holds the field data
        draw::DrawItems items; // holds descriptions of how to draw the node
        std::string name;
        feather::node::Type type; // this is the node group type
        int layer; // what layer is the node stored in
        //DataObject* parent; // ??still used??
        //FAttributeArray* attrs; // ??still used??
    };

    typedef std::vector<FNode*> FNodeArray;

    struct FConnection
    {
        FConnection() {};
        field::Type t1; // source type
        FNodeDescriptor n1; // source node 
        int f1; // source field
        field::Type t2; // target type
        FNodeDescriptor n2; // target node 
        int f2; // target field
        field::FieldBase* sfield; // source field
        field::FieldBase* tfield; // target field
        //field::Connection::Type conn_type;
        //FField* pfield; // parent field
        //FNode* pnode; // parent node
        //int field;
    };

} // namespace feather

#endif
