#include "WhisperPopup.h"

#include <asyncTaskManager.h>
 
static PT(AsyncTaskManager) g_task_mgr = AsyncTaskManager::get_global_ptr(); 

NotifyCategoryDef(WhisperPopup, "");

TypeHandle WhisperPopup::_type_handle;

WhisperPopup::WhisperPopup(const std::wstring& text, PT(TextFont) font, const unsigned int whisper_type, const float timeout): ClickablePopup(), MarginPopup(), m_text(text), m_font(font), m_whisper_type(whisper_type), m_timeout(timeout) {
    WhisperPopup_cat.debug() << "__init__(" << text << " " << "TextFont font" << " " << "NametagGlobals::WhisperType whisper_type" << " " << timeout << ")" << std::endl;
    m_inner_np = NodePath::any_path(this).attach_new_node("inner_np");
    m_inner_np.set_scale(.25);
    
    update_contents();
    set_priority(2);
    set_visible(true);
}

WhisperPopup::~WhisperPopup() {
}

void WhisperPopup::update_contents() {
    WhisperPopup_cat.debug() << "update_contents()" << std::endl;
    unsigned int cc;
    
    if (NametagGlobals::does_whisper_type_exist(m_whisper_type))
        cc = m_whisper_type;
    else
        cc = NametagGlobals::WTSystem;
        
    color_tuple_t colors = NametagGlobals::get_whisper_colors(cc, CLICKSTATE_NORMAL);
    
    NodePath balloon = NametagGlobals::speech_balloon_2d->generate(m_text, m_font, &colors[0], &colors[1], 7.5);
    balloon.reparent_to(m_inner_np);
    
    NodePath text_np = balloon.find("**/+TextNode");
    PT(TextNode) tn = DCAST(TextNode, text_np.node());
    LVecBase4f frame = tn->get_frame_actual();
    LPoint3f center = m_inner_np.get_relative_point(text_np, LPoint3f((frame.get_x() + frame.get_y()) / 2., 0, (frame.get_z() + frame.get_w()) / 2.));
    
    balloon.set_pos(balloon, -center);
}

void WhisperPopup::set_clickable(const std::wstring& sender_name, unsigned int from_id, bool) {
    // To do
}

void WhisperPopup::manage(MarginManager* manager) {
    WhisperPopup_cat.debug() << "manage(MarginManager manager)" << std::endl;
    MarginPopup::manage(manager);
    
    PT(GenericAsyncTask) task = new GenericAsyncTask("whisper-timeout", &WhisperPopup::timeout_task, (void*)this);
    task->set_delay(10);
    g_task_mgr->add(task);
}

AsyncTask::DoneStatus WhisperPopup::timeout_task(GenericAsyncTask* task, void* data) {
    WhisperPopup_cat.debug() << "timeout_task(GenericAsyncTask task, void data)" << std::endl;
    ((WhisperPopup*)data)->unmanage(((WhisperPopup*)data)->m_manager);
    return AsyncTask::DS_done;
}