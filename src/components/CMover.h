#pragma once

#include "util.h"

#include <nodePath.h>

class CMover;

class EXPCL_LIBMOVEMENT CMover
{
    public:
        CMover();
        ~CMover();
        void set_fwd_speed(int32_t speed);
        int32_t get_fwd_speed();
        void set_rot_speed(int32_t speed);
        int32_t get_rot_speed();
        
    private:
        int32_t m_fwdSpeed;
        int32_t m_rotSpeed;
};