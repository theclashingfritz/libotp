#include "config_libotp.h"

#include "CMover.h"
#include "CMoverGroup.h"
#include "CImpulse.h"
#include "ClickablePopup.h"
#include "MarginCell.h"
#include "MarginManager.h"
#include "MarginPopup.h"
#include "ChatBalloon.h"
#include "NametagGlobals.h"
#include "NametagGroup.h"
#include "Nametag.h"
#include "Settings.h"

Configure(config_libotp);
NotifyCategoryDef(libotp, "");

ConfigureFn(config_libotp) {
    init_libotp();
}

void init_libotp() {
    static bool initialized = false;
    if (initialized)
        return;

    initialized = true;
    
    CMover::init_type();
    CMoverGroup::init_type();
    CImpulse::init_type();
    ClickablePopup::init_type();
    MarginCell::init_type();
    MarginManager::init_type();
    MarginPopup::init_type();
    ChatBalloon::init_type();
    NametagGlobals::init_type();
    NametagGroup::init_type();
    Nametag::init_type();
    Settings::init_type();
}