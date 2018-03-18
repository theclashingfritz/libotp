#pragma once

#include "util.h"
#include "NametagFloat3d.h"

class NametagFloat2d : public virtual NametagFloat3d {
    PUBLISHED:
        NametagFloat2d();
        ~NametagFloat2d();
    
    TYPE_HANDLE(NametagFloat2d, NametagFloat3d);
};