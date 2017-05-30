#include "util.h"

#include "Nametag.h"
#include "NametagGroup.h"
#include "ChatBalloon.h"
#include "MarginManager.h"

NotifyCategoryDef(Nametag, "");

TypeHandle Nametag::_type_handle;

const float Nametag::name_padding = .2;
const float Nametag::chat_alpha = 1;

unsigned int Nametag::Nametag_serial = 0;

Nametag::Nametag(bool is_3d) : ClickablePopup(is_3d ? NametagGlobals::m_camera_nodepath : &NodePath()), m_contents(0), m_inner_np(NodePath::any_path(this).attach_new_node("nametag_contents")), m_wordwrap(7.5), m_chat_wordwrap(10), m_font(nullptr), m_qt_color(LVecBase4f(1)), m_color_code(NametagGlobals::CCNormal), m_avatar(nullptr), m_icon(NodePath("icon")), m_name_fg(LVecBase4f(0, 0, 0, 1)), m_name_bg(LVecBase4f(1)), m_chat_fg(LVecBase4f(0, 0, 0, 1)), m_chat_bg(LVecBase4f(1)), m_chat_flags(0) {
    Nametag_cat.debug() << "__init__(" << is_3d << ")" << std::endl;
    m_serial = Nametag::Nametag_serial++;
    frame = LVecBase4f(0.0, 0.0, 0.0, 0.0);
}

Nametag::~Nametag() {
    if (m_avatar != NULL && m_avatar != nullptr) {
        m_avatar = nullptr;
    }
}

/**
 * This little guy is a overload operator. Useful for comparing Nametag Objects!
 */
bool operator==(const Nametag& tag1, const Nametag& tag2) {
    return tag1.m_serial == tag2.m_serial;
}

void Nametag::set_draw_order(uint8_t draw_order) {
    Nametag_cat.debug() << "set_draw_order(" << draw_order << ")" << std::endl;
    m_draw_order = draw_order;
}

void Nametag::clear_draw_order() {
    Nametag_cat.debug() << "clear_draw_order()" << std::endl;
    m_draw_order = 0;
}

void Nametag::show_balloon(ChatBalloon* balloon, const std::wstring& text) {
    if (balloon == nullptr || balloon == NULL) {
        return;
    }
    
    Nametag_cat.debug() << "show_balloon(ChatBalloon balloon, '" << text << "')" << std::endl;
    LVecBase4f color = m_chat_flags & NametagGlobals::CFQuicktalker ? m_qt_color : m_chat_bg;
    
    if (color.get_w() > chat_alpha)
        color.set_w(chat_alpha);
        
    m_inner_np.attach_new_node(balloon->generate(text, m_font, &m_chat_fg, &color,
                                                 m_chat_wordwrap, &get_button()).node());
}

void Nametag::show_thought() {
    Nametag_cat.debug() << "show_thought()" << std::endl;
    show_balloon(get_thought_balloon(), m_chat_string);
}

void Nametag::show_speech() {
    Nametag_cat.debug() << "show_speech()" << std::endl;
    show_balloon(get_speech_balloon(), m_chat_string);
}

void Nametag::show_name() {
    Nametag_cat.debug() << "show_name()" << std::endl;
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
}

void Nametag::update() {
    Nametag_cat.debug() << "update()" << std::endl;
    unsigned int code = NametagGlobals::does_color_code_exist(m_color_code) ? m_color_code : NametagGlobals::CCNormal;
    Nametag_cat.spam() << "Removing Children!!" << std::endl;
    m_inner_np.node()->remove_all_children();
    
    Nametag_cat.spam() << "Getting Colors from Nametag Globals!" << std::endl;
    color_tuple_tuple_t colors = NametagGlobals::nametag_colors[code][get_click_state()];
    
    Nametag_cat.spam() << "Setting Name and Chat Colors!" << std::endl;
    color_tuple_t name_colors = colors[0];
    color_tuple_t chat_colors = colors[1];
    
    Nametag_cat.spam() << "Setting Name BG and FG!" << std::endl;
    m_name_fg = name_colors[0];
    m_name_bg = name_colors[1];
    
    Nametag_cat.spam() << "Setting Chat BG and FG!" << std::endl;
    m_chat_fg = chat_colors[0];
    m_chat_bg = chat_colors[1];
    
    Nametag_cat.spam() << "Preparing Name, Thought, or Speech!" << std::endl;
    if (m_contents & CThought && m_chat_flags & NametagGlobals::CFThought) {
        show_thought();
    } else if (m_contents & CSpeech && m_chat_flags & NametagGlobals::CFSpeech) {
        show_speech();
    } else if (m_contents & CName && m_display_name.size()) {
        show_name();
    }
}

void Nametag::set_active(bool active) {
    Nametag_cat.debug() << "set_active(" << active <<")" << std::endl;
    m_active = active;
}

void Nametag::set_contents(int contents) {
    Nametag_cat.debug() << "set_contents(" << contents <<")" << std::endl;
    m_contents = contents;
    update();
}

void Nametag::destroy() {
    Nametag_cat.debug() << "destroy()" << std::endl;
    ClickablePopup::destroy();
}

void Nametag::click_state_changed() {
    Nametag_cat.debug() << "click_state_changed()" << std::endl;
    update();
}

void Nametag::manage(MarginManager* manager) {
    return;
}

void Nametag::unmanage(MarginManager* manager) {
    return;
}

void Nametag::set_visible(bool flag) {
    return;
}
       
NodePath Nametag::get_button() {
    Nametag_cat.debug() << "get_button()" << std::endl;
    int cs = get_click_state();
    
    if (m_buttons.size() == 0) {
        return NodePath::not_found();
    } else if (cs < m_buttons.size()) {
        return m_buttons[cs];
    }
    
    return m_buttons.at(0);
}        

void Nametag::set_chat_wordwrap(uint16_t chat_wordwrap) {
    Nametag_cat.debug() << "set_chat_wordwrap(" << chat_wordwrap << ")" << std::endl;
    m_chat_wordwrap = chat_wordwrap;
}

void Nametag::set_group(NametagGroup* group) {
    Nametag_cat.debug() << "set_group(NametagGroup group)" << std::endl;
    if ((group == nullptr) || (group == NULL)) {
        return;
    } 
    
    m_has_group = true;
    m_group = group;
}

void Nametag::set_avatar(NodePath * avatar) {
    Nametag_cat.debug() << "set_avatar(NodePath avatar)" << std::endl;
    if ((avatar == nullptr) || (avatar == NULL)) {
        return;
    }
    
    m_avatar = avatar;
}

void Nametag::clear_avatar() {
    Nametag_cat.debug() << "clear_avatar()" << std::endl;
    if ((m_avatar == nullptr) || (m_avatar == NULL)) {
        return;
    }

    m_avatar = nullptr;
}

void Nametag::clear_group() {
    Nametag_cat.debug() << "clear_group()" << std::endl;
    if ((m_group == nullptr) || (m_group == NULL)) {
        return;
    } 

    m_has_group = false;
    m_group = nullptr;
}

std::wstring Nametag::get_name() {
    Nametag_cat.debug() << "get_name()" << std::endl;
    return m_name;
}

std::wstring Nametag::get_display_name() {
    Nametag_cat.debug() << "get_display_name()" << std::endl;
    return m_display_name;
}

unsigned int Nametag::get_contents() {
    Nametag_cat.debug() << "get_contents()" << std::endl;
    return m_contents;
}

uint8_t Nametag::get_draw_order() {
    Nametag_cat.debug() << "get_draw_order()" << std::endl;
    return m_draw_order;
}

uint16_t Nametag::get_chat_wordwrap() {
    Nametag_cat.debug() << "get_chat_wordwrap()" << std::endl;
    return m_chat_wordwrap;
}

bool Nametag::get_active() {
    Nametag_cat.debug() << "get_active()" << std::endl;
    return m_active;
}

bool Nametag::has_group() {
    Nametag_cat.debug() << "has_group()" << std::endl;
    return m_has_group;
}

NametagGroup * Nametag::get_group() {
    Nametag_cat.debug() << "get_group()" << std::endl;
    return m_group;
}

NodePath * Nametag::get_avatar() {
    Nametag_cat.debug() << "get_avatar()" << std::endl;
    return m_avatar;
}