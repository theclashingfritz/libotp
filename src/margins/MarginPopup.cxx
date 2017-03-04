#include "MarginPopup.h"

TypeHandle MarginPopup::_type_handle;

MarginPopup::MarginPopup() : m_manager(NULL), m_visible(false), m_priority(0), m_last_cell(NULL), m_assigned_cell(NULL) {
    
}

MarginPopup::~MarginPopup() {
    
}

void MarginPopup::set_visible(bool flag) {
    if (m_visible == flag)
        return;
        
    if (m_manager != NULL)
    {
        if (flag)
            m_manager->add_visible_popup(this);
            
        else
            m_manager->remove_visible_popup(this);
    }
}

bool MarginPopup::get_visible() {
    return m_visible;
}

void MarginPopup::set_priority(int priority) {
    m_priority = priority;
    if (m_manager != NULL)
        m_manager->reorganize();
}

int MarginPopup::get_priority() {
    return m_priority;
}

bool MarginPopup::is_displayed() {
    return m_assigned_cell != NULL;
}

void MarginPopup::manage(MarginManager* manager) {
    unmanage(m_manager);
    m_manager = manager;
    if (m_visible)
        manager->add_visible_popup(this);
}

void MarginPopup::unmanage(MarginManager* manager) {
    (void *) manager; // Unused arg
    if (m_manager != NULL)
    {
        if (m_visible)
            m_manager->remove_visible_popup(this);
            
        m_manager = NULL;
    }
}

void MarginPopup::set_last_cell(MarginCell* cell) {
    m_last_cell = cell;
}

MarginCell* MarginPopup::get_last_cell() {
    return m_last_cell;
}

void MarginPopup::set_assigned_cell(MarginCell* cell) {
    m_assigned_cell = cell;
}

MarginCell* MarginPopup::get_assigned_cell() {
    return m_assigned_cell;
}