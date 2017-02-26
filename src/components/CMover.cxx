#include "CMover.h"

TypeHandle CMover::_type_handle;

CMover::CMover(NodePath m_nodepath) {
    m_fwdSpeed = 1.0;
    m_rotSpeed = 1.0;
    m_vec_type = Vec3(0, 0, 0);
}

CMover::CMover(NodePath m_nodepath, float m_fwdSpeed) {
    m_rotSpeed = 1.0;
    m_vec_type = Vec3(0, 0, 0);
}

CMover::CMover(NodePath m_nodepath, float m_fwdSpeed, float m_rotSpeed) {
    m_vec_type = Vec3(0, 0, 0);
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

void CMover::add_c_impulse(string name, CImpulse impulse) {
    m_c_impulses[name] = impulse;
    m_c_impulses[name].set_mover(this);
}

void CMover::remove_c_impulse(string name) {
    if (m_c_impulses.find(name) == m_c_impulses.end()) {
        return;
    } else {
        m_c_impulses[name].clear_mover(this);
        it = m_c_impulses.find(name);
        m_c_impulses.erase(it);
    }
}

void CMover::add_force(VBase3 force) {
    return;
}

void CMover::add_rot_force(VBase3 force) {
    return;
}

void CMover::add_shove(VBase3 shove) {
    return;
}

void CMover::add_rot_shove(VBase3 shove) {
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
