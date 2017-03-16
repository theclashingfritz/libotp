#include "MarginCell.h"
#include "MarginPopup.h"
#include "ClickablePopup.h"
#include "WhisperPopup.h"

NotifyCategoryDef(MarginCell, "");

TypeHandle MarginCell::_type_handle;

MarginCell::MarginCell(MarginManager* manager): NodePath("cell"), m_manager(manager), m_content(nullptr) {
    MarginCell_cat.debug() << "__init__(MarginManager manager)" << std::endl;
}

MarginCell::~MarginCell() {
    
}
        
void MarginCell::set_available(bool available) {
    MarginCell_cat.debug() << "set_available(" << available << ")" << std::endl;
    if (!available && has_content()) {
        set_content(nullptr);
    }
        
    m_available = available;  
}
       
void MarginCell::set_content(MarginPopup* content) {
    MarginCell_cat.debug() << "set_content(MarginPopup content)" << std::endl;
    if (content == nullptr || content == NULL) {
        return;
    }  

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

void MarginCell::set_content_nodepath(PandaNode* path) {
    MarginCell_cat.debug() << "set_content_nodepath(PandaNode path)" << std::endl;
    if (path == nullptr || path == NULL) {
        return;
    }  

    if (has_content()) {
        m_content_np.remove_node();
    }
    
    m_content_np = attach_new_node(path);
    
    if (has_content()) {
        m_content->margin_visibility_changed();
    }
}

MarginPopup* MarginCell::get_content() {
    MarginCell_cat.debug() << "get_content()" << std::endl;
    if (has_content()) {
        return m_content;
    } else {
        return nullptr;
    }
}

bool MarginCell::has_content() {   
    MarginCell_cat.debug() << "has_content()" << std::endl;
    return (m_content != nullptr && m_content != NULL);
}
        
bool MarginCell::is_available() {
    MarginCell_cat.debug() << "is_available()" << std::endl;
    return m_available;
}

bool MarginCell::get_available() {
    MarginCell_cat.debug() << "get_available()" << std::endl;
    return m_available;
}

bool MarginCell::is_free() {
    MarginCell_cat.debug() << "is_free()" << std::endl;
    return is_available() && !has_content();
}