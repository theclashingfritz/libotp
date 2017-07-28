#include "util.h"

#include "PopupMouseWatcherRegion.h"

#include "Nametag.h"
#include "NametagGroup.h"
#include "ChatBalloon.h"
#include "MarginManager.h"

NotifyCategoryDef(Nametag, "");

TypeHandle Nametag::_type_handle;

const float Nametag::name_padding = .2;
const float Nametag::chat_alpha = 1;

unsigned int Nametag::Nametag_serial = 0;

Nametag::Nametag(bool is_3d) : m_contents(0), m_wordwrap(7.5), m_chat_wordwrap(10), m_font(nullptr), m_qt_color(LVecBase4f(1)), m_color_code(NametagGlobals::CCNormal), m_avatar(nullptr), m_icon(NodePath("icon")), m_name_fg(LVecBase4f(0, 0, 0, 1)), m_name_bg(LVecBase4f(1)), m_chat_fg(LVecBase4f(0, 0, 0, 1)), m_chat_bg(LVecBase4f(1)), m_chat_flags(0) {
    Nametag_cat.debug() << "__init__(" << is_3d << ")" << std::endl;
    m_serial = Nametag::Nametag_serial++;
    m_is_3d = is_3d;
    frame = LVecBase4f(0.0, 0.0, 0.0, 0.0);
}

Nametag::~Nametag() {
    if (m_avatar != NULL && m_avatar != nullptr) {
        m_avatar = nullptr;
    }
    if (m_group != NULL && m_group != nullptr) {
        m_group = nullptr;
    }
}

Nametag::Nametag(const Nametag& tag) : m_font(nullptr), m_avatar(nullptr), m_icon(NodePath("icon")) {
    Nametag_cat.debug() << "__init__(const Nametag& tag)" << std::endl;
    m_serial = Nametag::Nametag_serial++;
    frame = LVecBase4f(tag.frame);
    m_draw_order = tag.m_draw_order;
    m_is_3d = tag.m_is_3d;
    m_active = tag.m_active;
    m_contents = tag.m_contents;
    m_wordwrap = tag.m_wordwrap;
    m_chat_wordwrap = tag.m_chat_wordwrap;
    m_chat_flags = tag.m_chat_flags;
    m_color_code = tag.m_color_code;
    m_name_fg = LVecBase4f(tag.m_name_fg);
    m_name_bg = LVecBase4f(tag.m_name_bg);
    m_chat_fg = LVecBase4f(tag.m_chat_fg);
    m_chat_bg = LVecBase4f(tag.m_chat_bg);
    m_qt_color = LVecBase4f(tag.m_qt_color);
}

void Nametag::operator =(const Nametag& tag) {
    frame = LVecBase4f(tag.frame);
    m_draw_order = tag.m_draw_order;
    m_active = tag.m_active;
    m_contents = tag.m_contents;
    m_wordwrap = tag.m_wordwrap;
    m_chat_wordwrap = tag.m_chat_wordwrap;
    m_chat_flags = tag.m_chat_flags;
    m_color_code = tag.m_color_code;
    m_name_fg = LVecBase4f(tag.m_name_fg);
    m_name_bg = LVecBase4f(tag.m_name_bg);
    m_chat_fg = LVecBase4f(tag.m_chat_fg);
    m_chat_bg = LVecBase4f(tag.m_chat_bg);
    m_qt_color = LVecBase4f(tag.m_qt_color);
}

bool Nametag::operator ==(const Nametag& tag) {
    return m_serial == tag.m_serial;
}

bool Nametag::operator !=(const Nametag& tag) {
    return m_serial != tag.m_serial;
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

void Nametag::click() {
    if (m_has_group) {
        m_group->click();
    }
}

void Nametag::show_name() {

}

void Nametag::update() {

}

void Nametag::update_contents() {

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

void Nametag::set_region(LVecBase4f region_frame, int v1) {
    if (_region) {
        _region->set_frame(LVecBase4f(region_frame));
    } else {
        std::string name = Nametag::get_type().get_name();
        name += '-';
        name += ws2s(m_name);
        _region = new PopupMouseWatcherRegion(this, name, LVecBase4f(region_frame));
    }
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
    State cs = m_click_state;
    
    if (m_buttons.size() == 0) {
        return NodePath::not_found();
    } else if (cs < m_buttons.size()) {
        return m_buttons[int(cs)];
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