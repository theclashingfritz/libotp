#include "Nametag.h"

TypeHandle Nametag::_type_handle;

Nametag::Nametag(PandaNode m_pandaNode) {
    CSpeech = 2;
    CThought = 4;
    CName = 1;

    m_draw_order = 0;
    m_contents = 0;
    m_active = false;
    m_chat_wordwrap = 0;
    m_avatar = NULL;
}

Nametag::~Nametag() {

}

void Nametag::set_draw_order(uint8_t draw_order) {
    m_draw_order = draw_order;
}

void Nametag::clear_draw_order() {
    m_draw_order = 0;
}

uint8_t Nametag::get_draw_order() {
    return m_draw_order;
}

void Nametag::set_active(bool active) {
    m_active = active;
}

bool Nametag::get_active() {
    return m_active;
}

void Nametag::set_contents(unsigned int contents) {
    m_contents = contents;
}

unsigned int Nametag::get_contents() {
    return m_contents;
}

void Nametag::set_chat_wordwrap(uint16_t chat_wordwrap) {
    m_chat_wordwrap = chat_wordwrap;
}

uint16_t Nametag::get_chat_wordwrap() {
    return m_chat_wordwrap;
}

void Nametag::set_avatar(NodePath avatar) {
    m_avatar = avatar;
}

NodePath Nametag::get_avatar() {
    return m_avatar;
}

void Nametag::clear_avatar() {
    if (!m_avatar) {
        return;
    }

    m_avatar = NULL;
}