#pragma once

#include "util.h"
#include "NametagFloat3d.h"

class NametagFloat2d : public NametagFloat3d {
    PUBLISHED:
        NametagFloat2d();
        ~NametagFloat2d();
    
    TYPE_HANDLE(NametagFloat2d, TypedObject);
};