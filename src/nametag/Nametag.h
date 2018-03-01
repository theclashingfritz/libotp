#pragma once

#include "util.h"

#include "ClickablePopup.h"
#include "NametagGlobals.h"

#include <nodepath.h>
#include <pandanode.h>
#include <referenceCount.h>
#include <textNode.h>
#include <textFont.h>
#include <notifyCategoryProxy.h>
#include <mouseWatcher.h>
#include <pandabase.h>

NotifyCategoryDecl(Nametag, EXPCL_LIBOTP, EXPTP_LIBOTP);

class Nametag;
class ChatBalloon;
class NametagGroup; 
class MarginManager;

class EXPCL_LIBOTP Nametag : public virtual ClickablePopup, public virtual ReferenceCount {
    
    friend class NametagGroup;
    
    PUBLISHED:
        enum { 
            CName = 1,
            CSpeech = 2,
            CThought = 4
        };
        
        Nametag(bool is_3d=false);
        ~Nametag();
        
        Nametag(const Nametag& tag);
        
        void operator =(const Nametag& tag);
        bool operator !=(const Nametag& tag);
        bool operator ==(const Nametag& tag);

        void set_draw_order(uint8_t draw_order);
        void clear_draw_order();
        void set_contents(int contents);
        void set_active(bool active);
        void set_chat_wordwrap(uint16_t chat_wordwrap);
        void set_avatar(NodePath *avatar);
        void clear_avatar();
        
        void manage(MarginManager* manager);
        void unmanage(MarginManager* manager);
        void set_visible(bool flag);

        uint8_t get_draw_order();
        unsigned int get_contents();
        bool get_active();
        bool has_group();
        uint16_t get_chat_wordwrap();
        NametagGroup *get_group();
        NodePath *get_avatar();

    public:
        virtual void destroy();
        virtual void tick()=0;
        virtual void update_contents();
        
        std::wstring get_display_name();
        
        static const float name_padding;
        static const float chat_alpha;
        
    protected:
        virtual void show_balloon(ChatBalloon* balloon, const std::wstring& text);
        virtual void set_group(NametagGroup* group);
        virtual void click_state_changed();
        virtual void show_thought();
        virtual void clear_group();
        virtual void show_speech();
        virtual void show_name();
        virtual void update();
        
        std::wstring get_name();
        
        virtual ChatBalloon* get_speech_balloon()=0;
        virtual ChatBalloon* get_thought_balloon()=0;
        
        NodePath get_button();
        
        uint8_t m_draw_order;
        
        int m_contents;
        int m_chat_flags;
        
        float m_wordwrap;
        float m_chat_wordwrap;
        
        bool m_active;
        bool m_has_group;
        bool m_is_3d;
        
        PT(TextFont) m_font;
        
        LVecBase4f m_name_fg;
        LVecBase4f m_name_bg;
        LVecBase4f m_chat_fg;
        LVecBase4f m_chat_bg;
        LVecBase4f m_qt_color;
        
        unsigned int m_color_code;
        unsigned int m_serial;
        
        NametagGroup *m_group;
        
        NodePath *m_avatar;
        NodePath m_inner_np;
        NodePath m_icon;
        
        LVecBase4f frame;
        
        buttons_map_t m_buttons;
        
        std::wstring m_name;
        std::wstring m_display_name;
        std::wstring m_chat_string;

    private:
        static unsigned int Nametag_serial;
        //PandaNode m_pandaNode;
};
