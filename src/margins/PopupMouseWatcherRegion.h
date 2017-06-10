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
        PopupMouseWatcherRegion(ClickablePopup *popup, const string &name, const LVecBase4 &frame);
        PopupMouseWatcherRegion(const PopupMouseWatcherRegion& region);
        ~PopupMouseWatcherRegion();
        
    TYPE_HANDLE(PopupMouseWatcherRegion, TypedObject);
};