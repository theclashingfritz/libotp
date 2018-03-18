#pragma once

#include "util.h"
#include "Nametag3d.h"

class NametagFloat3d : public virtual Nametag3d {
    PUBLISHED:
        NametagFloat3d();
        NametagFloat3d(const NametagFloat3d &other);
        ~NametagFloat3d();
    
    TYPE_HANDLE(NametagFloat3d, Nametag3d);
};