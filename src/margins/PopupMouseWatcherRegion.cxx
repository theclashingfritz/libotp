#include "PopupMouseWatcherRegion.h"

TypeHandle PopupMouseWatcherRegion::_type_handle;

NotifyCategoryDef(PopupMouseWatcherRegion, "");

PopupMouseWatcherRegion::PopupMouseWatcherRegion(ClickablePopup *popup, const string &name, const LVecBase4 &frame) : MouseWatcherRegion(name, frame) {
    PopupMouseWatcherRegion_cat.debug() << "__init__(" << name << " LVecBase4 frame)" << std::endl;
    float value1 = *(float *)(this + 52);
    float value2 = *(float *)(this + 56);
    value1 = *(float *)&frame;
    value2 = *((float *)&frame + 4);
    
    PopupMouseWatcherRegion_cat.info() << "Value 1 is " << value1 << " which is at address " << reinterpret_cast<int>(std::addressof(value1)) << "!" << std::endl;
    PopupMouseWatcherRegion_cat.info() << "Value 2 is " << value2 << " which is at address " << reinterpret_cast<int>(std::addressof(value2)) << "!" << std::endl;
    
    *(float *)(this + 52) = *(float *)&frame;
    *(float *)(this + 56) = *((float *)&frame + 4);
    *(float *)(this + 60) = *((float *)&frame + 8);
    *(float *)(this + 64) = *((float *)&frame + 12);
    
}

PopupMouseWatcherRegion::PopupMouseWatcherRegion(const PopupMouseWatcherRegion& region) : MouseWatcherRegion("temp", LVecBase4(1, 1, 1, 1)) {
    PopupMouseWatcherRegion_cat.debug() << "__init__(const PopupMouseWatcherRegion& region)" << std::endl;
}

PopupMouseWatcherRegion::~PopupMouseWatcherRegion() {
    
}