#pragma once

#include "util.h"
#include "MarginCell.h"
#include "MarginPopup.h"

#include <pandabase.h>
#include <pandaNode.h>
#include <nodePath.h>

typedef pvector<MarginCell*> cell_vec_t;
typedef pvector<MarginPopup*> popup_vec_t;

class MarginManager : public PandaNode {
    PUBLISHED:
        MarginManager();
        ~MarginManager();
        
        MarginCell* add_grid_cell(float x, float y, float left, float right, float bottom, float top, const NodePath& parent, float aspect_ratio);
        MarginCell* add_grid_cell(float x, float y, float left, float right, float bottom, float top);
        MarginCell* add_cell(float left, float right, float bottom, float top);
        MarginCell* add_cell(float x, float y, const NodePath& parent);

        void set_cell_available(int cell_index, bool available);
        void set_cell_available(MarginCell* cell, bool available);
        void remove_cell(int cell_index);
        void remove_cell(MarginCell* cell);
        
        void add_visible_popup(MarginPopup* popup);
        void remove_visible_popup(MarginPopup* popup);
        
        bool get_cell_available(int cell_index);
        bool get_cell_available(MarginCell* cell);
        
    public:
        void reorganize();
        
    private: 
        static bool _sort_key(MarginPopup* lhs, MarginPopup* rhs);
        cell_vec_t m_cells;
        popup_vec_t m_popups;


    TYPE_HANDLE(MarginManager, TypedObject);
};