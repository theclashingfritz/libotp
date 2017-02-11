#include "CMover.h"

TypeHandle CMover::_type_handle;

CMover::CMover(NodePath m_nodepath): m_fwdSpeed(0), m_rotSpeed(0) {

}

CMover::~CMover() {
    
}

void CMover::set_fwd_speed(int32_t speed) {
    m_fwdSpeed = speed;
}

void CMover::set_rot_speed(int32_t speed) {
    m_rotSpeed = speed;
}

void CMover::set_node_path(NodePath np) {
    m_nodepath = np;
}

int32_t CMover::get_fwd_speed() {
    return m_fwdSpeed;
}

int32_t CMover::get_rot_speed() {
    return m_rotSpeed;
}

NodePath CMover::get_node_path() {
    return m_nodepath;
}