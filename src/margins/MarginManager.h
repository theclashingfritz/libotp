#pragma once

#include "util.h"
#include "MarginCell.h"
#include "MarginPopup.h"

#include <pandabase.h>
#include <typedObject.h>

typedef pvector<MarginCell*> cell_vec_t;
typedef pvector<MarginPopup*> popup_vec_t;

class MarginManager : public TypedObject {
    PUBLISHED:
        MarginManager();
        ~MarginManager();
        
        MarginCell* add_grid_cell(float x, float y, float left, float right,
                                  float bottom, float top, const NodePath& parent,
                                  float aspect_ratio);
                           
        void set_cell_available(MarginCell* cell, bool available);
        void add_visible_popup(MarginPopup* popup);
        void remove_visible_popup(MarginPopup* popup);
        
    public:
        void reorganize();
        
    private: 
        cell_vec_t m_cells;
        popup_vec_t m_popups;


    TYPE_HANDLE(MarginManager, TypedObject);
};