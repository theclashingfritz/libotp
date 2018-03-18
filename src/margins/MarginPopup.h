#pragma once

#include "util.h"
#include "MarginManager.h"

#include <pandabase.h>
#include <typedWritable.h>
#include <notifyCategoryProxy.h>
#include <pandaNode.h>

NotifyCategoryDecl(MarginPopup, EXPCL_LIBOTP, EXPTP_LIBOTP);

class MarginCell; 

class MarginPopup : public virtual PandaNode {
  
    PUBLISHED:
        MarginPopup();
        virtual ~MarginPopup();

        void set_visible(bool flag);
        bool get_visible();
        
        void set_priority(int priority);
        int get_priority();
        
        virtual bool is_displayed();
        
        virtual void margin_visibility_changed()=0;
        
        virtual void manage(MarginManager* manager);
        virtual void unmanage(MarginManager* manager);
        
        INLINE void ref() const;
        
    public:    
        void set_last_cell(MarginCell* cell);
        MarginCell* get_last_cell();
        
        void set_assigned_cell(MarginCell* cell);
        MarginCell* get_assigned_cell();

    protected:
        MarginCell* m_last_cell;
        MarginCell* m_assigned_cell;
        
        MarginManager* m_manager;
        int m_priority;
        bool m_visible;
        
    private:
        mutable AtomicAdjust::Integer _ref_count;
        
   TYPE_HANDLE(MarginPopup, PandaNode);
};