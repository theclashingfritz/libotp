#pragma once

#include "util.h"
#include <luse.h>
#include <nodepath.h>
#include <namable.h>
#include <typedReferenceCount.h>
#include <pandabase.h>

class NametagGroup;

class EXPCL_LIBOTP NametagGroup : public TypedReferenceCount, Namable {
    PUBLISHED:
        NametagGroup();
        ~NametagGroup();
        void set_avatar(NodePath* node);
        void set_active(bool active);
        void set_name_wordwrap(float wordwrap);
        void clear_shadow();
        NodePath* get_avatar();
        bool is_active();
        
    private:
        NodePath* m_avatar_nodepath; 
        bool m_active;

    TYPE_HANDLE(NametagGroup, TypedObject);
};