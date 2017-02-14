#pragma once

#include "util.h"
#include <luse.h>
#include <nodepath.h>
#include <typedObject.h>
#include <pandabase.h>

class NametagGroup;

class EXPCL_LIBOTP NametagGroup : public TypedObject {
    PUBLISHED:
        NametagGroup();
        ~NametagGroup();
        void set_avatar(NodePath node);
        NodePath get_avatar();
        
    private:
        NodePath m_avatar_nodepath; 

    TYPE_HANDLE(NametagGroup, TypedObject);
};