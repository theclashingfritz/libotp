#include "WhisperPopup.h"
#include "MarginCell.h"

#include <asyncTaskManager.h>
 
static PT(AsyncTaskManager) g_task_mgr = AsyncTaskManager::get_global_ptr(); 

NotifyCategoryDef(WhisperPopup, "");

TypeHandle WhisperPopup::_type_handle;

WhisperPopup::WhisperPopup(const std::wstring& text, PT(TextFont) font, const unsigned int whisper_type, const float timeout): ClickablePopup(), MarginPopup(), m_text(text), m_font(font), m_whisper_type(whisper_type), m_timeout(timeout) {
    WhisperPopup_cat.debug() << "__init__(" << text << " " << "TextFont font" << " " << "NametagGlobals::WhisperType whisper_type" << " " << timeout << ")" << std::endl;
    m_inner_np = NodePath::any_path(this).attach_new_node("inner_np");
    m_inner_np.set_scale(.25);
    m_from_id = 0;
    m_active = false;
    
    MarginCell* m_cell = get_assigned_cell();
    if (m_cell != nullptr && m_cell != NULL) {
        m_cell->set_content_nodepath(this);
    }
    
    update_contents();
    set_priority(2);
    set_visible(true);
}

WhisperPopup::~WhisperPopup() {
}

void WhisperPopup::update_contents() {
    WhisperPopup_cat.debug() << "update_contents()" << std::endl;
    unsigned int cc;
    
    if (NametagGlobals::does_whisper_type_exist(m_whisper_type)) {
        cc = m_whisper_type;
    } else {
        cc = NametagGlobals::WTSystem;
    }
        
    color_tuple_t colors = NametagGlobals::get_whisper_colors(cc, CLICKSTATE_NORMAL);
    
    NodePath balloon = NametagGlobals::speech_balloon_2d->generate(m_text, m_font, &colors[0], &colors[1], 7.5);
    balloon.reparent_to(m_inner_np);
    
    NodePath text_np = balloon.find("**/+TextNode");
    PT(TextNode) tn = DCAST(TextNode, text_np.node());
    frame = tn->get_frame_actual();
    LPoint3f center = m_inner_np.get_relative_point(text_np, LPoint3f((frame.get_x() + frame.get_y()) / 2., 0, (frame.get_z() + frame.get_w()) / 2.));
    
    balloon.set_pos(balloon, -center);
    
    if (m_active != false && m_from_id != 0) {
        set_click_region_event("clickedWhisper", m_from_id);
    }
}

void WhisperPopup::set_clickable(const std::wstring& sender_name, unsigned int from_id, bool todo) {
    WhisperPopup_cat.debug() << "set_clickable(string sender_name " << from_id << " " << todo << ")" << std::endl;
    m_active = true;
    m_from_id = from_id;
    update_contents();
    __update_click_region();
}

void WhisperPopup::__update_click_region() {
    WhisperPopup_cat.debug() << "__update_click_region()" << std::endl;
    if (is_displayed() && m_active != false && m_from_id != 0) {
        float x = *new float(frame.get_x());
        if (x > 1) {
            x = 1.00;
        } else if (x < -1) {
            x = -1.00;
        }
        if (x != x) {
            x = 1.00;
        }
        float y = *new float(frame.get_y());
        if (y > 1) {
            y = 1.00;
        } else if (y < -1) {
            y = -1.00;
        }
        if (y != y) {
            y = 1.00;
        }
        float z = *new float(frame.get_z());
        if (z > 1) {
            z = 1.00;
        } else if (z < -1) {
            z = -1.00;
        }
        if (z != z) {
            z = 1.00;
        }
        float w = *new float(frame.get_w());
        if (w > 1) {
            w = 1.00;
        } else if (w < -1) {
            w = -1.00;
        }
        if (w != w) {
            w = 1.00;
        }
        update_click_region(x, y, z, w);
    } else {
        return;
    }
}

void WhisperPopup::manage(MarginManager* manager) {
    WhisperPopup_cat.debug() << "manage(MarginManager manager)" << std::endl;
    MarginPopup::manage(manager);
    
    PT(GenericAsyncTask) task = new GenericAsyncTask("whisper-timeout", &WhisperPopup::timeout_task, (void*)this);
    task->set_delay(10);
    g_task_mgr->add(task);
}

bool WhisperPopup::is_displayed() {
    WhisperPopup_cat.debug() << "is_displayed()" << std::endl;
    return (m_assigned_cell != nullptr && m_assigned_cell != NULL);;
}

AsyncTask::DoneStatus WhisperPopup::timeout_task(GenericAsyncTask* task, void* data) {
    WhisperPopup_cat.debug() << "timeout_task(GenericAsyncTask task, void data)" << std::endl;
    ((WhisperPopup*)data)->unmanage(((WhisperPopup*)data)->m_manager);
    return AsyncTask::DS_done;
}