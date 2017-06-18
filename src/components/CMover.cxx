#include "CMover.h"

NotifyCategoryDef(CMover, "");

TypeHandle CMover::_type_handle;

CMover::CMover(NodePath nodepath) {
    if (!nodepath.is_empty()) {
        m_nodepath = nodepath;
    }
    m_fwd_speed = 1.0;
    m_rot_speed = 1.0;
    m_distance = 0.0;
    m_vec_type = Vec3(0, 0, 0);
    
    _dt = 1.0;
    _dt_clock = ClockObject::get_global_clock()->get_dt();
    
    movement = LVector3f(0.0);
    rotation = LVector3f(0.0);
    rot_force = LVector3f(0.0);
    push_force = LVector3f(0.0);

}

CMover::CMover(NodePath nodepath, float m_fwd_speed) {
    if (!nodepath.is_empty()) {
        m_nodepath = nodepath;
    }
    m_rot_speed = 1.0;
    m_distance = 0.0;
    m_vec_type = Vec3(0, 0, 0);
    
    _dt = 1.0;
    _dt_clock = ClockObject::get_global_clock()->get_dt();
    
    movement = LVector3f(0.0);
    rotation = LVector3f(0.0);
    rot_force = LVector3f(0.0);
    push_force = LVector3f(0.0);
}

CMover::CMover(NodePath nodepath, float m_fwd_speed, float m_rot_speed) {
    if (!nodepath.is_empty()) {
        m_nodepath = nodepath;
    }
    m_distance = 0.0;
    m_vec_type = Vec3(0, 0, 0);
    
    _dt = 1.0;
    _dt_clock = ClockObject::get_global_clock()->get_dt();
    
    movement = LVector3f(0.0);
    rotation = LVector3f(0.0);
    rot_force = LVector3f(0.0);
    push_force = LVector3f(0.0);
}

CMover::~CMover() {
    
}

void CMover::set_fwd_speed(float speed) {
    CMover_cat.debug() << "set_fwd_speed(" << speed << ")" << std::endl;
    m_fwd_speed = speed;
}

void CMover::set_rot_speed(float speed) {
    CMover_cat.debug() << "set_rot_speed(" << speed << ")" << std::endl;
    m_rot_speed = speed;
}

void CMover::set_dt(double dt) {
    CMover_cat.debug() << "set_dt(" << dt << ")" << std::endl;
    _dt = dt;
    if (_dt == -1.0) {
        ClockObject *co = ClockObject::get_global_clock();
        float clock_dt = co->get_dt();
        _dt = clock_dt - _dt_clock;
        _dt_clock = clock_dt;
    }
}

void CMover::reset_dt() {
    CMover_cat.debug() << "reset_dt()" << std::endl;
    ClockObject *co = ClockObject::get_global_clock();
    _dt_clock = co->get_dt();
}

void CMover::add_c_impulse(string name, CImpulse impulse) {
    CMover_cat.debug() << "add_c_impulse(" << name << " CImpulse impulse)" << std::endl;
    CMover_cat.spam() << "Adding CImpulse '" << name << "' to impulse map and setting it's mover to our current mover!" << std::endl;
    m_c_impulses[name] = impulse;
    m_c_impulses[name].set_mover(this);
}

void CMover::remove_c_impulse(string name) {
    CMover_cat.debug() << "remove_c_impulse(" << name << ")" << std::endl;
    if (m_c_impulses.find(name) == m_c_impulses.end()) {
        CMover_cat.spam() << "CImpulse '" << name << "' was not found in impulse map! Returning..." << std::endl;
        return;
    } else {
        pmap<std::string, CImpulse>::iterator it;
        it = m_c_impulses.find(name);
        if (it != m_c_impulses.end()) {
            // Found it? - Delete it!
            CMover_cat.spam() << "Removing CImpulse '" << name << "' from map and destorying CImpulse!" << std::endl;
            m_c_impulses[name].clear_mover(this);
            m_c_impulses.erase(it);
        } else {
            CMover_cat.spam() << "CImpulse '" << name << "' was not found in impulse map! Returning..." << std::endl;
            return;
        }
    }
}

void CMover::process_c_impulses(float dt) {
    CMover_cat.debug() << "process_c_impulses(" << dt << ")" << std::endl;
    CMover_cat.spam() << "Processing CImpulses!" << std::endl;
    if (_dt == -1.0) {
        ClockObject *co = ClockObject::get_global_clock();
        float clock_dt = co->get_dt();
        _dt = clock_dt - _dt_clock;
        _dt_clock = clock_dt;
    }
    for each(pair<string, CImpulse> x in m_c_impulses) {
        x.second.process(dt);
    }
}

void CMover::process_c_impulses() {
    CMover_cat.debug() << "process_c_impulses()" << std::endl;
    CMover_cat.spam() << "Processing CImpulses!" << std::endl;
    if (_dt == -1.0) {
        ClockObject *co = ClockObject::get_global_clock();
        float clock_dt = co->get_dt();
        _dt = clock_dt - _dt_clock;
        _dt_clock = clock_dt;
    }
    for each(pair<string, CImpulse> x in m_c_impulses) {
        x.second.process(get_dt());
    }
}

void CMover::add_force(Vec3 force) {
    CMover_cat.debug() << "add_force(Vec3(" << force[0] << ", " << force[1] << ", " << force[2] << "))" << std::endl;
    push_force[0] = push_force[0] + force[0];
    push_force[1] = force[1] + push_force[1];
    push_force[2] = force[2] + push_force[2];
}

void CMover::add_rot_force(Vec3 force) {
   CMover_cat.debug() << "add_rot_force(Vec3(" << force[0] << ", " << force[1] << ", " << force[2] << "))" << std::endl;
    rot_force[0] = rot_force[0] + force[0];
    rot_force[1] = force[1] + rot_force[1];
    rot_force[2] = force[2] + rot_force[2];
}

void CMover::add_shove(Vec3 shove) {
    CMover_cat.debug() << "add_shove(Vec3(" << shove[0] << ", " << shove[1] << ", " << shove[2] << "))" << std::endl;
    movement[0] = movement[0] + shove[0];
    movement[1] = shove[1] + movement[1];
    movement[2] = shove[2] + movement[2];
}

void CMover::add_rot_shove(Vec3 shove) {
    CMover_cat.debug() << "add_rot_shove(Vec3(" << shove[0] << ", " << shove[1] << ", " << shove[2] << "))" << std::endl;
    rotation[0] = rotation[0] + shove[0];
    rotation[1] = shove[1] + rotation[1];
    rotation[2] = shove[2] + rotation[2];
}

void CMover::set_node_path(NodePath np) {
    CMover_cat.debug() << "set_node_path(NodePath np)" << std::endl;
    m_nodepath = np;
}

void CMover::integrate() {
    CMover_cat.debug() << "CMover.integrate() [WARNING: This function may be incomplete/broken.]" << std::endl;
    if (m_nodepath.is_empty()) {
        CMover_cat.debug() << "CMover.integrate() was called while the nodepath is empty!" << std::endl;
        return;
    }
    LVector3f i_force; // ebp@1 MAPDST
    LVector3f i_rot_force; // ebx@1 MAPDST
    LVector3f v4; // eax@1 MAPDST
    float v8; // ST7C_4@1
    LVector3f v9; // ST78_4@1 MAPDST
    LVector3f v10; // eax@1 MAPDST
    LVector3f v15; // ST80_4@1
    LVector3f i_shove; // eax@1
    LVector3f empty;

    i_force = push_force;
    i_rot_force = rot_force;
    empty = LVector3f(0.0);
    v4 = i_rot_force * _dt;
    i_rot_force = empty;
    i_rot_force[0] = v4[0] + empty[0];
    i_rot_force[1] = v4[1] + empty[1];
    i_rot_force[2] = v4[2] + empty[2];
    v8 = _dt * _dt;
    v9 = movement * _dt;
    v10 = i_force * v8;
    i_force = v10 * NametagGlobals::scale_exponent;
    v10 = empty * _dt;
    v10 = v10 + i_force;
    v4 = v10 + v9;
    CMover_cat.spam() << "v4 = LVector3f(" << v4[0] << ", " << v4[1] << ", " << v4[2] << ")" << std::endl;
    m_nodepath.set_fluid_pos(m_nodepath, v4);
    v15 = rotation * _dt;
    v10 = rot_force * v8;
    v9 = v10 * NametagGlobals::scale_exponent;
    v10 = empty * _dt;
    v10 = v10 + v9;
    v4 = v10 + v15;
    CMover_cat.spam() << "v4 = LVector3f(" << v4[0] << ", " << v4[1] << ", " << v4[2] << ")" << std::endl;
    m_nodepath.set_hpr(m_nodepath, v4);
    movement[0] = 0.0;
    movement[1] = 0.0;
    movement[2] = 0.0;
    rotation[0] = 0.0;
    rotation[1] = 0.0;
    rotation[2] = 0.0;

}

float CMover::get_fwd_speed() {
    CMover_cat.debug() << "get_fwd_speed()" << std::endl;
    return m_fwd_speed;
}

float CMover::get_rot_speed() {
    CMover_cat.debug() << "get_rot_speed()" << std::endl;
    return m_rot_speed;
}

double CMover::get_dt() {
    CMover_cat.debug() << "get_dt()" << std::endl;
    return _dt;
}

CImpulse CMover::get_c_impulse(string name) {
    CMover_cat.debug() << "get_c_impulse(" << name << ")" << std::endl;
    if (m_c_impulses.find(name) == m_c_impulses.end()) {
        CMover_cat.spam() << "CImpulse '" << name << "' was not found in impulse map! Returning a fake CImpulse..." << std::endl;
        return CImpulse();
    } else {
        return m_c_impulses[name];
    }
}

NodePath CMover::get_node_path() {
    CMover_cat.debug() << "get_node_path()" << std::endl;
    return m_nodepath;
}
