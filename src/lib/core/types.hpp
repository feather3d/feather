// =====================================================================================
// 
//       Filename:  types.hpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03/08/2012 02:38:31 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Richard Layman (), rlayman2000@yahoo.com
//        Company:  
// 
// =====================================================================================
#ifndef TYPES_HPP
#define TYPES_HPP

#include "deps.hpp"
#include "field.hpp"
#include "node.hpp"

namespace feather
{
    typedef bool FBool;
    typedef int FInt;
    typedef float FFloat;
    typedef double FDouble;
    typedef std::string FString;
    typedef struct{double x; double y;} FPoint2D;
    typedef struct{double x; double y; double z;} FPoint3D;

    struct FVertex3D
    {
        FVertex3D(FFloat _x=0, FFloat _y=0, FFloat _z=0) { x=_x; y=_y; z=_z; };
        FFloat x;
        FFloat y;
        FFloat z;
    };

    typedef struct{double x; double y; double z;} FNormal3D;
    typedef struct{ double x; double y; double z; double w;} FVector3D;
    typedef struct{int r; int g; int b;} FColorRGB;
    typedef struct{int r; int g; int b; int a;} FColorRGBA;

    struct FTransform
    {
        FVertex3D translate;
        FVector3D rotate;
        FVector3D scale;
    };

    struct FTextureCoord
    {
        FTextureCoord(FDouble _s=0, FDouble _t=0) { s=_s; t=_t; };
        FDouble s;
        FDouble t;
    };

    typedef struct{int v; int vt; int vn;} FFacePoint;
    typedef struct{std::vector<FFacePoint> f; } FFace;
    typedef double FMatrix[4][4];

    // Arrays
    typedef std::vector<FBool> FBoolArray;
    typedef std::vector<FInt> FIntArray;
    typedef std::vector<FDouble> FDoubleArray;
    typedef std::vector<FString> FStringArray;
    typedef std::vector<FVertex3D> FVertex3DArray;
    typedef std::vector<FVector3D> FVector3DArray;
    typedef std::vector<FTextureCoord> FTextureCoordArray;
    typedef std::vector<FNormal3D> FNormal3DArray;
    typedef std::vector<FFace> FFaceArray;

    struct FStatus
    {
        FBool pass;
        FString message;        
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
        FVertex3DArray v;
        FTextureCoordArray st;
        FNormal3DArray vn;
        void clear() { v.clear(); st.clear(); vn.clear(); };
    };

    typedef struct {} DataObject;

    typedef struct {} FAttributeArray;

    struct FNode
    {
        FNode(node::Type t=node::Null) : type(t),parent(NULL) {};
        int id;
        node::Type type;
        DataObject* parent;
        FAttributeArray* attrs;
    };

    typedef std::vector<FNode*> FNodeArray;


} // namespace feather

#endif
