#pragma once

#include "util.h"

#include <stdlib.h> 
#include <math.h>

#include <Python.h>

#include <typedObject.h>
#include <pandabase.h>
#include <nodePath.h>

NotifyCategoryDecl(CPyObjectHandler, EXPCL_LIBOTP, EXPTP_LIBOTP);

class CPyObjectHandler;

class EXPCL_LIBOTP CPyObjectHandler : public virtual TypedObject {
    
    PUBLISHED:
        enum {
            REQUIRED_OBJ = 7,
            NO_DELETE_OBJ = 6,
            GUI_OBJ = 5,
            HIGH_PRIORITY_OBJ = 4,
            MEDIUM_PRIORITY_OBJ = 3,
            LOW_PRIORITY_OBJ = 2,
            EFFECT_OBJ = 1
        };
        
        CPyObjectHandler();
        ~CPyObjectHandler();  
        
        void add_object(std::string object_name, PyObject *obj, unsigned int object_importance = MEDIUM_PRIORITY_OBJ);
        void remove_object(std::string object_name);
        void set_object_count_limit(unsigned int limit);
        void clear_list();
        
        unsigned int get_object_count_limit();
        
        PyObject *get_py_object_dictonary();
        
        static CPyObjectHandler *get_global_ptr();
        
    private:
        pmap<std::string, std::pair<unsigned int, PyObject *>> object_map;
        
        unsigned int object_count_limit;
        
        static CPyObjectHandler *_global_ptr;
    
    TYPE_HANDLE(CPyObjectHandler, TypedObject);
};