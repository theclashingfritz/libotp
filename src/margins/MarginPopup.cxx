#include "MarginPopup.h"
#include "MarginCell.h"

NotifyCategoryDef(MarginPopup, "");

//TypeHandle MarginPopup::_type_handle;

MarginPopup::MarginPopup() : m_manager(nullptr), m_visible(false), m_priority(0) {
    MarginPopup_cat.debug() << "__init__()" << std::endl;
    m_last_cell = nullptr;
    m_assigned_cell = nullptr;
}

MarginPopup::~MarginPopup() {
    
}

void MarginPopup::set_visible(bool flag) {
    MarginPopup_cat.debug() << "set_visible(" << flag << ")" << std::endl;
    if (m_visible == flag) {
        return;
    }
        
    if (m_manager != nullptr && m_manager != NULL) {
        if (flag) {
            m_manager->add_visible_popup(this);
        } else {
            m_manager->remove_visible_popup(this);
        }
    }
}

bool MarginPopup::get_visible() {
    MarginPopup_cat.debug() << "get_visible()" << std::endl;
    return m_visible;
}

void MarginPopup::set_priority(int priority) {
    MarginPopup_cat.debug() << "set_priority(" << priority << ")" << std::endl;
    m_priority = priority;
    if (m_manager != nullptr && m_manager != NULL) {
        m_manager->reorganize();
    }
}

int MarginPopup::get_priority() {
    MarginPopup_cat.debug() << "get_priority()" << std::endl;
    return m_priority;
}

bool MarginPopup::is_displayed() {
    MarginPopup_cat.debug() << "is_displayed()" << std::endl;
    return (m_assigned_cell != nullptr && m_assigned_cell != NULL);
}

void MarginPopup::manage(MarginManager* manager) {
    MarginPopup_cat.debug() << "manage(MarginManager manager)" << std::endl;
    unmanage(m_manager);
    m_manager = manager;
    if (m_visible) {
        manager->add_visible_popup(this);
    }
}

void MarginPopup::unmanage(MarginManager* manager) {
    MarginPopup_cat.debug() << "unmanage(MarginManager manager)" << std::endl;
    (void *) manager; // Unused arg
    if (m_manager != nullptr && m_manager != NULL) {
        if (m_visible) {
            m_manager->remove_visible_popup(this);
        }
            
        m_manager = nullptr;
    }
}

void MarginPopup::set_last_cell(MarginCell* cell) {
    MarginPopup_cat.debug() << "set_last_cell(MarginCell cell)" << std::endl;
    m_last_cell = cell;
}

MarginCell* MarginPopup::get_last_cell() {
    MarginPopup_cat.debug() << "get_last_cell()" << std::endl;
    if ((m_last_cell == nullptr) || (m_last_cell == NULL)) {
        if (m_manager != nullptr && m_manager != NULL) {
            MarginCell* backup_cell = new MarginCell(m_manager);
            return backup_cell;
        }
        return nullptr;
    }
    return m_last_cell;
}

void MarginPopup::set_assigned_cell(MarginCell* cell) {
    MarginPopup_cat.debug() << "set_assigned_cell(MarginCell cell)" << std::endl;
    m_assigned_cell = cell;
}

MarginCell* MarginPopup::get_assigned_cell() {
    MarginPopup_cat.debug() << "get_assigned_cell()" << std::endl;
    if ((m_assigned_cell == nullptr) || (m_assigned_cell == NULL)) {
        if (m_manager != nullptr && m_manager != NULL) {
            MarginCell* backup_cell = new MarginCell(m_manager);
            return backup_cell;
        }
        return nullptr;
    }
    return m_assigned_cell;
}