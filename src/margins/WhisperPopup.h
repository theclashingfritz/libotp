#pragma once

#include "util.h"
#include "NametagGlobals.h"
#include "ClickablePopup.h"
#include "MarginPopup.h"

#include <pandabase.h>
#include <pointerTo.h>
#include <textFont.h>
#include <genericAsyncTask.h>

class MarginManager;

class WhisperPopup : public virtual ClickablePopup, public virtual MarginPopup {
    PUBLISHED:
        WhisperPopup(const std::wstring& text, PT(TextFont) font, const NametagGlobals::WhisperType whisper_type, const float timeout=10);
        ~WhisperPopup();
        
        void update_contents();
        void set_clickable(const std::wstring& sender_name, unsigned int from_id, bool);
        void manage(MarginManager* manager);
        
    private:
        static AsyncTask::DoneStatus timeout_task(GenericAsyncTask* task, void* data);
        const std::wstring& m_text;
        PT(TextFont) m_font;
        const NametagGlobals::WhisperType m_whisper_type;
        const float m_timeout;
        NodePath m_inner_np;
        
    TYPE_HANDLE(WhisperPopup, TypedObject);
};