#include "NametagGroup.h"

TypeHandle NametagGroup::_type_handle;

NametagGroup::NametagGroup() {
}

NametagGroup::~NametagGroup() {
}

void NametagGroup::set_avatar(NodePath node) {
    m_avatar_nodepath = node;
}

NodePath NametagGroup::get_avatar() {
    return m_avatar_nodepath;
}