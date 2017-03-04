#include "NametagGroup.h"

TypeHandle NametagGroup::_type_handle;

NametagGroup::NametagGroup() {
}

NametagGroup::~NametagGroup() {
}

void NametagGroup::set_avatar(NodePath* node) {
    m_avatar_nodepath = node;
}

void NametagGroup::set_active(bool active) {
    m_active = active;
}

void NametagGroup::set_name_wordwrap(float wordwrap) {
    
}

void NametagGroup::clear_shadow() {

} 

NodePath* NametagGroup::get_avatar() {
    return m_avatar_nodepath;
}

bool NametagGroup::is_active() {
    return m_active;
}