#pragma once

#include "util.h"
#include "MarginCell.h"
#include "MarginPopup.h"

#include <pandabase.h>
#include <pandaNode.h>
#include <nodePath.h>
#include <notifyCategoryProxy.h>

typedef pvector<MarginCell*> cell_vec_t;
typedef pvector<MarginPopup*> popup_vec_t;

NotifyCategoryDecl(MarginManager, EXPCL_LIBOTP, EXPTP_LIBOTP);

class MarginManager : public PandaNode {
    PUBLISHED:
        MarginManager();
        virtual ~MarginManager();
        
        void set_cell_available(int cell_index, bool available);
        void set_cell_available(MarginCell* cell, bool available);
        void remove_cell(int cell_index);
        void remove_cell(MarginCell* cell);
        
        void add_visible_popup(MarginPopup* popup);
        void remove_visible_popup(MarginPopup* popup);
        
        MarginCell* add_grid_cell(float x, float y, float left, float right, float bottom, float top);
        MarginCell* add_cell(float left, float right, float bottom, float top);
        
        static MarginManager* get_global_ptr();
        
        bool get_cell_available(int cell_index);
        bool get_cell_available(MarginCell* cell);
        
    public:
        void reorganize();
        
    private: 
        static bool _sort_key(MarginPopup* lhs, MarginPopup* rhs);
        static MarginManager* _global_ptr;
        cell_vec_t m_cells;
        popup_vec_t m_popups;


    TYPE_HANDLE(MarginManager, TypedObject);
};