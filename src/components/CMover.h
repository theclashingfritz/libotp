#pragma once

#include "util.h"
#include <string>
#include <fstream>
#include <istream>
#include <iostream>
#include <notifyCategoryProxy.h>
#include <typedReferenceCount.h>
#include <pandabase.h>
#include <nodePath.h>
#include "CImpulse.h"

NotifyCategoryDecl(CMover, EXPCL_LIBOTP, EXPTP_LIBOTP);

class CMover;

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
        void process_c_impulses(float dt);
        void process_c_impulses();
        void add_shove(VBase3 shove);
        void add_rot_shove(VBase3 shove);
        void add_force(VBase3 force);
        void add_rot_force(VBase3 force);
        void integrate(); 
        float get_fwd_speed();
        float get_rot_speed();
        float get_dt();
        CImpulse get_c_impulse(string name);
        NodePath get_node_path();
        
    public:
        Vec3 m_vec_type;
        
    private:
        float m_fwdSpeed;
        float m_rotSpeed;
        float m_dt;
        
        pmap<std::string, CImpulse> m_c_impulses;
        
        VBase3 shove;
        VBase3 force;
        
        NodePath m_nodepath;
        
    TYPE_HANDLE(CMover, TypedObject);
};