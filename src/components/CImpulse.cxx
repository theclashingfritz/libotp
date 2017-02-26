#include "CImpulse.h"
#include "CMover.h"

TypeHandle CImpulse::_type_handle;

CImpulse::CImpulse() {

}

CImpulse::~CImpulse() {
    m_mover = NULL;
}

void CImpulse::process(float dt) {
    return;
}

void CImpulse::set_mover(CMover* mover) {
    m_mover = mover;
    m_nodepath = m_mover->get_node_path();
    m_vec_type = m_mover->m_vec_type;
}

void CImpulse::clear_mover() {
    m_mover = NULL;
}

void CImpulse::clear_mover(CMover* mover) {
    if (mover == m_mover) {
        m_mover = NULL;
    }
}

CMover* CImpulse::get_mover() {
    return m_mover;
}

bool CImpulse::is_cpp() {
    return 1;
}

