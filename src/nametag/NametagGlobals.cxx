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
    
    LVecBase4f CCSuit[] = {LVecBase4f(0.2, 0.2, 0.2, 1.0), LVecBase4f(0.8, 0.8, 0.8, 0.375), LVecBase4f(0.2, 0.2, 0.2, 1.0), 
                LVecBase4f(0.2, 0.2, 0.2, 0.1875), LVecBase4f(0.4, 0.4, 0.4, 1.0), LVecBase4f(1.0, 1.0, 1.0, 0.5625),
                LVecBase4f(0.2, 0.2, 0.2, 1.0), LVecBase4f(0.8, 0.8, 0.8, 0.375)};
}

NametagGlobals::~NametagGlobals() {

}

void NametagGlobals::set_me(NodePath me) {
    m_nodepath = me;
}

void NametagGlobals::set_want_active_nametags(bool want) {
    m_want_active_nametags = want;
}

void NametagGlobals::set_force_onscreen_chat(bool want) {
    m_force_onscreen_chat = want;
}


