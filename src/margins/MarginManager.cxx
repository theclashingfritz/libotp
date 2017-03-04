#include "MarginManager.h"
#include <algorithm>

TypeHandle MarginManager::_type_handle;

MarginManager::MarginManager() {
    
}

MarginManager::~MarginManager() {
    
}

MarginCell* MarginManager::add_grid_cell(float x, float y, float left, float right, float bottom, float top, const NodePath& parent, float aspect_ratio) {
    float padding = .125;
    float scale = .2;
    float x_start = left + scale / 2. + padding;
    float y_start = bottom + scale / 2. + padding;
    float x_end = right - scale / 2. - padding;
    float y_end = top - scale / 2. - padding;
    float x_inc = (x_end - x_start) / 5.;
    float y_inc = (y_end - y_start) / 3.5;
    float x2 = x_start + x_inc * x;
    float y2 = y_start + y_inc * y - 1;
    
    if (x < 2)
        x2 += aspect_ratio;
        
    else
        x2 -= aspect_ratio;
        
    MarginCell* cell = new MarginCell(this);
    cell->reparent_to(parent);
    cell->set_scale(scale);
    cell->set_pos(x2, 0, y2);
    cell->set_available(true);
    
    m_cells.push_back(cell);
    reorganize();
    
    return cell;
}

void MarginManager::set_cell_available(MarginCell* cell, bool available) {
    cell->set_available(available);
    reorganize();
}

void MarginManager::add_visible_popup(MarginPopup* popup) {
    m_popups.push_back(popup);
    reorganize();
}

void MarginManager::remove_visible_popup(MarginPopup* popup) {
    popup_vec_t::iterator it = std::find(m_popups.begin(), m_popups.end(), popup);
    if (it == m_popups.end())
        return;
        
    m_popups.erase(it);
    reorganize();
}

static bool _sort_key(MarginPopup* lhs, MarginPopup* rhs) {
    return lhs->get_priority() < rhs->get_priority();
}

void MarginManager::reorganize() {
    cell_vec_t active_cells;
    for (cell_vec_t::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
    {
        if ((*it)->is_available())
            active_cells.push_back(*it);
    }
    
    popup_vec_t popups = m_popups;
    std::sort(popups.begin(), popups.end(), _sort_key);
    popups.resize(active_cells.size());
    
    cell_vec_t free_cells;
    for (cell_vec_t::iterator it = active_cells.begin(); it != active_cells.end(); ++it)
    {
        MarginCell* cell = *it;
        if (!cell->has_content())
            free_cells.push_back(cell);
        
        else if (std::find(popups.begin(), popups.end(), cell->get_content()) != popups.end())
            popups.erase(std::find(popups.begin(), popups.end(), cell->get_content()));
            
        else
        {
            cell->set_content(NULL);
            free_cells.push_back(cell);
        }
    }
    
    assert(free_cells.size() >= popups.size());
    std::random_shuffle(free_cells.begin(), free_cells.end());
    
    for (popup_vec_t::const_iterator it = popups.begin(); it != popups.end(); it++)
    {
        MarginPopup* popup = *it;
        MarginCell* last_cell = popup->get_last_cell();
        if (last_cell != NULL && std::find(free_cells.begin(), free_cells.end(), last_cell) != free_cells.end() && last_cell->is_free())
        {
            last_cell->set_content(popup);
            free_cells.erase(std::find(free_cells.begin(), free_cells.end(), last_cell));
        }
        
        else
        {
            free_cells.back()->set_content(popup);
            free_cells.pop_back();
        }
    }
}