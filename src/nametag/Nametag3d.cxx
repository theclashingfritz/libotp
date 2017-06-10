#include "Nametag3d.h"

#include <billboardEffect.h>
#include <math.h>

NotifyCategoryDef(Nametag3d, "");

TypeHandle Nametag3d::_type_handle;

Nametag3d::Nametag3d() : Nametag(true), m_bb_offset(3) {
    Nametag3d_cat.debug() << "__init__()" << std::endl;
    m_contents = Nametag::CName | Nametag::CSpeech | Nametag3d::CThought;
    billboard();
}

Nametag3d::Nametag3d(const Nametag3d& tag) : Nametag() {
    Nametag3d_cat.debug() << "__init__()" << std::endl;
    m_contents = tag.m_contents;
    m_chat_flags = tag.m_chat_flags;
    m_color_code = tag.m_color_code;
    m_name = std::wstring(tag.m_name);
    m_display_name = std::wstring(tag.m_display_name);
    m_chat_string = std::wstring(tag.m_chat_string);
    m_chat_wordwrap = tag.m_chat_wordwrap;
    m_wordwrap = tag.m_wordwrap;
    m_bb_offset = tag.m_bb_offset;
    m_name_fg = LVecBase4f(tag.m_name_fg);
    m_name_bg = LVecBase4f(tag.m_name_bg);
    m_chat_fg = LVecBase4f(tag.m_chat_fg);
    m_chat_bg = LVecBase4f(tag.m_chat_bg);
    m_avatar = &*tag.m_avatar; 
    m_group = &*tag.m_group;
    m_font = tag.m_font; 
    m_has_group = tag.m_has_group;
    m_active = tag.m_active;
    billboard();
}

Nametag3d::~Nametag3d() {

}

/**
 * This little guy is a overload operator. We use this to assign a Nametag object to this object!
 */
Nametag3d& Nametag3d::operator=(const Nametag3d& tag) {
    m_contents = tag.m_contents;
    m_chat_flags = tag.m_chat_flags;
    m_color_code = tag.m_color_code;
    m_name = std::wstring(tag.m_name);
    m_display_name = std::wstring(tag.m_display_name);
    m_chat_string = std::wstring(tag.m_chat_string);
    m_chat_wordwrap = tag.m_chat_wordwrap;
    m_wordwrap = tag.m_wordwrap;
    m_bb_offset = tag.m_bb_offset;
    m_name_fg = LVecBase4f(tag.m_name_fg);
    m_name_bg = LVecBase4f(tag.m_name_bg);
    m_chat_fg = LVecBase4f(tag.m_chat_fg);
    m_chat_bg = LVecBase4f(tag.m_chat_bg);
    m_avatar = &*tag.m_avatar; 
    m_group = &*tag.m_group;
    m_font = tag.m_font; 
    m_has_group = tag.m_has_group;
    m_active = tag.m_active;
    billboard();
    return *this;
}

void Nametag3d::tick() {
    Nametag3d_cat.debug() << "tick()" << std::endl;
    if (NametagGlobals::m_camera_nodepath->is_empty()) {
        return;
    }
    double dist = m_inner_np.get_pos(*NametagGlobals::m_camera_nodepath).length();
    dist = (dist > 50 ? 50 : dist) < 1 ? 1: dist;
    float scale = sqrt(dist) * .055;
    m_inner_np.set_scale(scale);
    
    if ((this->is_hidden()) || (this->get_top() != NametagGlobals::m_camera_nodepath->get_top() && this->get_top().get_name() != "render2d")) {
        disable_click_region();
    } else {
        float x = *new float(frame.get_x() * scale);
        float y = *new float(frame.get_y() * scale);
        float z = *new float(frame.get_z() * scale);
        float w = *new float(frame.get_w() * scale);

        update_click_region(x, y, z, w);
    }
}

void Nametag3d::set_billboard_offset(float offset) {
    m_bb_offset = offset;
}

void Nametag3d::billboard() {
    Nametag3d_cat.debug() << "billboard()" << std::endl;
    if (NametagGlobals::m_camera_nodepath->is_empty()) {
        return;
    }
    m_inner_np.set_effect(BillboardEffect::make(LVecBase3f(0, 0, 1), true, false, m_bb_offset, *NametagGlobals::m_camera_nodepath, LPoint3f(0)));
}

void Nametag3d::update_contents() {
    update();
}

float Nametag3d::get_billboard_offset() {
    return m_bb_offset;
}

bool Nametag3d::safe_to_flatten_below() {
    return false;
}

ChatBalloon* Nametag3d::get_speech_balloon() {
    Nametag3d_cat.debug() << "get_speech_balloon()" << std::endl;
    if (NametagGlobals::speech_balloon_3d == nullptr || NametagGlobals::speech_balloon_3d == NULL) {
        return nullptr;
    }
    return NametagGlobals::speech_balloon_3d;
}

ChatBalloon* Nametag3d::get_thought_balloon() {
    Nametag3d_cat.debug() << "get_thought_balloon()" << std::endl;
    if (NametagGlobals::thought_balloon_3d == nullptr || NametagGlobals::thought_balloon_3d == NULL) {
        return nullptr;
    }
    return NametagGlobals::thought_balloon_3d;
}