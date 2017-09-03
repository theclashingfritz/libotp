#include "Nametag3d.h"
#include "NametagGroup.h"

#include "PopupMouseWatcherRegion.h"

#include <billboardEffect.h>
#include <math.h>

NotifyCategoryDef(Nametag3d, "");

TypeHandle Nametag3d::_type_handle;

Nametag3d::Nametag3d() : Nametag(true), m_bb_offset(3) {
    Nametag3d_cat.debug() << "__init__()" << std::endl;
    m_inner_np = NodePath("nametag_contents");
    m_contents = Nametag::CName | Nametag::CSpeech | Nametag3d::CThought;
    billboard();
};

Nametag3d::Nametag3d(const Nametag3d& tag) : Nametag() {
    Nametag3d_cat.debug() << "__init__()" << std::endl;
    m_inner_np = NodePath("nametag_contents");
    m_contents = tag.m_contents;
    m_chat_flags = tag.m_chat_flags;
    m_color_code = tag.m_color_code;
    m_name = std::wstring(tag.m_name);
    m_display_name = std::wstring(tag.m_display_name);
    m_chat_string = std::wstring(tag.m_chat_string);
    m_chat_wordwrap = tag.m_chat_wordwrap;
    m_wordwrap = tag.m_wordwrap;
    m_bb_offset = tag.m_bb_offset;
    m_name_fg = LVecBase4f(tag.m_name_fg);
    m_name_bg = LVecBase4f(tag.m_name_bg);
    m_chat_fg = LVecBase4f(tag.m_chat_fg);
    m_chat_bg = LVecBase4f(tag.m_chat_bg);
    m_avatar = &*tag.m_avatar; 
    m_group = &*tag.m_group;
    m_font = tag.m_font; 
    m_has_group = tag.m_has_group;
    m_active = tag.m_active;
    billboard();
};

Nametag3d::~Nametag3d() {

};

/**
 * This little guy is a overload operator. We use this to assign a Nametag object to this object!
 */
Nametag3d& Nametag3d::operator=(const Nametag3d& tag) {
    m_contents = tag.m_contents;
    m_chat_flags = tag.m_chat_flags;
    m_color_code = tag.m_color_code;
    m_name = std::wstring(tag.m_name);
    m_display_name = std::wstring(tag.m_display_name);
    m_chat_string = std::wstring(tag.m_chat_string);
    m_chat_wordwrap = tag.m_chat_wordwrap;
    m_wordwrap = tag.m_wordwrap;
    m_bb_offset = tag.m_bb_offset;
    m_name_fg = LVecBase4f(tag.m_name_fg);
    m_name_bg = LVecBase4f(tag.m_name_bg);
    m_chat_fg = LVecBase4f(tag.m_chat_fg);
    m_chat_bg = LVecBase4f(tag.m_chat_bg);
    m_avatar = &*tag.m_avatar; 
    m_group = &*tag.m_group;
    m_font = tag.m_font; 
    m_has_group = tag.m_has_group;
    m_active = tag.m_active;
    billboard();
    return *this;
};

void Nametag3d::tick() {
    // Do nothing. Disney didn't use Tick and it will be completely scarpped after i 
    // completely reverse engineer libotp enough to discard it.
};

void Nametag3d::set_billboard_offset(float offset) {
    m_bb_offset = offset;
};

void Nametag3d::billboard() {
    Nametag3d_cat.debug() << "billboard()" << std::endl;
    if (NametagGlobals::m_camera_nodepath->is_empty()) {
        return;
    }
    m_inner_np.set_effect(BillboardEffect::make(LVecBase3f(0, 0, 1), true, false, m_bb_offset, *NametagGlobals::m_camera_nodepath, LPoint3f(0)));
};

void Nametag3d::cull_callback(CullTraverser *traverser, CullTraverserData *traverser_data) {
    NodePath wpath = traverser_data->get_node_path();
    const RenderState *render_state = wpath.get_state();
    int bin_index = render_state->get_bin_index();
    adjust_to_camera(wpath, bin_index);
};

void Nametag3d::show_name() {
    if (m_font == nullptr || m_font == NULL) {
        return;
    }
    m_inner_np.attach_new_node(m_icon.node());
    PT(TextNode) text = new TextNode("name");
    text->set_font(m_font);
    text->set_align(TextNode::A_center);
    text->set_wordwrap(m_wordwrap);
    text->set_wtext(m_display_name);
    NodePath t = m_inner_np.attach_new_node(text, 1);
    t.set_color(0.0, 0.0, 0.0, 1.0);
    t.set_transparency(TransparencyAttrib::M_none);
    t.set_scale(0.90);
    
    NodePath panel = NametagGlobals::m_card_nodepath->copy_to(m_inner_np, 0);
    
    panel.set_color(1.0, 1.0, 1.0, 1.0);
    panel.set_transparency(TransparencyAttrib::M_alpha);
    panel.set_scale(t, (text->get_left() - text->get_right() - 0.15), 1, ((text->get_top() + 0.13) - text->get_bottom()));
    panel.set_two_sided(1, 0);
    panel.set_color_scale(0.8, 0.8, 0.8, 0.5);
    panel.set_pos(t.get_bounds()->as_bounding_box()->get_approx_center());
    panel.set_scissor(text->get_left(), text->get_right(), text->get_bottom(), text->get_top());
    
    t.set_y(panel.get_y() - 0.03);
    t.set_attrib(DepthWriteAttrib::make(DepthWriteAttrib::M_off));
}

void Nametag3d::adjust_to_camera(NodePath path, int bin_index) {
    Nametag3d_cat.debug() << "adjust_to_camera(NodePath path, " << value << ")" << std::endl;
    //In till i completely reverse engineer the adjust_to_camera() function in Disney's libotp, this will do.
    /**
    void Nametag3d::adjust_to_camera(NodePath path, int value) {
        TransformState * transform_state = path.get_transform();
        LVector3f scale;
        if (transform_state->has_scale()) {
            scale = transform_state->get_scale();
        } else {
            return;
        }
        LVector3f up = LVector3f::up;
        LVector3f forward = LVector3f::forward;
    }
    **/
    if (NametagGlobals::m_camera_nodepath->is_empty()) {
        return;
    }
    double distance = path.get_pos(*NametagGlobals::m_camera_nodepath).length();
    distance = max(min(distance, 1.0), 50.0);
    path.set_scale(sqrt(distance) * 0.065);
    
    PT(TextNode) text = DCAST(TextNode, m_inner_np.find("name").node());
    float left = text->get_left();
    float right = text->get_right();
    float top = text->get_top();
    float bottom = text->get_bottom();
    
    CPT(TransformState) transform = m_inner_np.get_net_transform();
    if (NametagGlobals::m_camera_nodepath != nullptr && NametagGlobals::m_camera_nodepath != NULL && !NametagGlobals::m_camera_nodepath->is_empty()) {
        CPT(TransformState) cam_transform = NametagGlobals::m_camera_nodepath->get_net_transform();
        transform = cam_transform->get_inverse()->compose(transform);
    }
    
    LQuaternionf quat = LQuaternionf(1.0f, 0.0f, 0.0f, 0.0f);
    
    if (quat.is_nan()) {
        Nametag3d_cat.error() << "Base Quat returned is_nan() as True!" << std::endl;
        return;
    }
    
    transform = transform->set_quat(quat);
    LMatrix4f mat = transform->get_mat();
    LVecBase3f c_top_left = mat.xform_point(LPoint3f(left, 0, top));
    LVecBase3f c_bottom_right = mat.xform_point(LPoint3f(right, 0, bottom));
    
    float s_left;
    float s_right;
    float s_top;
    float s_bottom;
    LPoint2f s_top_left, s_bottom_right;
    
    if (NametagGlobals::m_camera_nodepath != nullptr && NametagGlobals::m_camera_nodepath != NULL && !NametagGlobals::m_camera_nodepath->is_empty()) {
        PT(Lens) lens = DCAST(Camera, NametagGlobals::m_camera_nodepath->node())->get_lens();
        
        s_top_left = LPoint2f();
        s_bottom_right = LPoint2f();
        
        if (!lens->project(LPoint3f(c_top_left), s_top_left) || !lens->project(LPoint3f(c_bottom_right), s_bottom_right)) {
            _region->set_active(false);
            return;
        }
    } else {
        s_top_left = LPoint2f(s_top_left.get_x(), s_top_left.get_y());
        s_bottom_right = LPoint2f(s_bottom_right.get_x(), s_bottom_right.get_y());
    }
    
    s_left = s_top_left.get_x();
    s_right = s_bottom_right.get_x();
    s_top = s_top_left.get_y();
    s_bottom = s_bottom_right.get_y();
    
    int depth = (c_top_left.get_y() + c_bottom_right.get_y()) / 2.0;
    depth = depth * 1000;
    depth = depth * -1;
    
    Nametag3d_cat.debug() << "Setting Region Frame!" << std::endl;
    if (_region != nullptr && _region != NULL) {
        _region->set_sort(depth);
        _region->set_frame(s_left, s_right, s_bottom, s_top);
        _region->set_active(true);
    }
    Nametag3d_cat.debug() << "Updated Click Region!" << std::endl;
};

void Nametag3d::update_contents() {
    update();
};

float Nametag3d::get_billboard_offset() {
    return m_bb_offset;
};

bool Nametag3d::safe_to_flatten_below() {
    return false;
};

ChatBalloon* Nametag3d::get_speech_balloon() {
    Nametag3d_cat.debug() << "get_speech_balloon()" << std::endl;
    if (NametagGlobals::speech_balloon_3d == nullptr || NametagGlobals::speech_balloon_3d == NULL) {
        return nullptr;
    }
    return NametagGlobals::speech_balloon_3d;
};

ChatBalloon* Nametag3d::get_thought_balloon() {
    Nametag3d_cat.debug() << "get_thought_balloon()" << std::endl;
    if (NametagGlobals::thought_balloon_3d == nullptr || NametagGlobals::thought_balloon_3d == NULL) {
        return nullptr;
    }
    return NametagGlobals::thought_balloon_3d;
};