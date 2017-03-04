#pragma once

#include "util.h"
#include "Nametag.h"
#include "MarginPopup.h"

class Nametag2d;

class EXPCL_LIBOTP Nametag2d : public virtual Nametag, public virtual MarginPopup {
    PUBLISHED:
        Nametag2d();
        ~Nametag2d();
        
    public:
        virtual void tick();
        virtual void margin_visibility_changed();
        virtual bool is_displayed();
        
        static const float scale_2d;
        static const float chat_alpha;
        static const float arrow_offset;
        static const float arrow_scale;
        
    protected:
        virtual void update();
        
        void consider_update_click_region();
        
        virtual void show_balloon(ChatBalloon* balloon, const std::wstring& text);
        virtual void show_name();
        
        virtual ChatBalloon* get_speech_balloon();
        virtual ChatBalloon* get_thought_balloon();
        
        NodePath* m_arrow;
        
    TYPE_HANDLE(Nametag2d, TypedObject);
};