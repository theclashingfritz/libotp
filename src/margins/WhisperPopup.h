#pragma once

#include "util.h"
#include "NametagGlobals.h"
#include "ClickablePopup.h"
#include "MarginPopup.h"

#include <pandabase.h>
#include <pointerTo.h>
#include <textFont.h>
#include <genericAsyncTask.h>
#include <notifyCategoryProxy.h>

NotifyCategoryDecl(WhisperPopup, EXPCL_LIBOTP, EXPTP_LIBOTP);

class MarginManager;

class WhisperPopup : public virtual ClickablePopup, public virtual MarginPopup {
    PUBLISHED:
        WhisperPopup(const std::wstring& text, PT(TextFont) font, const unsigned int whisper_type, const float timeout=10);
        ~WhisperPopup();
        
        void update_contents();
        void set_clickable(const std::wstring& sender_name, unsigned int from_id, bool todo);
        void manage(MarginManager* manager);
        
        virtual bool is_displayed();
        
    private:
        static AsyncTask::DoneStatus timeout_task(GenericAsyncTask* task, void* data);
        void __update_click_region();
    
        bool m_active;
        const std::wstring& m_text;
        PT(TextFont) m_font;
        const unsigned int m_whisper_type;
        int m_from_id;
        const float m_timeout;
        NodePath m_inner_np;
        LVecBase4f frame;
        
    TYPE_HANDLE(WhisperPopup, TypedObject);
};