#include "Nametag2d.h"
#include <math.h>

const float Nametag2d::scale_2d = .25;
const float Nametag2d::chat_alpha = .5;
const float Nametag2d::arrow_offset = -1;
const float Nametag2d::arrow_scale = 1.5;

TypeHandle Nametag2d::_type_handle;

Nametag2d::Nametag2d() : Nametag(), MarginPopup(), m_arrow(NULL) {
    m_contents = Nametag::CName | Nametag::CSpeech;
    m_chat_wordwrap = 7.5;
    m_inner_np.set_scale(scale_2d);
}

Nametag2d::~Nametag2d() {
}

void Nametag2d::show_balloon(ChatBalloon* balloon, const std::wstring& text) {
    wstringstream ss;
    ss << m_display_name << ": " << text;
    Nametag::show_balloon(balloon, ss.str());
    
    NodePath balloon_np, text_np;
    balloon_np = NodePath::any_path(this).find("*/balloon");
    text_np = balloon_np.find("**/+TextNode");
    
    PT(TextNode) tn = DCAST(TextNode, text_np.node());
    LVecBase4f frame = tn->get_frame_actual();
    LPoint3f center = m_inner_np.get_relative_point(text_np, LPoint3f((frame.get_x() + frame.get_y()) / 2., 0,
                                                                       (frame.get_z() + frame.get_w()) / 2.));
                                                    
    balloon_np.set_pos(balloon_np, -center);
    
    set_priority(1);
    
    if (m_arrow != NULL)
    {
        m_arrow->remove_node();
        m_arrow = NULL;
    }
}

void Nametag2d::show_name() {
    Nametag::show_name();
    
    set_priority(0);

    m_arrow = &NametagGlobals::m_arrow_nodepath.copy_to(m_inner_np);
    m_arrow->set_z(arrow_offset + DCAST(TextNode, m_inner_np.find("**/+TextNode").node())->get_bottom());
    m_arrow->set_scale(arrow_scale);
    m_arrow->set_color(m_name_fg);
}
   
void Nametag2d::update() {
    Nametag::update();
    consider_update_click_region();
}

void Nametag2d::margin_visibility_changed() {
    consider_update_click_region();
}

void Nametag2d::consider_update_click_region() {
    if (is_displayed())
        update_click_region(-1, 1, -1, 1);
}

void Nametag2d::tick() {
    if (!is_displayed() || m_arrow == NULL)
        return;
        
    if (m_avatar == NULL)
        return;
        
    NodePath camera = NametagGlobals::m_camera_nodepath;
    NodePath toon = NametagGlobals::m_nodepath.is_empty() ? camera : NametagGlobals::m_nodepath;
    
    if (m_avatar->is_empty())
        return;
        
    LVecBase3f pos = toon.get_quat(camera).xform(m_avatar->get_pos());
    double angle = atan(pos.get_x() / pos.get_y()) / 3.14159265 * 180;
    m_arrow->set_r(angle - 90);
}

ChatBalloon* Nametag2d::get_speech_balloon() {
    return NametagGlobals::speech_balloon_2d;
}

ChatBalloon* Nametag2d::get_thought_balloon() {
    return NametagGlobals::thought_balloon_2d;
}

bool Nametag2d::is_displayed() {
    if (!MarginPopup::is_displayed())
        return false;

    return m_chat_flags & NametagGlobals::CFSpeech;
}