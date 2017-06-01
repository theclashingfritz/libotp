#include "NametagGroup.h"
#include "Nametag2d.h"
#include "Nametag3d.h"

#include <asyncTaskManager.h>
#include <boundingBox.h>
#include <boundingHexahedron.h>

static PT(AsyncTaskManager) g_task_mgr = AsyncTaskManager::get_global_ptr(); 

NotifyCategoryDef(NametagGroup, "");

TypeHandle NametagGroup::_type_handle;
unsigned int NametagGroup::NametagGroup_serial;

NametagGroup::NametagGroup() : m_icon(new PandaNode("icon")), m_chat_timeout_task(nullptr), m_stomp_flags(0), m_color_code(NametagGlobals::CCNormal), m_chat_flags(0), m_font(nullptr), m_manager(nullptr), m_avatar(nullptr), m_qt_color(LVecBase4f(1)), m_active(true), m_chat_page(0), m_visible_3d(true), m_stomp_task(nullptr) {
    NametagGroup_cat.debug() << "__init__()" << std::endl;
    m_nametag_2d = new Nametag2d();
    m_nametag_3d = new Nametag3d();
    add_nametag(m_nametag_2d);
    add_nametag(m_nametag_3d);
    
    m_serial = NametagGroup::NametagGroup_serial++;
    
    m_tick_task = new GenericAsyncTask(get_unique_id(), &NametagGroup::tick_task, (void*)this);
    m_tick_task->set_sort(45);
    g_task_mgr->add(m_tick_task);
}

NametagGroup::~NametagGroup() {
    if (m_icon != NULL && m_icon != nullptr) {
        delete m_icon;
        m_icon = nullptr;
    }
    if (m_avatar != NULL && m_avatar != nullptr) {
        m_avatar = nullptr;
    }
    if (m_nametag_2d != NULL && m_nametag_2d != nullptr) {
        delete m_nametag_2d;
        m_nametag_2d = nullptr;
    }
    if (m_nametag_3d != NULL && m_nametag_3d != nullptr) {
        delete m_nametag_3d;
        m_nametag_3d = nullptr;
    }
    if (m_tick_task != NULL && m_tick_task != nullptr) {
        delete m_tick_task;
        m_tick_task = nullptr;
    }
    if (m_stomp_task != NULL && m_stomp_task != nullptr) {
        delete m_stomp_task;
        m_stomp_task = nullptr;
    }
    if (m_chat_timeout_task != NULL && m_chat_timeout_task != nullptr) {
        delete m_chat_timeout_task;
        m_chat_timeout_task = nullptr;
    }
}

void NametagGroup::destroy() {
    NametagGroup_cat.debug() << "destory()" << std::endl;
    if (m_tick_task != NULL && m_tick_task != nullptr) {
        m_tick_task->remove();
        m_tick_task = nullptr;
    }
    
    if (m_stomp_task != NULL && m_stomp_task != nullptr) {
        m_stomp_task->remove();
        m_stomp_task = nullptr;
    }
    
    if (m_manager != NULL && m_manager != nullptr) {
        unmanage(m_manager);
    }
    
    for (nametag_vec_t::iterator it = m_nametags.begin(); it != m_nametags.end(); ++it) {
        remove_nametag(*it);
    }
        
    m_nametags.clear();
}

void NametagGroup::set_avatar(NodePath* node) {
    NametagGroup_cat.debug() << "set_avatar(NodePath node)" << std::endl;
    m_avatar = node;
}

void NametagGroup::set_active(bool active) {
    NametagGroup_cat.debug() << "set_active(" << active << ")" << std::endl;
    m_active = active;
}

void NametagGroup::set_page_number(int page) {
    NametagGroup_cat.debug() << "set_page_number(" << page << ")" << std::endl;
    m_chat_page = page;
    update_tags();
}

void NametagGroup::set_font(PT(TextFont) font) {
    NametagGroup_cat.debug() << "set_font(TextFont font)" << std::endl;
    m_font = font;
    update_tags();
}

void NametagGroup::set_chat_font(PT(TextFont) font) {
    NametagGroup_cat.debug() << "set_chat_font(TextFont font)" << std::endl;
    m_font = font;
    update_tags();
}

void NametagGroup::set_color_code(unsigned int cc) {
    NametagGroup_cat.debug() << "set_color_code(" << cc << ")" << std::endl;
    m_color_code = cc;
    update_tags();
}

void NametagGroup::set_name(const std::wstring& name) {
    NametagGroup_cat.debug() << "set_name(wstring name)" << std::endl;
    m_name = name;
    update_tags();
}

void NametagGroup::set_display_name(const std::wstring& name) {
    NametagGroup_cat.debug() << "set_display_name(wstring name)" << std::endl;
    m_display_name = name;
    update_tags();
}

void NametagGroup::set_qt_color(LVecBase4f color) {
    NametagGroup_cat.debug() << "set_qt_color(LVecBase4f color)" << std::endl;
    m_qt_color = color;
    update_tags();
}

void NametagGroup::set_contents(int contents) {
    NametagGroup_cat.debug() << "set_contents(" << contents << ")" << std::endl;
    for (nametag_vec_t::iterator it = m_nametags.begin(); it != m_nametags.end(); ++it) {
        (*it)->set_contents(contents);
    }
}

void NametagGroup::add_nametag(Nametag* nametag) {
    NametagGroup_cat.debug() << "add_nametag(Nametag nametag)" << std::endl;
    if (nametag == NULL || nametag == nullptr) {
        return;
    }
    
    nametag->set_group(this);
    m_nametags.push_back(nametag);
    update_nametag(nametag);
    
    if (m_manager != NULL && m_manager != nullptr && nametag->is_of_type(Nametag2d::get_class_type())) {
        nametag->manage(m_manager);
    }
}

void NametagGroup::remove_nametag(Nametag* nametag) {
    NametagGroup_cat.debug() << "remove_nametag(Nametag nametag)" << std::endl;
    if (nametag == NULL || nametag == nullptr) {
        return;
    }
    
    nametag_vec_t::iterator it = std::find(m_nametags.begin(), m_nametags.end(), nametag);
    if (it != m_nametags.end()) {
        m_nametags.erase(it);
        
        if (m_manager != NULL && m_manager != nullptr && nametag->is_of_type(Nametag2d::get_class_type())) {
            nametag->unmanage(m_manager);
        }
            
        nametag->clear_group();
        nametag->destroy();
    }
}

void NametagGroup::manage(MarginManager* manager) {
    NametagGroup_cat.debug() << "manage(MarginManager manager)" << std::endl;
    if (manager == NULL || manager == nullptr) {
        return;
    }
    
    m_manager = manager;
    for (nametag_vec_t::iterator it = m_nametags.begin(); it != m_nametags.end(); ++it) {
        Nametag* nametag = *it;
        if (nametag->is_of_type(Nametag2d::get_class_type()))
            nametag->manage(manager);
    }
}

void NametagGroup::unmanage(MarginManager* manager) {
    NametagGroup_cat.debug() << "unmanage(MarginManager manager)" << std::endl;
    if (manager == NULL || manager == nullptr) {
        return;
    }
    
    m_manager = nullptr;
    for (nametag_vec_t::iterator it = m_nametags.begin(); it != m_nametags.end(); ++it) {
        Nametag* nametag = *it;
        if (nametag->is_of_type(Nametag2d::get_class_type()))
            nametag->unmanage(manager);
    }
}

void NametagGroup::set_chat(const std::wstring& chat_string, int chat_flags) {
    NametagGroup_cat.debug() << "set_chat(wstring chat_string " << chat_flags << ")" << std::endl;
    if (!(m_chat_flags & CFSpeech)) {
        update_chat(chat_string, chat_flags);
    } else {
        clear_chat();
        m_stomp_text = chat_string;
        m_stomp_flags = chat_flags;
        m_stomp_task = new GenericAsyncTask(get_unique_id(), &NametagGroup::update_stomp_task, (void*)this);
        m_stomp_task->set_delay(get_stomp_delay());
        g_task_mgr->add(m_stomp_task);
    }
}

void NametagGroup::update_chat(const std::wstring& chat_string, int chat_flags) {
    NametagGroup_cat.debug() << "update_chat(wstring chat_string " << chat_flags << ")" << std::endl;
    m_chat_pages.clear();
    m_chat_flags = 0;
    
    if (chat_string.size()) {
        m_chat_flags = chat_flags;
        
        // Split the string
        const wchar_t* begin;
        const wchar_t* str = chat_string.c_str();
        
        do {
            begin = str;

            while(*str != '\x07' && *str) {
                str++;
            }

            m_chat_pages.push_back(std::wstring(begin, str));
        } while (*str++ != NULL);
    }
    
    set_page_number(0);
    
    stop_chat_timeout();
    if (chat_flags & NametagGlobals::CFTimeout) {
        start_chat_timeout();
    }
}

void NametagGroup::start_chat_timeout() {
    NametagGroup_cat.debug() << "start_chat_timeout()" << std::endl;
    float timeout = get_chat().size() / 2.;
    timeout = timeout < 4 ? 4 : timeout;
    timeout = timeout > 12 ? 12 : timeout;
    
    m_chat_timeout_task = new GenericAsyncTask(get_unique_id(), &NametagGroup::do_chat_timeout_task, (void*)this);
    m_chat_timeout_task->set_delay(timeout);
    g_task_mgr->add(m_chat_timeout_task);
}

void NametagGroup::stop_chat_timeout() {
    NametagGroup_cat.debug() << "stop_chat_timeout()" << std::endl;
    if (m_chat_timeout_task != NULL && m_chat_timeout_task != nullptr) {
        m_chat_timeout_task->remove();
        m_chat_timeout_task = nullptr;
    }
}

void NametagGroup::update_nametag(Nametag* nametag) {
    NametagGroup_cat.debug() << "update_nametag(Nametag nametag)" << std::endl;
    if (nametag == NULL || nametag == nullptr) {
        return;
    }
    
    nametag->m_font = m_font;
    nametag->m_name = m_name;
    nametag->m_display_name = m_display_name;
    nametag->m_qt_color = m_qt_color;
    nametag->m_color_code = m_color_code;
    nametag->m_chat_string = get_chat();
    nametag->m_buttons = get_buttons();
    nametag->m_chat_flags = m_chat_flags;
    nametag->m_avatar = m_avatar;
    nametag->m_icon = NodePath(m_icon);
    
    if (m_active || has_button()) {
        nametag->set_click_region_event(get_unique_id());
    } else {
        nametag->set_click_region_event("");
    }
        
    nametag->update();
}

void NametagGroup::update_tags() {
    NametagGroup_cat.debug() << "update_tags()" << std::endl;
    for (nametag_vec_t::iterator it = m_nametags.begin(); it != m_nametags.end(); ++it) {
        update_nametag(*it);
    }
}

void NametagGroup::set_name_wordwrap(float wordwrap) {
    NametagGroup_cat.debug() << "set_name_wordwrap(" << wordwrap << ")" << std::endl;
}

void NametagGroup::clear_shadow() {
    NametagGroup_cat.debug() << "clear_shadow()" << std::endl;
} 

void NametagGroup::clear_chat() {
    NametagGroup_cat.debug() << "clear_chat()" << std::endl;
    const std::wstring empty;
    update_chat(empty, 0);
    if (m_stomp_task != NULL && m_stomp_task != nullptr) {
        m_stomp_task->remove();
        m_stomp_task = nullptr;
    }
}

unsigned int NametagGroup::get_num_chat_pages() {
    NametagGroup_cat.debug() << "get_num_chat_pages()" << std::endl;
    if (!(m_chat_flags & (CFSpeech | CFThought))) {
        return 0;
    }
        
    return m_chat_pages.size();
}

float NametagGroup::get_stomp_delay() {
    NametagGroup_cat.debug() << "get_stomp_delay()" << std::endl;
    return .2;
};

Nametag3d* NametagGroup::get_nametag_3d() {
    NametagGroup_cat.debug() << "get_nametag_3d()" << std::endl;
    return m_nametag_3d;
}

Nametag2d* NametagGroup::get_nametag_2d() {
    NametagGroup_cat.debug() << "get_nametag_2d()" << std::endl;
    return m_nametag_2d;
}

PT(PandaNode) NametagGroup::get_name_icon() {
    NametagGroup_cat.debug() << "get_name_icon()" << std::endl;
    return m_icon;
}

buttons_map_t NametagGroup::get_buttons() {
    NametagGroup_cat.debug() << "get_buttons()" << std::endl;
    buttons_map_t empty;
    
    if (get_num_chat_pages() < 2) {
        if (m_chat_flags & CFPageButton) {
            return NametagGlobals::page_buttons;
        } else if (m_chat_flags & CFQuitButton) {
            return NametagGlobals::quit_buttons;
        } else {
            return empty;
        }
    } else if (m_chat_page == get_num_chat_pages() - 1) {
        return (m_chat_flags & NametagGlobals::CFNoQuitButton) ? empty : NametagGlobals::quit_buttons;
    }
        
    return (m_chat_flags & NametagGlobals::CFPageButton) ? NametagGlobals::page_buttons : empty;
}

const std::wstring NametagGroup::get_chat() {
    NametagGroup_cat.debug() << "get_chat()" << std::endl;
    std::wstring empty;
    if (m_chat_page >= m_chat_pages.size()) {
        return empty;
    }
        
    return m_chat_pages.at(m_chat_page);
}

const std::wstring NametagGroup::get_stomp_text() {
    NametagGroup_cat.debug() << "get_stomp_text()" << std::endl;
    return m_stomp_text;
}

const std::string NametagGroup::get_unique_id() {
    NametagGroup_cat.debug() << "get_unique_id()" << std::endl;
    std::string s = "Nametag-";
    s += m_serial;
    return s;
}

NodePath* NametagGroup::get_avatar() {
    NametagGroup_cat.debug() << "get_avatar()" << std::endl;
    return m_avatar;
}

bool NametagGroup::is_active() {
    NametagGroup_cat.debug() << "is_active()" << std::endl;
    return m_active;
}

bool NametagGroup::get_chat_stomp() {
    NametagGroup_cat.debug() << "get_chat_stomp()" << std::endl;
    return (m_stomp_task != NULL && m_stomp_task != nullptr);
}


bool NametagGroup::has_button() {
    NametagGroup_cat.debug() << "has_button()" << std::endl;
    return get_buttons().size() > 0;
}

AsyncTask::DoneStatus NametagGroup::update_stomp() {
    NametagGroup_cat.spam() << "update_stomp()" << std::endl;
    update_chat(m_stomp_text, m_stomp_flags);
    m_stomp_task = nullptr;
    return AsyncTask::DS_done;
}

AsyncTask::DoneStatus NametagGroup::do_chat_timeout() {
    NametagGroup_cat.spam() << "do_chat_timeout()" << std::endl;
    const std::wstring empty;
    update_chat(empty, 0);
    return AsyncTask::DS_done;
}

AsyncTask::DoneStatus NametagGroup::tick() {
    NametagGroup_cat.spam() << "tick()" << std::endl;
    for (nametag_vec_t::iterator it = m_nametags.begin(); it != m_nametags.end(); ++it) {
        (*it)->tick();
    }
        
    if (m_avatar == NULL || m_avatar == nullptr) {
        return AsyncTask::DS_cont;
    } else if (m_avatar->is_empty()) {
        return AsyncTask::DS_cont;
    }
            
    bool visible_3d;
    
    if (m_avatar->is_hidden()) {
        visible_3d = false;
    } else if (NametagGlobals::m_force_onscreen_chat && m_chat_flags & NametagGlobals::CFSpeech) {
        visible_3d = false;
    } else {
        if (!NametagGlobals::m_camera_nodepath->is_empty()) {
            LPoint3f min_corner, max_corner;
            m_avatar->calc_tight_bounds(min_corner, max_corner);
            PT(BoundingBox) avatar_bounds = new BoundingBox(min_corner, max_corner);
            PT(BoundingHexahedron) camera_bounds = DCAST(BoundingHexahedron, DCAST(Camera, NametagGlobals::m_camera_nodepath->node())->get_lens()->make_bounds());
            camera_bounds->xform(NametagGlobals::m_camera_nodepath->get_mat(m_avatar->get_parent()));
            visible_3d = (camera_bounds->contains(avatar_bounds) & BoundingVolume::IF_some);
        } else {
            visible_3d = false;
        }
    }
    
    if (visible_3d != m_visible_3d) {
        m_visible_3d = visible_3d;
        for (nametag_vec_t::iterator it = m_nametags.begin(); it != m_nametags.end(); ++it) {
            Nametag* nametag = *it;
            nametag->tick();
            if (nametag->is_of_type(Nametag2d::get_class_type()))
                nametag->set_visible(!visible_3d);
        }
    }
    
    return AsyncTask::DS_cont;
}

AsyncTask::DoneStatus NametagGroup::update_stomp_task(GenericAsyncTask* task, void* data) {
    return ((NametagGroup*)data)->update_stomp();
}

AsyncTask::DoneStatus NametagGroup::do_chat_timeout_task(GenericAsyncTask* task, void* data) {
    return ((NametagGroup*)data)->do_chat_timeout();
}

AsyncTask::DoneStatus NametagGroup::tick_task(GenericAsyncTask* task, void* data) {
    return ((NametagGroup*)data)->tick();
}