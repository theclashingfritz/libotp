#include "NametagGlobals.h"

TypeHandle NametagGlobals::_type_handle;

NametagGlobals::NametagGlobals() {
    CFSpeech = 1;
    CFThought = 2;
    CFQuicktalker = 4;
    CFTimeout = 8;
    CFPageButton = 16;
    CFQuitButton = 32;
    CFReversed = 64;
    CFSndOpenchat = 128;
    CFNoQuitButton = 256;
    
    m_want_active_nametags = 1;
    m_force_2d_nametags = 0;
    m_force_onscreen_chat = 0;
    m_click_sound = NULL;
    
    LVecBase4f CCSuit[] = {
        LVecBase4f(0.2, 0.2, 0.2, 1.0), 
        LVecBase4f(0.8, 0.8, 0.8, 0.375), 
        LVecBase4f(0.2, 0.2, 0.2, 1.0), 
        LVecBase4f(0.2, 0.2, 0.2, 0.1875), 
        LVecBase4f(0.4, 0.4, 0.4, 1.0), 
        LVecBase4f(1.0, 1.0, 1.0, 0.5625),
        LVecBase4f(0.2, 0.2, 0.2, 1.0), 
        LVecBase4f(0.8, 0.8, 0.8, 0.375)};
}

NametagGlobals::~NametagGlobals() {
    delete[] m_click_sound;
}

void NametagGlobals::set_toon(NodePath toon) {
    m_nodepath = toon;
}

void NametagGlobals::set_arrow_model(NodePath node) {
    m_arrow_nodepath = node;
}

void NametagGlobals::set_camera(NodePath node) {
    m_camera_nodepath = node;
}

void NametagGlobals::set_want_active_nametags(bool want) {
    m_want_active_nametags = want;
}

void NametagGlobals::set_force_onscreen_chat(bool want) {
    m_force_onscreen_chat = want;
}

void NametagGlobals::set_force_2d_nametags(bool want) {
    m_force_2d_nametags = want;
}

void NametagGlobals::set_click_sound(PT(AudioSound) sound) {
    m_click_sound = sound;
}

void NametagGlobals::set_rollover_sound(PT(AudioSound) sound) {
    m_rollover_sound = sound;
}

NodePath NametagGlobals::get_toon() {
    return m_nodepath;
}

NodePath NametagGlobals::get_arrow_model() {
    return m_arrow_nodepath;
}

NodePath NametagGlobals::get_camera() {
    return m_camera_nodepath;
}

bool NametagGlobals::get_want_active_nametags() {
    return m_want_active_nametags;
}

bool NametagGlobals::get_force_onscreen_chat() {
    return m_force_onscreen_chat;
}

bool NametagGlobals::get_force_2d_nametags() {
    return m_force_2d_nametags;
}