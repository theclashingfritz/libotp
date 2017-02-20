#pragma once

#include "util.h"
#include "CMID.h"
#include <string>
#include <pvector.h>
#include <typedReferenceCount.h>
#include <pandabase.h>
#include <nodePath.h>
#include "CImpulse.h"

class EXPCL_LIBOTP CMover : public TypedReferenceCount {
    
    PUBLISHED:
        CMover(NodePath m_nodepath);
        CMover(NodePath m_nodepath, float m_fwdSpeed);
        CMover(NodePath m_nodepath, float m_fwdSpeed, float m_rotSpeed);
        ~CMover();
        
        void set_fwd_speed(float speed);
        void set_rot_speed(float speed);
        void set_node_path(NodePath np);
        void set_dt(float dt);
        void reset_dt();
        void add_c_impulse(string name, CImpulse impulse);
        void remove_c_impulse(string name);
        void add_shove(LVecBase3f shove);
        void add_rot_shove(LVecBase3f shove);
        void add_force(LVecBase3f force);
        void add_rot_force(LVecBase3f force);
        float get_fwd_speed();
        float get_rot_speed();
        float get_dt();
        NodePath get_node_path();
        
    private:
        float m_fwdSpeed;
        float m_rotSpeed;
        float m_dt;
        
        pvector<CImpulse> m_c_impulses;
        
        LVecBase3f shove;
        LVecBase3f force;
        
        NodePath m_nodepath;
        
    TYPE_HANDLE(CMover, TypedObject);
};