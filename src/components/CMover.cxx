#include "CMover.h"

#ifdef HAVE_THEMDIA
#include <ThemidaSDK.h>
#endif

NotifyCategoryDef(CMover, "");

TypeHandle CMover::_type_handle;

CMover::CMover(NodePath nodepath) {
    if (!nodepath.is_empty()) {
        m_nodepath = nodepath;
    }
    m_fwd_speed = 1.0;
    m_rot_speed = 1.0;
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

CMover::CMover(const CMover& mover) {
    if (!mover.m_nodepath.is_empty()) {
        m_nodepath = mover.m_nodepath;
    }
    m_fwd_speed = mover.m_fwd_speed;
    m_rot_speed = mover.m_rot_speed;
    m_vec_type = mover.m_vec_type;
    
    _dt = mover._dt;
    _dt_clock = mover._dt_clock;
    
    movement = LVector3f(mover.movement);
    rotation = LVector3f(mover.rotation);
    rot_force = LVector3f(mover.rot_force);
    push_force = LVector3f(mover.push_force);
}

void CMover::operator =(const CMover& mover) {
    if (!mover.m_nodepath.is_empty()) {
        m_nodepath = mover.m_nodepath;
    }
    m_fwd_speed = mover.m_fwd_speed;
    m_rot_speed = mover.m_rot_speed;
    m_vec_type = mover.m_vec_type;
    
    _dt = mover._dt;
    _dt_clock = mover._dt_clock;
    
    movement = LVector3f(mover.movement);
    rotation = LVector3f(mover.rotation);
    rot_force = LVector3f(mover.rot_force);
    push_force = LVector3f(mover.push_force);
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

void CMover::add_c_impulse(string name, CImpulse *impulse) {
    CMover_cat.debug() << "add_c_impulse('" << name << "', CImpulse impulse)" << std::endl;
    if (impulse == nullptr || impulse == NULL) {
        return;
    }
    m_c_impulses[name] = impulse;
    m_c_impulses[name]->set_mover(this);
}

void CMover::remove_c_impulse(string name) {
#ifdef HAVE_THEMDIA
    ENCODE_START
#endif
    CMover_cat.debug() << "remove_c_impulse(" << name << ")" << std::endl;
    if (m_c_impulses.find(name) == m_c_impulses.end()) {
        CMover_cat.spam() << "CImpulse '" << name << "' was not found in impulse map! Returning..." << std::endl;
        return;
    } else {
        pmap<std::string, CImpulse *>::iterator it;
        it = m_c_impulses.find(name);
        if (it != m_c_impulses.end()) {
            // Found it? - Delete it!
            CMover_cat.spam() << "Removing CImpulse '" << name << "' from map!" << std::endl;
            m_c_impulses[name]->clear_mover(this);
            m_c_impulses.erase(it);
        } else {
            CMover_cat.spam() << "CImpulse '" << name << "' was not found in impulse map! Returning..." << std::endl;
            return;
        }
    }
#ifdef HAVE_THEMDIA
    ENCODE_END
#endif
}

void CMover::process_c_impulses(float dt) {
    CMover_cat.debug() << "process_c_impulses(" << dt << ")" << std::endl;
    _dt = dt;
    if (_dt == -1.0) {
        ClockObject *co = ClockObject::get_global_clock();
        float clock_dt = co->get_dt();
        _dt = clock_dt - _dt_clock;
        _dt_clock = clock_dt;
    }
    for each(pair<string, CImpulse *> x in m_c_impulses) {
        x.second->process(_dt);
    }
}

void CMover::process_c_impulses() {
    CMover_cat.debug() << "process_c_impulses()" << std::endl;
    if (_dt == -1.0) {
        ClockObject *co = ClockObject::get_global_clock();
        float clock_dt = co->get_dt();
        _dt = clock_dt - _dt_clock;
        _dt_clock = clock_dt;
    }
    for each(pair<string, CImpulse *> x in m_c_impulses) {
        x.second->process(_dt);
    }
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
    // Can't believe it was this simple. All the force crud goes unused. 
    movement = movement * (_dt * _dt);
    CMover_cat.spam() << "movement = LVector3f(" << movement[0] << ", " << movement[1] << ", " << movement[2] << ")" << std::endl;
    m_nodepath.set_fluid_pos(m_nodepath, movement);
    rotation = rotation * _dt;
    CMover_cat.spam() << "rotation = LVector3f(" << rotation[0] << ", " << rotation[1] << ", " << rotation[2] << ")" << std::endl;
    m_nodepath.set_hpr(m_nodepath, rotation);
    movement = LVector3f(0.0);
    rotation = LVector3f(0.0);
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

CImpulse *CMover::get_c_impulse(string name) {
    CMover_cat.debug() << "get_c_impulse(" << name << ")" << std::endl;
    if (m_c_impulses.find(name) == m_c_impulses.end()) {
        CMover_cat.spam() << "CImpulse '" << name << "' was not found in impulse map! Returning a nullptr!" << std::endl;
        return nullptr;
    } else {
        return m_c_impulses[name];
    }
}

NodePath CMover::get_node_path() {
    CMover_cat.debug() << "get_node_path()" << std::endl;
    return m_nodepath;
}
