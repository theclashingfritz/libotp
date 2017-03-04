#pragma once

#include "util.h"
#include <pandabase.h>
#include <nodePath.h>
#include <typedObject.h>

class MarginManager; 
class MarginPopup; 

class MarginCell : public NodePath, public TypedObject {
    PUBLISHED:
        MarginCell(MarginManager* manager);
        ~MarginCell();
        
    public:
        void set_available(bool available);
        bool get_available();
        
        void set_content(MarginPopup* content);
        bool has_content();
        MarginPopup* get_content();
        
        bool is_available();
        bool is_free();
        
    private:
        bool m_available;
        MarginPopup* m_content;
        MarginManager* m_manager;
        NodePath m_content_np;
        
    TYPE_HANDLE(MarginCell, TypedObject);
};