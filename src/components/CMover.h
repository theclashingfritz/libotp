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
#include <clockObject.h>
#include "CImpulse.h"
#include "NametagGlobals.h"

NotifyCategoryDecl(CMover, EXPCL_LIBOTP, EXPTP_LIBOTP);

class CMover;

class EXPCL_LIBOTP CMover : public TypedReferenceCount {
    
    PUBLISHED:
        CMover(NodePath nodepath);
        CMover(NodePath nodepath, float m_fwdSpeed);
        CMover(NodePath nodepath, float m_fwdSpeed, float m_rotSpeed);
        ~CMover();
        
        void set_fwd_speed(float speed);
        void set_rot_speed(float speed);
        void set_node_path(NodePath np);
        void set_dt(double dt);
        void reset_dt();
        void add_c_impulse(string name, CImpulse impulse);
        void remove_c_impulse(string name);
        void process_c_impulses(float dt);
        void process_c_impulses();
        void add_shove(Vec3 shove);
        void add_rot_shove(Vec3 shove);
        void add_force(Vec3 force);
        void add_rot_force(Vec3 force);
        void integrate(); 
        float get_fwd_speed();
        float get_rot_speed();
        double get_dt();
        CImpulse get_c_impulse(string name);
        NodePath get_node_path();
        
    public:
        LVector3f m_vec_type;
        
    private:
        float m_fwd_speed;
        float m_rot_speed;
        float m_distance;
        
        float _dt;
        float _dt_clock;
        
        pmap<std::string, CImpulse> m_c_impulses;
        
        LVector3f vel;
        LVector3f step;
        
        LVector3f push_force;
        LVector3f rot_force;
        LVector3f movement;
        LVector3f rotation;
        
        Mat3 rot_mat;
        
        NodePath m_nodepath;
        
    TYPE_HANDLE(CMover, TypedObject);
};