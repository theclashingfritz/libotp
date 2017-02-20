#pragma once

#include "util.h"

#include "Nametag.h"

#include <pandabase.h>

class Nametag3d;

class EXPCL_LIBOTP Nametag3d : public Nametag {
    
    PUBLISHED:
        Nametag3d();
        ~Nametag3d();
    
    TYPE_HANDLE(Nametag3d, TypedObject);
};