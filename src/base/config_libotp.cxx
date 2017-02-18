#include "config_libotp.h"

#include "CMover.h"
#include "CImpulse.h"
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
    CImpulse::init_type();
    NametagGlobals::init_type();
    NametagGroup::init_type();
    Nametag::init_type();
    Settings::init_type();
}