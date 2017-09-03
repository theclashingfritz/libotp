#include "CImpulse.h"
#include "CMover.h"

NotifyCategoryDef(CImpulse, "");

TypeHandle CImpulse::_type_handle;

CImpulse::CImpulse() {

}

CImpulse::~CImpulse() {
    m_mover = NULL;
}

void CImpulse::process(float dt) {
    CImpulse_cat.debug() << "process(" << dt << ")" << std::endl;
    return;
}

void CImpulse::set_mover(CMover* mover) {
    CImpulse_cat.debug() << "set_mover(CMover mover)" << std::endl;
    m_mover = mover;
    m_nodepath = m_mover->get_node_path();
    m_vec_type = m_mover->m_vec_type;
}

void CImpulse::set_node_path(NodePath path) {
    CImpulse_cat.debug() << "set_node_path(NodePath path)" << std::endl;
    m_nodepath = path;
}

void CImpulse::clear_mover() {
    CImpulse_cat.debug() << "clear_mover()" << std::endl;
    m_mover = NULL;
}

void CImpulse::clear_mover(CMover* mover) {
    CImpulse_cat.debug() << "clear_mover(CMover* mover)" << std::endl;
    if (mover == m_mover) {
        m_mover = NULL;
    }
}

NodePath CImpulse::get_node_path() {
    CImpulse_cat.debug() << "get_node_path()" << std::endl;
    return m_nodepath;
}

CMover* CImpulse::get_mover() {
    CImpulse_cat.debug() << "get_mover()" << std::endl;
    return m_mover;
}

bool CImpulse::is_cpp() {
    CImpulse_cat.debug() << "is_cpp()" << std::endl;
    return 1;
}

