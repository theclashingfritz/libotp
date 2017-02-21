#include "CImpulse.h"

TypeHandle CImpulse::_type_handle;

CImpulse::CImpulse() {

}

CImpulse::~CImpulse() {
    
}

void CImpulse::process(float dt) {
    return;
}

void CImpulse::set_mover(CMover* mover) {
    m_mover = mover;
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

