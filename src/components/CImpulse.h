#pragma once

#include "util.h"
#include <typedObject.h>
#include <pandabase.h>

class CImpulse;

class EXPCL_LIBOTP CImpulse : public TypedObject {
    PUBLISHED:
        CImpulse();
        ~CImpulse();
        bool is_cpp();
        
    TYPE_HANDLE(CImpulse, TypedObject);
};