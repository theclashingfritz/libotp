#include "NametagGlobals.h"

TypeHandle NametagGlobals::_type_handle;

typedef pmap<NametagGlobals::ColorCode, state_map_t> color_map_t; // {cc: state_map_t}
typedef pmap<NametagGlobals::WhisperType, state_map_simple_t> whisper_color_map_t; // {wt: state_map_simple_t}

PT(MouseWatcher) NametagGlobals::m_mouse_watcher = NULL;
PT(AudioSound) NametagGlobals::m_click_sound = NULL;
PT(AudioSound) NametagGlobals::m_rollover_sound = NULL;

NodePath& NametagGlobals::m_camera_nodepath EMPTY_NODEPATH;
NodePath& NametagGlobals::m_arrow_nodepath EMPTY_NODEPATH;
NodePath& NametagGlobals::m_nametag_model_nodepath EMPTY_NODEPATH;
NodePath& NametagGlobals::m_page_button_nodepath EMPTY_NODEPATH;
NodePath& NametagGlobals::m_nodepath EMPTY_NODEPATH;

ChatBalloon* NametagGlobals::speech_balloon_2d = NULL;
ChatBalloon* NametagGlobals::speech_balloon_3d = NULL;
ChatBalloon* NametagGlobals::thought_balloon_2d = NULL;
ChatBalloon* NametagGlobals::thought_balloon_3d = NULL;

uint8_t NametagGlobals::CFSpeech = 1;
uint8_t NametagGlobals::CFThought = 2;
uint8_t NametagGlobals::CFQuicktalker = 4;
uint8_t NametagGlobals::CFTimeout = 8;
uint8_t NametagGlobals::CFPageButton = 16;
uint8_t NametagGlobals::CFQuitButton = 32;
uint8_t NametagGlobals::CFReversed = 64;
uint8_t NametagGlobals::CFSndOpenchat = 128;
uint16_t NametagGlobals::CFNoQuitButton = 256;

uint8_t NametagGlobals::CName = 1;
uint8_t NametagGlobals::CSpeech = 2;
uint8_t NametagGlobals::CThought = 4;

buttons_map_t NametagGlobals::page_buttons;
buttons_map_t NametagGlobals::quit_buttons;

color_map_t NametagGlobals::nametag_colors;
whisper_color_map_t NametagGlobals::whisper_colors;

bool NametagGlobals::m_want_active_nametags = 1;
bool NametagGlobals::m_force_2d_nametags = 0;
bool NametagGlobals::m_force_onscreen_chat = 0;

NametagGlobals::NametagGlobals() {   
    add_color(CCNormal,
           (0.3, 0.3, 0.7, 1.0), (0.8, 0.8, 0.8, 0.5),
           (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
           (0.3, 0.3, 0.7, 1.0), (0.2, 0.2, 0.2, 0.6),
           (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
           (0.5, 0.5, 1.0, 1.0), (1.0, 1.0, 1.0, 1.0),
           (0.0, 0.6, 0.6, 1.0), (1.0, 1.0, 1.0, 1.0),
           (0.3, 0.3, 0.7, 1.0), (0.8, 0.8, 0.8, 0.5),
           (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    add_color(CCNoChat,
               (0.8, 0.4, 0.0, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (1.0, 0.5, 0.5, 1.0), (0.2, 0.2, 0.2, 0.6),
               (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
               (1.0, 0.5, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.0, 0.6, 0.6, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.8, 0.4, 0.0, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    add_color(CCNonPlayer,
               (0.8, 0.4, 0.0, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.8, 0.4, 0.0, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.8, 0.4, 0.0, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.8, 0.4, 0.0, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    add_color(CCSuit,
               (0.2, 0.2, 0.2, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.2, 0.2, 0.2, 1.0), (0.2, 0.2, 0.2, 0.6),
               (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.4, 0.4, 0.4, 1.0), (1.0, 1.0, 1.0, 0.7),
               (0.0, 0.6, 0.6, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.2, 0.2, 0.2, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    add_color(CCSuitBuilding,
               (0.5, 0.5, 0.5, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.5, 0.5, 0.5, 1.0), (0.8, 0.8, 0.8, 0.5),
               (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.5, 0.5, 0.5, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.6, 0.6, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.5, 0.5, 0.5, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    add_color(CCToonBuilding,
               (0.2, 0.6, 0.9, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.2, 0.6, 0.9, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.2, 0.6, 0.9, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.2, 0.6, 0.9, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    add_color(CCHouseBuilding,
               (0.2, 0.6, 0.9, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.2, 0.2, 0.5, 1.0), (0.2, 0.2, 0.2, 0.6),
               (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.5, 0.5, 1.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.0, 0.6, 0.6, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.0, 0.6, 0.2, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    add_color(CCSpeedChat,
               (0.0, 0.6, 0.2, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.0, 0.5, 0.0, 1.0), (0.5, 0.5, 0.5, 0.6),
               (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.0, 0.7, 0.2, 1.0), (1.0, 1.0, 1.0, 0.7),
               (0.0, 0.6, 0.6, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.0, 0.6, 0.2, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    add_color(CCFreeChat,
               (0.3, 0.3, 0.7, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.2, 0.2, 0.5, 1.0), (0.2, 0.2, 0.2, 0.6),
               (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.5, 0.5, 1.0, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.0, 0.6, 0.6, 1.0), (1.0, 1.0, 1.0, 1.0),
               (0.3, 0.3, 0.7, 1.0), (0.8, 0.8, 0.8, 0.5),
               (0.0, 0.0, 0.0, 1.0), (1.0, 1.0, 1.0, 1.0)
    );

    add_whisper_color(WTNormal,
                       (0.0, 0.0, 0.0, 1.0), (0.2, 0.6, 0.8, 0.6),
                       (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
                       (0.0, 0.0, 0.0, 1.0), (0.2, 0.7, 0.9, 0.6),
                       (0.0, 0.0, 0.0, 1.0), (0.2, 0.7, 0.8, 0.6)
    );

    add_whisper_color(WTQuickTalker,
                       (0.0, 0.0, 0.0, 1.0), (0.2, 0.6, 0.8, 0.6),
                       (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
                       (0.0, 0.0, 0.0, 1.0), (0.2, 0.7, 0.9, 0.6),
                       (0.0, 0.0, 0.0, 1.0), (0.2, 0.7, 0.8, 0.6)
    );

    add_whisper_color(WTSystem,
                       (0.0, 0.0, 0.0, 1.0), (0.8, 0.3, 0.6, 0.6),
                       (1.0, 0.5, 0.5, 1.0), (1.0, 1.0, 1.0, 1.0),
                       (0.0, 0.0, 0.0, 1.0), (0.8, 0.4, 1.0, 1.0),
                       (0.0, 0.0, 0.0, 1.0), (0.8, 0.3, 0.6, 0.6)
    );
}

NametagGlobals::~NametagGlobals() {
    delete m_click_sound;
    m_click_sound = NULL;
    delete m_rollover_sound;
    m_rollover_sound = NULL;
}

void NametagGlobals::add_color(ColorCode cc, LVecBase4f normal_name_fg, LVecBase4f normal_name_bg, LVecBase4f normal_chat_fg, LVecBase4f normal_chat_bg, LVecBase4f click_name_fg, LVecBase4f click_name_bg, LVecBase4f click_chat_fg, LVecBase4f click_chat_bg, LVecBase4f hover_name_fg, LVecBase4f hover_name_bg, LVecBase4f hover_chat_fg, LVecBase4f hover_chat_bg, LVecBase4f disabled_name_fg, LVecBase4f disabled_name_bg, LVecBase4f disabled_chat_fg, LVecBase4f disabled_chat_bg) {
    color_tuple_t normal_name(2);
    normal_name[0] = normal_name_fg;
    normal_name[1] = normal_name_bg;
    
    color_tuple_t normal_chat(2);
    normal_chat[0] = normal_chat_fg;
    normal_chat[1] = normal_chat_bg;
    
    color_tuple_tuple_t normal(2);
    normal[0] = normal_name;
    normal[1] = normal_chat;
    
    color_tuple_t click_name(2);
    click_name[0] = click_name_fg;
    click_name[1] = click_name_bg;
    
    color_tuple_t click_chat(2);
    click_chat[0] = click_chat_fg;
    click_chat[1] = click_chat_bg;
    
    color_tuple_tuple_t click(2);
    click[0] = click_name;
    click[1] = click_chat;
    
    color_tuple_t hover_name(2);
    hover_name[0] = hover_name_fg;
    hover_name[1] = hover_name_bg;
    
    color_tuple_t hover_chat(2);
    hover_chat[0] = hover_chat_fg;
    hover_chat[1] = hover_chat_bg;
    
    color_tuple_tuple_t hover(2);
    hover[0] = hover_name;
    hover[1] = hover_chat;
    
    color_tuple_t disabled_name(2);
    disabled_name[0] = disabled_name_fg;
    disabled_name[1] = disabled_name_bg;
    
    color_tuple_t disabled_chat(2);
    disabled_chat[0] = disabled_chat_fg;
    disabled_chat[1] = disabled_chat_bg;
    
    color_tuple_tuple_t disabled(2);
    disabled[0] = disabled_name;
    disabled[1] = disabled_chat;
    
    state_map_t def;
    def[CLICKSTATE_NORMAL] = normal;
    def[CLICKSTATE_CLICK] = click;
    def[CLICKSTATE_HOVER] = hover;
    def[CLICKSTATE_DISABLED] = disabled;
    
    nametag_colors[cc] = def;
}

void NametagGlobals::add_whisper_color(WhisperType wt, LVecBase4f normal_fg, LVecBase4f normal_bg, LVecBase4f click_fg, LVecBase4f click_bg, LVecBase4f hover_fg, LVecBase4f hover_bg, LVecBase4f disabled_fg, LVecBase4f disabled_bg) {
    color_tuple_t normal(2);
    normal[0] = normal_fg;
    normal[1] = normal_bg;
    
    color_tuple_t click(2);
    click[0] = click_fg;
    click[1] = click_bg;
    
    color_tuple_t hover(2);
    hover[0] = hover_fg;
    hover[1] = hover_bg;
    
    color_tuple_t disabled(2);
    disabled[0] = disabled_fg;
    disabled[1] = disabled_bg;
    
    state_map_simple_t def;
    def[CLICKSTATE_NORMAL] = normal;
    def[CLICKSTATE_CLICK] = click;
    def[CLICKSTATE_HOVER] = hover;
    def[CLICKSTATE_DISABLED] = disabled;
    
    whisper_colors[wt] = def;
}

void NametagGlobals::set_speech_balloon_3d(ChatBalloon* sb3d) {
    speech_balloon_3d = sb3d;
}

void NametagGlobals::set_thought_balloon_3d(ChatBalloon* tb3d) {
    thought_balloon_3d = tb3d;
}

void NametagGlobals::set_speech_balloon_2d(ChatBalloon* sb2d) {
    speech_balloon_2d = sb2d;
}

void NametagGlobals::set_thought_balloon_2d(ChatBalloon* tb2d) {
    thought_balloon_2d = tb2d;
}

void NametagGlobals::set_toon(NodePath& toon) {
    m_nodepath = toon;
}

void NametagGlobals::set_arrow_model(NodePath& node) {
    m_arrow_nodepath = node;
}

void NametagGlobals::set_mouse_watcher(NodePath& np) {
    m_mouse_watcher = DCAST(MouseWatcher, np.node());
}

void NametagGlobals::set_camera(NodePath& node) {
    m_camera_nodepath = node;
}

void NametagGlobals::set_want_active_nametags(bool want) {
    m_want_active_nametags = want;
}

void NametagGlobals::set_force_onscreen_chat(bool want) {
    m_force_onscreen_chat = want;
}

void NametagGlobals::set_force_2d_nametags(bool want) {
    m_force_2d_nametags = want;
}

void NametagGlobals::set_click_sound(PT(AudioSound) sound) {
    m_click_sound = sound;
}

void NametagGlobals::set_rollover_sound(PT(AudioSound) sound) {
    m_rollover_sound = sound;
}

PT(AudioSound) NametagGlobals::get_click_sound() {
    return m_click_sound;
}

PT(AudioSound) NametagGlobals::get_rollover_sound() {
    return m_rollover_sound;
}

NodePath& NametagGlobals::get_toon() {
    return m_nodepath;
}

NodePath& NametagGlobals::get_arrow_model() {
    return m_arrow_nodepath;
}

NodePath& NametagGlobals::get_camera() {
    return m_camera_nodepath;
}

LVecBase4f NametagGlobals::get_name_fg(ColorCode cc, int clickstate)
{
    if (clickstate < CLICKSTATE_NORMAL || clickstate > CLICKSTATE_DISABLED) {
        return LVecBase4f(1);
    }
    
    if (!does_color_code_exist(cc)) {
        return LVecBase4f(1);
    }
    
    return nametag_colors[cc][clickstate][0][0];
}

color_tuple_t NametagGlobals::get_whisper_colors(WhisperType wt, int clickstate)
{
    if (clickstate < CLICKSTATE_NORMAL || clickstate > CLICKSTATE_DISABLED) {
        color_tuple_t ret(2);
        ret[0] = LVecBase4f(1);
        ret[1] = LVecBase4f(1);
        return ret;
    }
    
    if (!does_whisper_type_exist(wt)) {
        color_tuple_t ret(2);
        ret[0] = LVecBase4f(1);
        ret[1] = LVecBase4f(1);
        return ret;
    }
    
    return whisper_colors[wt][clickstate];
}

bool NametagGlobals::get_want_active_nametags() {
    return m_want_active_nametags;
}

bool NametagGlobals::get_force_onscreen_chat() {
    return m_force_onscreen_chat;
}

bool NametagGlobals::get_force_2d_nametags() {
    return m_force_2d_nametags;
}

bool NametagGlobals::does_color_code_exist(ColorCode cc) {
    return nametag_colors.count(cc) == 1;
}

bool NametagGlobals::does_whisper_type_exist(WhisperType wt) {
    return whisper_colors.count(wt) == 1;
}