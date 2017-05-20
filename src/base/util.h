#pragma once

#include "config_libotp.h"

#include <stdint.h>
#include <pandabase.h>
#include <nodepath.h>
#include <luse.h>

#define EMPTY_NODEPATH NodePath()
#define PT_EMPTY_NODEPATH &NodePath()

#define CLICKSTATE_NORMAL    0
#define CLICKSTATE_CLICK     1
#define CLICKSTATE_HOVER     2
#define CLICKSTATE_DISABLED  3

#define ROL(x, y) __asm{rol x, y}
#define ROR(x, y) __asm{ror x, y}

// Handy typedefs.
typedef LPoint3f LVertexf;
typedef LVector3f LNormalf;
typedef LPoint2f LTexCoordf;
typedef LPoint3f LTexCoord3f;
typedef LVecBase4f LColorf;
typedef LVecBase3f LRGBColorf;
typedef LPoint3d LVertexd;
typedef LVector3d LNormald;
typedef LPoint2d LTexCoordd;
typedef LPoint3d LTexCoord3d;
typedef LVecBase4d LColord;
typedef LVecBase3d LRGBColord;

typedef LVertexf LVertex;
typedef LNormalf LNormal;
typedef LTexCoordf LTexCoord;
typedef LTexCoord3f LTexCoord3;
typedef LColorf LColor;
typedef LRGBColorf LRGBColor;

//Define the Python Typedefs. 
typedef LMatrix4f Mat4;
typedef LMatrix3f Mat3;
typedef LVecBase4f VBase4;
typedef LVector4f Vec4;
typedef LPoint4f Point4;
typedef LVecBase3f VBase3;
typedef LVector3f Vec3;
typedef LPoint3f Point3;
typedef LVecBase2f VBase2;
typedef LVector2f Vec2;
typedef LPoint2f Point2;
typedef LQuaternionf Quat;

//Define these older Panda3D types for our new Panda3D
typedef unsigned short ushort;
typedef PointerToArray<ushort> PTA_ushort;
typedef ConstPointerToArray<ushort> CPTA_ushort;
typedef LVector3f Normalf;
typedef PointerToArray<Normalf> PTA_Normalf;
typedef ConstPointerToArray<Normalf> CPTA_Normalf;
typedef LPoint3f Vertexf;
typedef PointerToArray<Vertexf> PTA_Vertexf;
typedef ConstPointerToArray<Vertexf> CPTA_Vertexf;
typedef LPoint2f TexCoordf;
typedef PointerToArray<TexCoordf> PTA_TexCoordf;
typedef ConstPointerToArray<TexCoordf> CPTA_TexCoordf;
typedef LVecBase4f Colorf;
typedef PointerToArray<Colorf> PTA_Colorf;
typedef ConstPointerToArray<Colorf> CPTA_Colorf;

// PROPERTY MACRO (TYPE, NAME)
// This is a helper for classes
// Declares protected m_NAME variable of type TYPE
// and generates PUBLISHED INLINE setter/getter
// N.B. Anything below a PROPERTY declaration will
// default to protected
#define PROPERTY(TYPE, NAME) PUBLISHED: \
                                INLINE void set_##NAME (TYPE value) { m_##NAME = value; }; \
                                INLINE TYPE get_##NAME () { return m_##NAME ; }; \
                            protected: \
                                TYPE m_##NAME ;
                                
// PROPERTY_STRING is like above, but it uses const std::string& for setter
#define PROPERTY_STRING(NAME) PUBLISHED: \
                                INLINE void set_##NAME (const std::string& value) { m_##NAME = value; }; \
                                INLINE std::string get_##NAME () { return m_##NAME ; }; \
                            protected: \
                                std::string m_##NAME ;

// TYPE_HANDLE MACRO
// This is a helper for classes
// Reduces litter in class declaration
#define TYPE_HANDLE(NAME, PARENT) public: \
                                    static TypeHandle get_class_type() { return _type_handle; }; \
                                    virtual TypeHandle force_init_type() { init_type(); return get_class_type(); }; \
                                    virtual TypeHandle get_type() const { return get_class_type(); }; \
                                    static void init_type() { PARENT :: init_type(); register_type(_type_handle, #NAME, PARENT :: get_class_type()); }; \
                                  private: static TypeHandle _type_handle;
