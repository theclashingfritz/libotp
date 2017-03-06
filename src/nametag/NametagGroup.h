#pragma once

#include "util.h"
#include "Nametag.h"
#include "Nametag2d.h"
#include "Nametag3d.h"
#include "NametagGlobals.h"

#include <luse.h>
#include <nodepath.h>
#include <namable.h>
#include <genericAsyncTask.h>
#include <typedReferenceCount.h>
#include <pandabase.h>

class NametagGroup;
class MarginManager;

typedef pvector<Nametag*> nametag_vec_t;
typedef pvector<std::wstring> chat_page_vec_t;


class EXPCL_LIBOTP NametagGroup : public virtual TypedReferenceCount {
    PUBLISHED:
        NametagGroup();
        ~NametagGroup();
        
        void destroy();
        void add_nametag(Nametag* nametag);
        void remove_nametag(Nametag* nametag);
        void set_page_number(int page);
        void set_avatar(NodePath* node);
        void set_font(PT(TextFont) font);
        void set_chat_font(PT(TextFont) font);
        void set_name(const std::wstring& name);
        void set_name_wordwrap(float wordwrap);
        void set_chat(const std::wstring& chat_string, int chat_flags);
        void set_contents(int contents);
        void set_display_name(const std::wstring& name);
        void set_qt_color(LVecBase4f color);
        void set_color_code(NametagGlobals::ColorCode cc);
        void set_active(bool active);
        void manage(MarginManager* manager);
        void unmanage(MarginManager* manager);
        void clear_shadow();
        void clear_chat();
        
        unsigned int get_num_chat_pages();
        
        const std::wstring get_chat();
        const std::wstring get_stomp_text();
        
        const std::string get_unique_id();
        
        //virtual Nametag2d* get_nametag_2d();  //TODO: Figure out why this causes a compilation error (Besides the fact i removed the function temporary.)
        virtual Nametag3d* get_nametag_3d();
        
        PT(PandaNode) get_name_icon();
        
        NodePath* get_avatar();
        
        float get_stomp_delay();
        
        bool has_button();
        bool get_chat_stomp();
        bool is_active();
        
    private:
        buttons_map_t get_buttons();
        
        void update_nametag(Nametag* nametag);
        void update_tags();
        void update_chat(const std::wstring& chat_string, int chat_flags);
        void start_chat_timeout();
        void stop_chat_timeout();
        
        AsyncTask::DoneStatus update_stomp();
        AsyncTask::DoneStatus do_chat_timeout();
        AsyncTask::DoneStatus tick();
        
        static AsyncTask::DoneStatus update_stomp_task(GenericAsyncTask* task, void* data);
        static AsyncTask::DoneStatus do_chat_timeout_task(GenericAsyncTask* task, void* data);
        static AsyncTask::DoneStatus tick_task(GenericAsyncTask* task, void* data);
        
        Nametag2d* m_nametag_2d;
        Nametag3d* m_nametag_3d;
        
        PT(PandaNode) m_icon;
        
        PT(AsyncTask) m_chat_timeout_task;
        PT(AsyncTask) m_tick_task;
        PT(AsyncTask) m_stomp_task;
        
        PT(TextFont) m_font;
        
        LVecBase4f m_qt_color;
        
        std::wstring m_name;
        std::wstring m_display_name;
        std::wstring m_stomp_text;
        
        chat_page_vec_t m_chat_pages;
        
        MarginManager* m_manager;
        
        nametag_vec_t m_nametags;
        
        int m_chat_page;
        int m_chat_flags;
        int m_stomp_flags;
        
        unsigned int m_serial;
        
        static unsigned int NametagGroup::NametagGroup_serial;
        
        NametagGlobals::ColorCode m_color_code;
    
        NodePath* m_avatar; 
        
        bool m_active;
        bool m_visible_3d;

    TYPE_HANDLE(NametagGroup, TypedObject);
};