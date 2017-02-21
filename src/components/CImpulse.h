#pragma once

#include "util.h"
#include <typedObject.h>
#include <nodePath.h>
#include <pandabase.h>

//#include "CMover.h"

class CMover;
class CImpulse;

class EXPCL_LIBOTP CImpulse : public TypedObject {
    
    PUBLISHED:
        CImpulse();
        ~CImpulse();
        void process(float dt);
        void set_mover(CMover* mover);
        void clear_mover();
        void clear_mover(CMover* mover);
        CMover* get_mover();
        bool is_cpp();
        
    private:
        CMover* m_mover;
        NodePath m_nodepath;
        
    TYPE_HANDLE(CImpulse, TypedObject);
};