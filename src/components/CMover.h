#pragma once

#include "util.h"
#include <typedObject.h>

#include <nodePath.h>

class CMover;

class EXPCL_LIBOTP CMover : public TypedObject
{
    PUBLISHED:
        CMover(NodePath m_nodepath);
        ~CMover();
        void set_fwd_speed(float speed);
        void set_rot_speed(float speed);
        void set_node_path(NodePath np);
        void set_dt(float dt);
        void reset_dt();
        float get_fwd_speed();
        float get_rot_speed();
        float get_dt();
        NodePath get_node_path();
        
    private:
        float m_fwdSpeed;
        float m_rotSpeed;
        float m_dt;
        NodePath m_nodepath;
        
    TYPE_HANDLE(CMover, TypedObject);
};