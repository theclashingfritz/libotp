#pragma once

#include "util.h"
#include "Nametag3d.h"

class NametagFloat3d : public Nametag3d {
    PUBLISHED:
        NametagFloat3d();
        ~NametagFloat3d();
        
    public:
        virtual void tick();
    
    TYPE_HANDLE(NametagFloat3d, TypedObject);
};