#pragma once

#include "util.h"
#include "CMID.h"
#include <typedObject.h>
#include <pandabase.h>

//#include "CMover.h"

class EXPCL_LIBOTP CImpulse : public TypedObject {
    
    PUBLISHED:
        CImpulse();
        ~CImpulse();
        bool is_cpp();
        void clear_mover(CMover* mover);
        
    private:
        CMover* m_mover;
        
    TYPE_HANDLE(CImpulse, TypedObject);
};