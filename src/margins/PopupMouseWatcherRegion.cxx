#include "PopupMouseWatcherRegion.h"

#include <mouseWatcherParameter.h>

TypeHandle PopupMouseWatcherRegion::_type_handle;

NotifyCategoryDef(PopupMouseWatcherRegion, "");

PopupMouseWatcherRegion::PopupMouseWatcherRegion(ClickablePopup *popup, const std::string &name, const LVecBase4f &frame) : MouseWatcherRegion(name, frame) {
    PopupMouseWatcherRegion_cat.debug() << "__init__(ClickablePopup popup " << name << " LVecBase4 frame)" << std::endl;
    
    _popup = popup;
    _name = std::string(name);
}

PopupMouseWatcherRegion::PopupMouseWatcherRegion(const PopupMouseWatcherRegion& region) : MouseWatcherRegion(std::string(region._name), LVecBase4f(region._frame)) {
    PopupMouseWatcherRegion_cat.debug() << "__init__(const PopupMouseWatcherRegion& region)" << std::endl;
    
    _popup = region._popup;
    _name = std::string(region._name);
}

PopupMouseWatcherRegion::~PopupMouseWatcherRegion() {
    
}

void PopupMouseWatcherRegion::enter_region(MouseWatcherParameter param) {
    _popup->enter_region(param);
}

void PopupMouseWatcherRegion::exit_region(MouseWatcherParameter param) {
    _popup->exit_region(param);
}

void PopupMouseWatcherRegion::press(MouseWatcherParameter param) {
    _popup->press(param);
}

void PopupMouseWatcherRegion::release(MouseWatcherParameter param) {
    _popup->release(param);
}