#include "CMover.h"

CMover::CMover() {

}

CMover::~CMover() {
    
}

void CMover::set_fwd_speed(int32_t speed) {
    m_fwdSpeed = speed;
}

int32_t CMover::get_fwd_speed() {
    return m_fwdSpeed;
}

void CMover::set_rot_speed(int32_t speed) {
    m_rotSpeed = speed;
}

int32_t CMover::get_rot_speed() {
    return m_rotSpeed;
}