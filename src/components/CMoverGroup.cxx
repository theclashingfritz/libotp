#include "CMoverGroup.h"

TypeHandle CMoverGroup::_type_handle;


CMoverGroup::CMoverGroup() {
    
}

CMoverGroup::~CMoverGroup() {
    
}

void CMoverGroup::add_c_mover(string name, CMover* mover) {
    libotp_cat.debug() << "Adding CMover to our mover map!" << std::endl;
    m_c_movers[name] = mover;
}

void CMoverGroup::remove_c_mover(string name) {
    if (m_c_movers.find(name) == m_c_movers.end()) {
        libotp_cat.debug() << "CMover was not found in mover map! Returning..." << std::endl;
        return;
    } else {
        libotp_cat.debug() << "Removing CMover from mover map and destorying CMover!" << std::endl;
        pmap<std::string, CMover*>::iterator it;
        it = m_c_movers.find(name);
        if (it != m_c_movers.end()) {
            // Found it - Delete it
            delete it->second;
            m_c_movers.erase(it);
        }
    }
}


void CMoverGroup::process_c_impulses_and_integrate() {
    libotp_cat.debug() << "Processing all CImpulses in our CMover Groups!" << std::endl;
    for each(pair<string, CMover*> x in m_c_movers) {
        x.second->process_c_impulses(get_dt());
    }
}

void CMoverGroup::set_dt(float dt) {
    m_dt = dt;
}

void CMoverGroup::set_dt() {
    m_dt = 0.0;
}

void CMoverGroup::reset_dt() {
    m_dt = 0.0;
}

float CMoverGroup::get_dt() {
    return m_dt;
}