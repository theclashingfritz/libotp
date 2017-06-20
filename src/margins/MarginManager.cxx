#include "MarginManager.h"
#include <algorithm>

NotifyCategoryDef(MarginManager, "");

TypeHandle MarginManager::_type_handle;

MarginManager* MarginManager::_global_ptr = nullptr;

MarginManager::MarginManager() : PandaNode("mrmanager") {
    MarginManager_cat.debug() << "__init__()" << std::endl;
}

MarginManager::~MarginManager() {
    
}

void MarginManager::set_cell_available(int cell_index, bool available) {
    MarginManager_cat.debug() << "set_cell_available(" << cell_index << " " << available << ")" << std::endl;
    MarginCell* cell = m_cells.at(cell_index);
    
    if ((cell == nullptr) || (cell == NULL)) {
        return;
    }
    
    cell_vec_t::iterator it = std::find(m_cells.begin(), m_cells.end(), cell);
    if (it == m_cells.end())
        return;
    
    cell->set_available(available);
    reorganize();
}

void MarginManager::set_cell_available(MarginCell* cell, bool available) {
    MarginManager_cat.debug() << "set_cell_available(" << "MarginCell cell" << " " << available << ")" << std::endl;
    if ((cell == nullptr) || (cell == NULL)) {
        return;
    }
    
    cell->set_available(available);
    reorganize();
}

void MarginManager::remove_cell(int cell_index) {
    MarginManager_cat.debug() << "remove_cell(" << cell_index << ")" << std::endl;
    MarginCell* cell = m_cells.at(cell_index);
    
    if ((cell == nullptr) || (cell == NULL)) {
        return;
    }
    
    cell_vec_t::iterator it = std::find(m_cells.begin(), m_cells.end(), cell);
    if (it == m_cells.end())
        return;
        
    m_cells.erase(it);
    reorganize();
}

void MarginManager::remove_cell(MarginCell* cell) {
    MarginManager_cat.debug() << "remove_cell(MarginCell cell)" << std::endl;
    if ((cell == nullptr) || (cell == NULL)) {
        return;
    }
   
    cell_vec_t::iterator it = std::find(m_cells.begin(), m_cells.end(), cell);
    if (it == m_cells.end())
        return;
        
    m_cells.erase(it);
    reorganize();
}

void MarginManager::add_visible_popup(MarginPopup* popup) {
    MarginManager_cat.debug() << "add_visible_popup(MarginPopup popup)" << std::endl;
    if ((popup == nullptr) || (popup == NULL)) {
        return;
    }
    
    m_popups.push_back(popup);
    reorganize();
}

void MarginManager::remove_visible_popup(MarginPopup* popup) {
    MarginManager_cat.debug() << "remove_visible_popup(MarginPopup popup)" << std::endl;
    if ((popup == nullptr) || (popup == NULL)) {
        return;
    }
    
    popup_vec_t::iterator it = std::find(m_popups.begin(), m_popups.end(), popup);
    if (it == m_popups.end())
        return;
        
    m_popups.erase(it);
    reorganize();
}

void MarginManager::reorganize() {
    MarginManager_cat.debug() << "reorganize()" << std::endl;
    cell_vec_t active_cells;
    for (cell_vec_t::iterator it = m_cells.begin(); it != m_cells.end(); ++it) {
        if ((*it)->is_available())
            active_cells.push_back(*it);
    }
    
    popup_vec_t popups = m_popups;
    std::sort(popups.begin(), popups.end(), _sort_key);
    popups.resize(active_cells.size());
    
    cell_vec_t free_cells;
    for (cell_vec_t::iterator it = active_cells.begin(); it != active_cells.end(); ++it) {
        MarginCell* cell = *it;
        if (cell != nullptr && cell != NULL) {
            if (!cell->has_content()) {
                free_cells.push_back(cell);
            } else if (std::find(popups.begin(), popups.end(), cell->get_content()) != popups.end()) {
                popups.erase(std::find(popups.begin(), popups.end(), cell->get_content()));
            } else {
                cell->set_content(nullptr);
                free_cells.push_back(cell);
            }
        }
    }
    
    assert(free_cells.size() >= popups.size());
    std::random_shuffle(free_cells.begin(), free_cells.end());
    
    for (popup_vec_t::const_iterator it = popups.begin(); it != popups.end(); it++) {
        MarginPopup* popup = *it;
        if (popup != nullptr && popup != NULL) {
            MarginCell* last_cell = popup->get_last_cell();
            if (last_cell != nullptr && last_cell != NULL && std::find(free_cells.begin(), free_cells.end(), last_cell) != free_cells.end() && last_cell->is_free()) {
                last_cell->set_content(popup);
                free_cells.erase(std::find(free_cells.begin(), free_cells.end(), last_cell));
            } else {
                free_cells.back()->set_content(popup);
                free_cells.pop_back();
            }
        }
    }
}


MarginCell* MarginManager::add_grid_cell(float x, float y, float left, float right, float bottom, float top) {
    MarginManager_cat.debug() << "add_grid_cell(" << x << " " << y << " " << left << " " << right << " " << bottom << " " << top << ")" << std::endl;
    double v7; // st7@1 MAPDST
    double v8; // st5@1 MAPDST
    double v12; // st6@1 MAPDST
    float f_top; // ST0C_4@1
    float f_bottom; // ST08_4@1
    float f_right; // ST04_4@1
    float v17; // [sp+1Ch] [bp+Ch]@1 MAPDST
    float f_left; // [sp+20h] [bp+10h]@1 MAPDST

    v7 = left;
    f_left = right - left;
    v17 = f_left / 6.0;
    f_left = top - bottom;
    f_left = f_left / 6.0;
    v8 = v7 + v17 * x;
    v7 = v17;
    v17 = v8;
    v12 = f_left;
    f_left = bottom + f_left * y;
    v8 = v12 + f_left;
    v12 = f_left;
    f_left = v8;
    f_left = f_left - 0.009999999776482582;
    f_top = f_left;
    f_left = v12 + 0.009999999776482582;
    f_bottom = f_left;
    f_left = v7 + v17;
    f_left = f_left - 0.009999999776482582;
    f_right = f_left;
    f_left = v17 + 0.009999999776482582;
    return add_cell(f_left, f_right, f_bottom, f_top);
}

MarginCell* MarginManager::add_cell(float left, float right, float bottom, float top) {
    MarginManager_cat.debug() << "add_cell(" << left << " " << right << " " << bottom << " " << top << ")" << std::endl;
    float padding = .125;
    float scale = .2;
    float x_start = left + scale / 2. + padding;
    float y_start = bottom + scale / 2. + padding;
    float x_end = right - scale / 2. - padding;
    float y_end = top - scale / 2. - padding;
    float x_inc = (x_end - x_start) / 5.;
    float y_inc = (y_end - y_start) / 3.5;
    float x2 = x_start + x_inc;
    float y2 = y_start + y_inc;
        
    MarginCell* cell = new MarginCell(this);
    cell->reparent_to(NodePath(this->get_parent(0)));
    cell->set_scale(scale);
    cell->set_pos(x2, 0, y2);
    cell->set_available(true);
    
    m_cells.push_back(cell);
    reorganize();
    
    return cell;
}


bool MarginManager::_sort_key(MarginPopup* lhs, MarginPopup* rhs) {
    MarginManager_cat.debug() << "_sort_key(MarginPopup lhs, MarginPopup rhs)" << std::endl;
    if (((lhs == nullptr) || (rhs == nullptr)) || ((lhs == NULL) || (rhs == NULL))) {
        return false;
    }
    
    return lhs->get_priority() < rhs->get_priority();
}

bool MarginManager::get_cell_available(int cell_index) {
    MarginManager_cat.debug() << "get_cell_available(" << cell_index << ")" << std::endl;
    MarginCell* cell = m_cells.at(cell_index);
    
    if ((cell == nullptr) || (cell == NULL)) {
        return false;
    }
    
    cell_vec_t::iterator it = std::find(m_cells.begin(), m_cells.end(), cell);
    if (it == m_cells.end())
        return false;
    
    return cell->get_available();
}

bool MarginManager::get_cell_available(MarginCell* cell) { 
    MarginManager_cat.debug() << "get_cell_available(MarginCell cell)" << std::endl;
    if ((cell == nullptr) || (cell == NULL)) {
        return false;
    }
    
    cell_vec_t::iterator it = std::find(m_cells.begin(), m_cells.end(), cell);
    if (it == m_cells.end())
        return false;
    
    return cell->get_available();
}

MarginManager* MarginManager::get_global_ptr() {
    if ((_global_ptr == nullptr) || (_global_ptr == NULL)) {
        _global_ptr = new MarginManager;
    }
    return _global_ptr;
}