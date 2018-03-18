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
#include <mouseWatcherRegion.h>

NotifyCategoryDecl(PopupMouseWatcherRegion, EXPCL_LIBOTP, EXPTP_LIBOTP);

class PopupMouseWatcherRegion;

class PopupMouseWatcherRegion : public virtual MouseWatcherRegion {
    public:
        PopupMouseWatcherRegion(ClickablePopup *popup, const std::string &name, const LVecBase4f &frame);
        PopupMouseWatcherRegion(const PopupMouseWatcherRegion& region);
        ~PopupMouseWatcherRegion();
        
        virtual void enter_region(MouseWatcherParameter param);
        virtual void exit_region(MouseWatcherParameter param);
        virtual void press(MouseWatcherParameter param);
        virtual void release(MouseWatcherParameter param);
        
    private:
        std::string _name;
        ClickablePopup *_popup;
        LVecBase4f _frame;
        
    TYPE_HANDLE(PopupMouseWatcherRegion, MouseWatcherRegion);
};