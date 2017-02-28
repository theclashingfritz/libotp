#include "CMoverGroup.h"

NotifyCategoryDef(CMoverGroup, "");

TypeHandle CMoverGroup::_type_handle;


CMoverGroup::CMoverGroup() {
    
}

CMoverGroup::~CMoverGroup() {
    
}

void CMoverGroup::add_c_mover(string name, CMover* mover) {
    CMoverGroup_cat.debug() << "Adding CMover '" << name << "' to our mover map!" << std::endl;
    m_c_movers[name] = mover;
}

void CMoverGroup::remove_c_mover(string name) {
    if (m_c_movers.find(name) == m_c_movers.end()) {
        CMoverGroup_cat.debug() << "CMover '" << name << "' was not found in mover map! Returning..." << std::endl;
        return;
    } else {
        pmap<std::string, CMover*>::iterator it;
        it = m_c_movers.find(name);
        if (it != m_c_movers.end()) {
            // Found it? - Delete it!
            CMoverGroup_cat.info() << "Removing CMover '" << name << "' from mover map and destorying CMover!" << std::endl;
            delete it->second;
            m_c_movers.erase(it);
        } else {
            CMoverGroup_cat.debug() << "CMover '" << name << "' was not found in mover map! Returning..." << std::endl;
            return;
        }
    }
}

void CMoverGroup::process_c_impulses() {
    CMoverGroup_cat.debug() << "Processing all CImpulses in our CMover Groups!" << std::endl;
    for each(pair<string, CMover*> x in m_c_movers) {
        x.second->process_c_impulses(get_dt());
    }
}

void CMoverGroup::process_c_impulses_and_integrate() {
    CMoverGroup_cat.debug() << "Processing all CImpulses in our CMover Groups! But not integrating as it's deperacted." << std::endl;
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