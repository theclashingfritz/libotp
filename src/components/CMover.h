#pragma once

#include "util.h"
#include <typedObject.h>

#include <nodePath.h>

class CMover;

class EXPCL_LIBMOVEMENT CMover : public TypedObject
{
    PUBLISHED:
        CMover(NodePath m_nodepath);
        ~CMover();
        void set_fwd_speed(int32_t speed);
        void set_rot_speed(int32_t speed);
        void set_node_path(NodePath np);
        int32_t get_fwd_speed();
        int32_t get_rot_speed();
        NodePath get_node_path();
        
    private:
        int32_t m_fwdSpeed;
        int32_t m_rotSpeed;
        NodePath m_nodepath;
};