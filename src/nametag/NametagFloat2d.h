#pragma once

#include "util.h"
#include "Nametag2d.h"

class NametagFloat2d : public Nametag2d {
    PUBLISHED:
        NametagFloat2d();
        ~NametagFloat2d();
        
    public:
         virtual void billboard();
    
    TYPE_HANDLE(NametagFloat2d, TypedObject);
};