#pragma once

#include "util.h"
#include <pandabase.h>
#include <nodePath.h>
#include <pandaNode.h>
#include <notifyCategoryProxy.h>
#include <typedObject.h>

NotifyCategoryDecl(MarginCell, EXPCL_LIBOTP, EXPTP_LIBOTP);

class MarginManager; 
class MarginPopup;

class MarginCell : public NodePath, public virtual TypedObject {
    PUBLISHED:
        MarginCell(MarginManager* manager);
        virtual ~MarginCell();
        
    public:
        void set_available(bool available);
        void set_content(MarginPopup* content);
        void set_content_nodepath(PandaNode* path);
        
        MarginPopup* get_content();
        
        bool get_available();
        bool has_content();
        bool is_available();
        bool is_free();
        
    private:
        bool m_available;
        MarginPopup* m_content;
        MarginManager* m_manager;
        NodePath m_content_np;
        
    TYPE_HANDLE(MarginCell, TypedObject);
};