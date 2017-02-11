#include "CMover.h"

TypeHandle CMover::_type_handle;

CMover::CMover(NodePath m_nodepath): m_fwdSpeed(0.0), m_rotSpeed(0.0) {

}

CMover::~CMover() {
    
}

void CMover::set_fwd_speed(float speed) {
    m_fwdSpeed = speed;
}

void CMover::set_rot_speed(float speed) {
    m_rotSpeed = speed;
}

void CMover::set_dt(float dt) {
    m_dt = dt;
}

void CMover::reset_dt() {
    m_dt = 0.0;
}

void CMover::add_force(LVecBase3f force){
    return;
}

void CMover::add_rot_force(LVecBase3f force){
    return;
}

void CMover::add_shove(LVecBase3f shove){
    return;
}

void CMover::add_rot_shove(LVecBase3f shove){
    return;
}

void CMover::set_node_path(NodePath np) {
    m_nodepath = np;
}

float CMover::get_fwd_speed() {
    return m_fwdSpeed;
}

float CMover::get_rot_speed() {
    return m_rotSpeed;
}

float CMover::get_dt() {
    return m_dt;
}

NodePath CMover::get_node_path() {
    return m_nodepath;
}
