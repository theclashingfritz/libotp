#include "ClickablePopup.h"

#include <eventHandler.h>
#include <throw_event.h>

static EventHandler* g_event_handler = EventHandler::get_global_event_handler();

TypeHandle ClickablePopup::_type_handle;
unsigned int ClickablePopup::ClickablePopup_serial = 0;

ClickablePopup::ClickablePopup(NodePath* camera) : PandaNode("popup"), EventReceiver(), m_mouse_watcher(NametagGlobals::m_mouse_watcher), m_click_sound(NametagGlobals::m_click_sound), m_rollover_sound(NametagGlobals::m_rollover_sound), m_cam(camera), m_disabled(false), m_clicked(false), m_hovered(false), m_click_state(CLICKSTATE_NORMAL), m_click_event("") {
    m_name = "ClickablePopup-";
    m_name += ClickablePopup::ClickablePopup_serial++;
    
    if (m_mouse_watcher != NULL) {
        m_region_name = m_name;
        m_region_name += "-region";
        m_region = new MouseWatcherRegion(m_region_name, 0, 0, 0, 0);
        m_mouse_watcher->add_region(m_region);
    
        accept(get_event(m_mouse_watcher->get_enter_pattern()));
        accept(get_event(m_mouse_watcher->get_leave_pattern()));
        accept(get_event(m_mouse_watcher->get_button_down_pattern()));
        accept(get_event(m_mouse_watcher->get_button_up_pattern()));
    }
}

ClickablePopup::~ClickablePopup() {
    ignore_all();
}

void ClickablePopup::destroy() {
    if (m_mouse_watcher != NULL) {
        m_mouse_watcher->remove_region(m_region);
    }
    ignore_all();
}

void ClickablePopup::set_click_region_event(const std::string& event) {
    if (!event.size())
        m_disabled = true;
    
    else{
        m_click_event = event;
        m_disabled = false;
    }
    
    update_click_state();
}

int ClickablePopup::get_click_state() {
    return m_click_state;
}


const std::string ClickablePopup::get_event(const std::string& pattern) {
    std::string result = pattern;
    result.replace(result.find("%r"), m_name.size(), m_name);
    return result;
}

void ClickablePopup::update_click_state() {
    int state, old_state;
    
    if (m_disabled)
        state = CLICKSTATE_DISABLED;
        
    else if (m_clicked)
        state = CLICKSTATE_CLICK;
        
    else if (m_hovered)
        state = CLICKSTATE_HOVER;
        
    else
       state = CLICKSTATE_NORMAL;
        
    if (m_click_state == state)
        return;
        
    old_state = m_click_state;
    m_click_state = state;
    
    if (old_state == CLICKSTATE_NORMAL && state == CLICKSTATE_HOVER)
        if (m_rollover_sound != NULL) {
            m_rollover_sound->play();  
        }
        
    else if (state == CLICKSTATE_CLICK)
        if (m_click_sound != NULL) {
            m_click_sound->play();  
        }
        
    else if (old_state == CLICKSTATE_CLICK && state == CLICKSTATE_HOVER)
        throw_event(m_click_event);
}

void ClickablePopup::accept(const std::string& ev) {
    g_event_handler->add_hook(ev, &ClickablePopup::handle_event, (void*)this);
}

void ClickablePopup::ignore_all() {
    g_event_handler->remove_hooks_with(this);
}

void ClickablePopup::update_click_region(float left, float right, float bottom, float top) {
    CPT(TransformState) transform = NodePath::any_path(this).get_net_transform();
    if (m_cam != NULL) {
        CPT(TransformState) cam_transform = m_cam->get_net_transform();
        transform = cam_transform->get_inverse()->compose(transform);
    }
    
    transform = transform->set_quat(LQuaternionf());
    LMatrix4f mat = transform->get_mat();
    LVecBase3f c_top_left = mat.xform_point(LPoint3f(left, 0, top));
    LVecBase3f c_bottom_right = mat.xform_point(LPoint3f(right, 0, bottom));
    
    LPoint2f s_top_left, s_bottom_right;
    
    if (m_cam != NULL) {
        PT(Lens) lens = DCAST(Camera, m_cam->node())->get_lens();
        
        if (!lens->project(LPoint3f(c_top_left), s_top_left) || !lens->project(LPoint3f(c_bottom_right), s_bottom_right)) {
            if (m_region != NULL)
                m_region->set_active(false);
            return;
        }
    } else {
        s_top_left = LPoint2f(s_top_left.get_x(), s_top_left.get_y());
        s_bottom_right = LPoint2f(s_bottom_right.get_x(), s_bottom_right.get_y());
    }
    
    if (m_region != NULL) {
        m_region->set_frame(s_top_left.get_x(), s_bottom_right.get_x(), s_top_left.get_y(), s_bottom_right.get_y());
        m_region->set_active(true);
    }
}

void ClickablePopup::mouse_enter(const Event* ev) {
    m_hovered = true;
    update_click_state();
}

void ClickablePopup::mouse_leave(const Event* ev) {
    m_hovered = false;
    update_click_state();
}

void ClickablePopup::button_down(const Event* ev) {
    if (ev->get_parameter(1).get_string_value() == "button1") {
        m_clicked = true;
        update_click_state();
    }
}

void ClickablePopup::button_up(const Event* ev) {
    if (ev->get_parameter(1).get_string_value() == "button1") {
        m_clicked = false;
        update_click_state();
    }
}


void ClickablePopup::handle_event(const Event* ev, void* data) {
    ClickablePopup* _this = (ClickablePopup*)data;
    const std::string name = ev->get_name();
    
    if (name == _this->get_event(_this->m_mouse_watcher->get_enter_pattern()))
        _this->mouse_enter(ev);
        
    if (name == _this->get_event(_this->m_mouse_watcher->get_leave_pattern()))
        _this->mouse_leave(ev);
    
    if (name == _this->get_event(_this->m_mouse_watcher->get_button_down_pattern()))
        _this->button_down(ev);
    
    if (name == _this->get_event(_this->m_mouse_watcher->get_button_up_pattern()))
        _this->button_up(ev);
}