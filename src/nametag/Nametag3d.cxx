#include "Nametag3d.h"

#include <billboardEffect.h>
#include <math.h>

NotifyCategoryDef(Nametag3d, "");

TypeHandle Nametag3d::_type_handle;

Nametag3d::Nametag3d() : Nametag(), m_bb_offset(3) {
    Nametag3d_cat.debug() << "__init__()" << std::endl;
    m_contents = Nametag::CName | Nametag::CSpeech | Nametag3d::CThought;
    billboard();
}

Nametag3d::Nametag3d(const Nametag3d& tag) : Nametag() {
    Nametag3d_cat.debug() << "__init__()" << std::endl;
    m_contents = *new int(tag.m_contents);
    m_chat_flags = *new int(tag.m_chat_flags);
    m_color_code = *new unsigned int(tag.m_color_code);
    m_name = *new std::wstring(tag.m_name);
    m_display_name = *new std::wstring(tag.m_display_name);
    m_chat_string = *new std::wstring(tag.m_chat_string);
    m_chat_wordwrap = *new float(tag.m_chat_wordwrap);
    m_wordwrap = *new float(tag.m_wordwrap);
    m_bb_offset = *new float(tag.m_bb_offset);
    m_name_fg = *new LVecBase4f(tag.m_name_fg);
    m_name_bg = *new LVecBase4f(tag.m_name_bg);
    m_chat_fg = *new LVecBase4f(tag.m_chat_fg);
    m_chat_bg = *new LVecBase4f(tag.m_chat_bg);
    m_avatar = tag.m_avatar; 
    m_group = tag.m_group;
    m_font = tag.m_font; 
    m_has_group = bool(*new int(tag.m_has_group));
    m_active = bool(*new int(tag.m_active));
    billboard();
}

Nametag3d::~Nametag3d() {

}

/**
 * This little guy is a overload operator. We use this to assign a Nametag object to this object!
 */
Nametag3d& Nametag3d::operator=(const Nametag3d& tag) {
    m_contents = *new int(tag.m_contents);
    m_chat_flags = *new int(tag.m_chat_flags);
    m_color_code = *new unsigned int(tag.m_color_code);
    m_name = *new std::wstring(tag.m_name);
    m_display_name = *new std::wstring(tag.m_display_name);
    m_chat_string = *new std::wstring(tag.m_chat_string);
    m_chat_wordwrap = *new float(tag.m_chat_wordwrap);
    m_wordwrap = *new float(tag.m_wordwrap);
    m_bb_offset = *new float(tag.m_bb_offset);
    m_name_fg = *new LVecBase4f(tag.m_name_fg);
    m_name_bg = *new LVecBase4f(tag.m_name_bg);
    m_chat_fg = *new LVecBase4f(tag.m_chat_fg);
    m_chat_bg = *new LVecBase4f(tag.m_chat_bg);
    m_avatar = tag.m_avatar; 
    m_group = tag.m_group;
    m_font = tag.m_font; 
    m_has_group = bool(*new int(tag.m_has_group));
    m_active = bool(*new int(tag.m_active));
    billboard();
    return *this;
}

void Nametag3d::tick() {
    Nametag3d_cat.debug() << "tick()" << std::endl;
    double dist = m_inner_np.get_pos(NametagGlobals::m_camera_nodepath).length();
    dist = (dist > 50 ? 50 : dist) < 1 ? 1: dist;
    m_inner_np.set_scale(sqrt(dist) * .055);
    
    update_click_region(-1, 1, -1, 1);
}

void Nametag3d::billboard() {
    Nametag3d_cat.debug() << "billboard()" << std::endl;
    m_inner_np.set_effect(BillboardEffect::make(LVecBase3f(0, 0, 1), true, false, m_bb_offset, NametagGlobals::m_camera_nodepath, LPoint3f(0)));
}

ChatBalloon* Nametag3d::get_speech_balloon() {
    Nametag3d_cat.debug() << "get_speech_balloon()" << std::endl;
    return NametagGlobals::speech_balloon_3d;
}

ChatBalloon* Nametag3d::get_thought_balloon() {
    Nametag3d_cat.debug() << "get_thought_balloon()" << std::endl;
    return NametagGlobals::thought_balloon_3d;
}