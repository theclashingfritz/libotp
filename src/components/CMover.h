#pragma once

#include "util.h"

#include <nodePath.h>

class CMover;

class EXPCL_LIBMOVEMENT CMover
{
    PUBLISHED:
        CMover();
        ~CMover();
        void set_fwd_speed(int32_t speed);
        
    private:
        int32_t fwdSpeed;
};