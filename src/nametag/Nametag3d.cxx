#include "Nametag3d.h"

#include <billboardEffect.h>
#include <math.h>

TypeHandle Nametag3d::_type_handle;

Nametag3d::Nametag3d() : Nametag(), m_bb_offset(3) {
    m_contents = Nametag::CName | Nametag::CSpeech | Nametag3d::CThought;
    billboard();
}

Nametag3d::~Nametag3d() {

}

void Nametag3d::tick() {
    double dist = m_inner_np.get_pos(NametagGlobals::m_camera_nodepath).length();
    dist = (dist > 50 ? 50 : dist) < 1 ? 1: dist;
    m_inner_np.set_scale(sqrt(dist) * .055);
    
    update_click_region(-1, 1, -1, 1);
}

void Nametag3d::billboard() {
    m_inner_np.set_effect(BillboardEffect::make(LVecBase3f(0, 0, 1), true, false, m_bb_offset, NametagGlobals::m_camera_nodepath, LPoint3f(0)));
}

ChatBalloon* Nametag3d::get_speech_balloon() {
    return NametagGlobals::speech_balloon_3d;
}

ChatBalloon* Nametag3d::get_thought_balloon() {
    return NametagGlobals::thought_balloon_3d;
}