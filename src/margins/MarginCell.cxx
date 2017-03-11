#include "MarginCell.h"
#include "MarginPopup.h"
#include "ClickablePopup.h"

#include <pandaNode.h>

TypeHandle MarginCell::_type_handle;

MarginCell::MarginCell(MarginManager* manager): NodePath("cell"), m_manager(manager), m_content(nullptr) {
    
}

MarginCell::~MarginCell() {
    
}
        
void MarginCell::set_available(bool available) {
    if (!available && has_content()) {
        set_content(nullptr);
    }
        
    m_available = available;  
}

bool MarginCell::get_available() {
    return m_available;
}
       
void MarginCell::set_content(MarginPopup* content) {
    if (has_content()) {
        m_content->set_assigned_cell(nullptr);
        m_content_np.remove_node();
        m_content->margin_visibility_changed();
    }
    
    if (content != nullptr && content != NULL) {
        content->set_assigned_cell(this);
        content->set_last_cell(this);
        std::string m_content_np_name = "m_content_np";
        m_content_np = attach_new_node(new PandaNode(m_content_np_name));
        content->margin_visibility_changed();
    }
    
    m_content = content;
}

bool MarginCell::has_content() {   
    return (m_content != nullptr && m_content != NULL);
}

MarginPopup* MarginCell::get_content() {
    if (m_content != nullptr && m_content != NULL) {
        return m_content;
    } else {
        return nullptr;
    }
}
        
bool MarginCell::is_available() {
    return m_available;
}

bool MarginCell::is_free() {
    return is_available() && !has_content();
}