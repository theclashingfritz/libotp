#include "Nametag2d.h"
#include <textFont.h>
#include <math.h>

NotifyCategoryDef(Nametag2d, "");

TypeHandle Nametag2d::_type_handle;

Nametag2d::Nametag2d() : Nametag(), MarginPopup(), PandaNode("popup"), m_arrow(nullptr) {
    Nametag2d_cat.debug() << "__init__()" << std::endl;
    m_contents = Nametag::CName | Nametag::CSpeech;
    m_chat_wordwrap = 7.5;
    m_inner_np.set_scale(NametagGlobals::_global_nametag_scale);
};

Nametag2d::Nametag2d(const Nametag2d& tag) : Nametag(), MarginPopup(), PandaNode("popup"), m_arrow(nullptr) {
    Nametag2d_cat.debug() << "__init__(Nametag2d)" << std::endl;
    m_inner_np.set_scale(NametagGlobals::_global_nametag_scale);
    m_contents = tag.m_contents;
    m_chat_flags = tag.m_chat_flags;
    m_color_code = tag.m_color_code;
    m_name = std::wstring(tag.m_name);
    m_display_name = std::wstring(tag.m_display_name);
    m_chat_string = std::wstring(tag.m_chat_string);
    m_chat_wordwrap = tag.m_chat_wordwrap;
    m_wordwrap = tag.m_wordwrap;
    m_name_fg = LVecBase4f(tag.m_name_fg);
    m_name_bg = LVecBase4f(tag.m_name_bg);
    m_chat_fg = LVecBase4f(tag.m_chat_fg);
    m_chat_bg = LVecBase4f(tag.m_chat_bg);
    m_avatar = &*tag.m_avatar;
    m_group = &*tag.m_group;
    m_font = tag.m_font; 
    m_has_group = tag.m_has_group;
    m_active = tag.m_active;
};

Nametag2d::~Nametag2d() {
};

/**
 * This little guy is a overload operator. We use this to assign a Nametag object to this object!
 */
Nametag2d& Nametag2d::operator=(const Nametag2d& tag) {
    m_contents = tag.m_contents;
    m_chat_flags = tag.m_chat_flags;
    m_color_code = tag.m_color_code;
    m_name = std::wstring(tag.m_name);
    m_display_name = std::wstring(tag.m_display_name);
    m_chat_string = std::wstring(tag.m_chat_string);
    m_chat_wordwrap = tag.m_chat_wordwrap;
    m_wordwrap = tag.m_wordwrap;
    m_name_fg = LVecBase4f(tag.m_name_fg);
    m_name_bg = LVecBase4f(tag.m_name_bg);
    m_chat_fg = LVecBase4f(tag.m_chat_fg);
    m_chat_bg = LVecBase4f(tag.m_chat_bg);
    m_avatar = &*tag.m_avatar;
    m_group = &*tag.m_group;
    m_font = tag.m_font; 
    m_has_group = tag.m_has_group;
    m_active = tag.m_active;
    return *this;
};

void Nametag2d::show_balloon(ChatBalloon* balloon, const std::wstring& text) {
    Nametag2d_cat.debug() << "show_balloon(ChatBalloon balloon " << text << ")" << std::endl;
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
    
    if (m_arrow != nullptr || m_arrow != NULL) {
        m_arrow->remove_node();
        m_arrow = nullptr;
    }
};

void Nametag2d::show_name() {
    Nametag2d_cat.debug() << "show_name()" << std::endl;
    if ((m_font == nullptr) || (m_font == NULL)) {
        return;
    }
        
    m_inner_np.attach_new_node(m_icon.node());
    
    PT(TextNode) tn = new TextNode("name");
    tn->set_font(m_font);
    tn->set_align(TextNode::A_center);
    tn->set_wordwrap(m_wordwrap);
    tn->set_wtext(m_display_name);
        
    double width = tn->get_width();
    double height = tn->get_height();
        
    NodePath t = m_inner_np.attach_new_node(tn, 1);
    t.set_color(m_name_fg);
    t.set_transparency(m_name_fg.get_w() < 1 ? TransparencyAttrib::M_alpha : TransparencyAttrib::M_none);
    t.set_y(-0.25);

    NodePath panel = NametagGlobals::m_nametag_model_nodepath->copy_to(m_inner_np);
    panel.set_x((tn->get_left() + tn->get_right()) / 2.);
    panel.set_z((tn->get_top() + tn->get_bottom()) / 2.);
    panel.set_scale(width + name_padding, 1, height + name_padding);
    panel.set_color(m_name_bg);
    panel.set_transparency(m_name_bg.get_w() < 1 ? TransparencyAttrib::M_alpha : TransparencyAttrib::M_none);
    
    frame = LVecBase4f(float(tn->get_left() - name_padding / 2.), float(tn->get_right() + name_padding + 2.), 
                       float(tn->get_bottom() - name_padding / 2.),  float(tn->get_top() + name_padding / 2.));
    
    set_priority(0);

    m_arrow = &NametagGlobals::m_arrow_nodepath->copy_to(m_inner_np);
    m_arrow->set_z(NametagGlobals::arrow_offset + DCAST(TextNode, m_inner_np.find("**/+TextNode").node())->get_bottom());
    m_arrow->set_scale(NametagGlobals::arrow_scale);
    m_arrow->set_color(m_name_fg);
};
   
void Nametag2d::update() {
    // This is also deperacted. 
};

void Nametag2d::update_contents() {
    Nametag2d_cat.debug() << "update_contents()" << std::endl;
    
    unsigned int code = NametagGlobals::does_color_code_exist(m_color_code) ? m_color_code : NametagGlobals::CCNormal;
    Nametag2d_cat.spam() << "Removing Children!!" << std::endl;
    if (!m_inner_np.is_empty()) {
        m_inner_np.node()->remove_all_children();
    }
    
    Nametag2d_cat.spam() << "Getting Colors from Nametag Globals!" << std::endl;
    color_tuple_tuple_t colors = NametagGlobals::nametag_colors[code][int(m_click_state)];
    
    Nametag2d_cat.spam() << "Setting Name and Chat Colors!" << std::endl;
    color_tuple_t name_colors = colors[0];
    color_tuple_t chat_colors = colors[1];
    
    Nametag2d_cat.spam() << "Setting Name BG and FG!" << std::endl;
    m_name_fg = name_colors[0];
    m_name_bg = name_colors[1];
    
    Nametag2d_cat.spam() << "Setting Chat BG and FG!" << std::endl;
    m_chat_fg = chat_colors[0];
    m_chat_bg = chat_colors[1];
    
    Nametag2d_cat.spam() << "Preparing Name, Thought, or Speech!" << std::endl;
    if (m_contents & CThought && m_chat_flags & NametagGlobals::CFThought) {
        show_thought();
    } else if (m_contents & CSpeech && m_chat_flags & NametagGlobals::CFSpeech) {
        show_speech();
    } else if (m_contents & CName && m_display_name.size()) {
        show_name();
    }
};

void Nametag2d::margin_visibility_changed() {
    Nametag2d_cat.debug() << "margin_visibility_changed()" << std::endl;
    consider_update_click_region();
};

void Nametag2d::consider_update_click_region() {
    Nametag2d_cat.debug() << "consider_update_click_region()" << std::endl;
    if (is_displayed()) {
        float x = *new float(frame.get_x() * NametagGlobals::_global_nametag_scale);
        float y = *new float(frame.get_y() * NametagGlobals::_global_nametag_scale);
        float z = *new float(frame.get_z() * NametagGlobals::_global_nametag_scale);
        float w = *new float(frame.get_w() * NametagGlobals::_global_nametag_scale);
    } else {
        return;
    }
};

void Nametag2d::tick() {
    // Tick is only here because of Nametag3d not being completely RE'd enoguh to remove it.
};

void Nametag2d::cull_callback(CullTraverser *traverser, CullTraverserData *traverser_data) {
    rotate_arrow();
};

void Nametag2d::rotate_arrow() {
    Nametag2d_cat.debug() << "rotate_arrow()" << std::endl;
    if (!is_displayed() || (m_arrow == nullptr || m_arrow == NULL)) {
        return;
    }
        
    if ((m_avatar == nullptr || m_avatar == NULL) || (m_avatar->is_empty())) {
        return;
    }
    NodePath camera = *NametagGlobals::m_camera_nodepath;
    NodePath toon = NametagGlobals::m_nodepath->is_empty() ? camera : *NametagGlobals::m_nodepath;
    LVecBase3f pos1 = toon.get_pos(camera);
    LVecBase3f pos2 = m_avatar->get_pos(camera);
    LPoint3f distance = pos1 - pos2;
    double angle = atan2(distance.get_x(), distance.get_y()) * (180 / 3.14159265);
    const LVecBase3f hpr(0.0, 0.0, -angle);
    const LVecBase3f scale(0.5, 0.5, 0.5);
    const LVecBase3f shear(0.0, 0.0, 0.0);
    LMatrix4f mat;
    compose_matrix(mat, scale, shear, hpr, 0);
    m_arrow->set_mat(mat);
};

ChatBalloon* Nametag2d::get_speech_balloon() {
    Nametag2d_cat.debug() << "get_speech_balloon()" << std::endl;
    return NametagGlobals::speech_balloon_2d;
};

ChatBalloon* Nametag2d::get_thought_balloon() {
    Nametag2d_cat.debug() << "get_thought_balloon()" << std::endl;
    return NametagGlobals::thought_balloon_2d;
};

bool Nametag2d::is_displayed() {
    Nametag2d_cat.debug() << "is_displayed()" << std::endl;
    if (!MarginPopup::is_displayed()) {
        return false;
    }

    return m_chat_flags & NametagGlobals::CFSpeech;
};

INLINE TypedObject *Nametag2d::as_typed_object() {
   return PandaNode::as_typed_object();
} 

INLINE const TypedObject *Nametag2d::as_typed_object() const {
   return PandaNode::as_typed_object();
}

INLINE void Nametag2d::ref() const {
#ifdef _DEBUG
  nassertv(test_ref_count_integrity());
#endif

  AtomicAdjust::inc(_ref_count);
}