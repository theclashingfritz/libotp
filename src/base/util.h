#pragma once

#include "config_libmovement.h"
#include <pandabase.h>
#include <luse.h>

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