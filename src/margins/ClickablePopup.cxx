#include "ClickablePopup.h"

#include <luse.h>
#include <mouseWatcherParameter.h>
#include <mouseButton.h>

NotifyCategoryDef(ClickablePopup, "");

TypeHandle ClickablePopup::_type_handle;
unsigned int ClickablePopup::ClickablePopup_serial = 0;

ClickablePopup::ClickablePopup() {
    ClickablePopup_cat.debug() << "__init__()" << std::endl;
    
    m_click_state = stateStopped;
    m_click_sound = NametagGlobals::m_click_sound;
    m_rollover_sound = NametagGlobals::m_rollover_sound;
};

ClickablePopup::~ClickablePopup() {

};

void ClickablePopup::destroy() {
    ClickablePopup_cat.debug() << "destory()" << std::endl;
};


void ClickablePopup::update_contents() {
    
};

void ClickablePopup::click() {
    update_contents();
}

void ClickablePopup::set_state(State state) {
    if (m_click_state != state) {
        m_click_state = state;
        update_contents();
    }
};

uint32_t ClickablePopup::get_state() {
    return m_click_state;
}

void ClickablePopup::press(MouseWatcherParameter param) {
    if (param.get_button() == MouseButton::one()) {
        if (m_click_sound) {
            m_click_sound->play();
        }
    }
}

void ClickablePopup::release(MouseWatcherParameter param) {
    if (param.get_button() == MouseButton::one()) {
        if ((param._keycode >> 2) & 1) {
            set_state(stateStopped);
        } else {
            set_state(stateRunning);
            update_contents();
        }
    }
}

void ClickablePopup::enter_region(MouseWatcherParameter param) {
    if (m_rollover_sound) {
        m_rollover_sound->play();  
        set_state(stateRunning);
    }
};

void ClickablePopup::exit_region(MouseWatcherParameter param) {
    set_state(stateStopped);
};