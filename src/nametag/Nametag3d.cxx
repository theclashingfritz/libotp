#include "Nametag3d.h"
#include "NametagGroup.h"

#include <billboardEffect.h>
#include <math.h>

NotifyCategoryDef(Nametag3d, "");

TypeHandle Nametag3d::_type_handle;

Nametag3d::Nametag3d() : Nametag(true), m_bb_offset(3) {
    Nametag3d_cat.debug() << "__init__()" << std::endl;
    m_contents = Nametag::CName | Nametag::CSpeech | Nametag3d::CThought;
    billboard();
};

Nametag3d::Nametag3d(const Nametag3d& tag) : Nametag() {
    Nametag3d_cat.debug() << "__init__()" << std::endl;
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

void Nametag3d::adjust_to_camera(NodePath path, int value) {
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
    /**
    __asm{push    ebp
       push    eax
       push    ebx
       push    esi
       push    edi
       mov     ebx, ecx
       mov     ecx, [ebx+20h]
       xor     edi, edi
       cmp     byte [ebx+16Ch], 0
       mov     [esp+0C0h], ecx
       jnz     loc_100107E6
       mov     eax, [ecx+14Ch]
       cmp     eax, 2
       jg      loc_1001051D
       mov     eax, 2
    loc_1001051D:
       mov     [ecx+14Ch], eax
       mov     ecx, [ebx+20h]
       mov     al, [ebx+1Ch]
       cmp     ecx, edi
       jz      loc_10010556
       test    al, al
       jz      loc_10011CBB
       call    NametagGroup::display_as_active
       test    al, al
       jnz     loc_1001056B
       mov     ecx, [esp+3A0h]
       mov     [fs:0], ecx
       pop     ecx
       pop     edi
       pop     esi
       pop     ebx
       mov     esp, ebp
       pop     ebp
       retn    8
    loc_10010556:
       test    al, al
       jz      loc_10011CBB
       cmp     [NametagGlobals::_master_nametags_active], 0
       jz      loc_10011CBB
    loc_1001056B:
       cmp     byte [ebx+1A4h], 0
       jz      loc_10011CBB
       mov     eax, [ThreadSimpleManager::_pointers_initialized]
       cmp     byte [eax], 0
       jnz     loc_10010588
       call    ThreadSimpleManager::init_pointers
    loc_10010588:
       mov     ecx, [ThreadSimpleManager::_global_ptr]
       mov     eax, [ecx]
       mov     edx, [eax+80h]
       mov     eax, [edx+4]
       cmp     eax, edi
       jnz     loc_100105B4
       mov     eax, [Thread::_external_thread]
       cmp     [eax], edi
       jnz     loc_100105AC
       call    Thread::init_external_thread
    loc_100105AC:
       mov     ecx, [Thread::_external_thread]
       mov     eax, [ecx]
    loc_100105B4:
       mov     ecx, [ebp+8]
       push    eax
       lea     edx, [esp+68h]
       push    edx
       call    NodePath::get_net_transform
       mov     [esp+3A8h], edi
       mov     ecx, [esp+64h]
       mov     eax, [ecx+0ECh]
       mov     edx, eax
       shr     edx, 7
       not     dl
       test    dl, 1
       jnz     loc_1001062E
       mov     esi, ecx
       mov     dword [esp+3A8h], 1
       mov     [esp+64h], edi
       cmp     esi, edi
    loc_100105F3:
       jz      loc_10011CBB
    loc_100105F9:
       mov     eax, [esi+0Ch]
       mov     edx, [eax+4]
       lea     ecx, [esi+0Ch]
       call    edx
       test    al, al
       jnz     loc_10011CBB
       mov     eax, [esi]
       mov     edx, [eax]
       push    1
       mov     ecx, esi
       call    edx
       mov     ecx, [esp+3A0h]
       mov     [fs:0], ecx
       pop     ecx
       pop     edi
       pop     esi
       pop     ebx
       mov     esp, ebp
       pop     ebp
       retn    8
    loc_1001062E:
       mov     esi, ecx
       test    al, 40h
       jnz     loc_1001064C
       lea     eax, [ecx+0F0h]
       mov     [esp+4Ch], eax
       mov     byte [esp+3A8h], 2
       call    TransformState::do_calc_mat
    loc_1001064C:
       fld     dword [ebx+1A8h]
       add     esi, 0A4h
       fld     [0.5]
       fsub    st(1), st
       fxch    st(1)
       fstp    dword [esp+1Ch]
       fld     dword [ebx+1B0h]
       fld1    
       fsub    st(1), st
       fxch    st(1)
       fstp    dword [esp+14h]
       fld     dword [esp+1Ch]
       fstp    dword [esp+0E8h]
       fldz    
       fst     dword [esp+0ECh]
       fld     dword [esp+14h]
       fstp    dword [esp+0F0h]
       fld     dword [ebx+1ACh]
       faddp   st(3), st
       fxch    st(2)
       fstp    dword [esp+1Ch]
       fadd    dword [ebx+1B4h]
       fstp    dword [esp+14h]
       fld     dword [esp+1Ch]
       fstp    dword [esp+0DCh]
       fstp    dword [esp+0E0h]
       fld     dword [esp+14h]
       fstp    dword [esp+0E4h]
       lea     ecx, [esp+0E8h]
       push    ecx
       lea     edx, [esp+354h]
       push    edx
       mov     ecx, esi
       mov     byte [esp+3B0h], 4
       call    LMatrix4f::xform_point
       fld     dword [eax]
       fstp    dword [esp+0E8h]
       lea     ecx, [esp+2B4h]
       fld     dword [eax+4]
       fstp    dword [esp+0ECh]
       fld     dword [eax+8]
       lea     eax, [esp+0DCh]
       push    eax
       fstp    dword [esp+0F4h]
       push    ecx
       mov     ecx, esi
       call    LMatrix4f::xform_point
       fld     dword [eax]
       fstp    dword [esp+0DCh]
       fld     dword [eax+4]
       fstp    dword [esp+0E0h]
       fld     dword [eax+8]
       mov     eax, [ebp+0Ch]
       fstp    dword [esp+0E4h]
       shl     eax, 10h
       fld     dword [esp+0E8h]
       or      eax, 2710h
       fstp    dword [esp+1F8h]
       fld     dword [esp+0DCh]
       fstp    dword [esp+1FCh]
       fld     dword [esp+0F0h]
       fstp    dword [esp+200h]
       fld     dword [esp+0E4h]
       fstp    dword [esp+204h]
       push    eax
       lea     edx, [esp+1FCh]
       push    edx
       mov     ecx, ebx
       mov     byte [esp+3B0h], 5
       call    Nametag::set_region
       mov     dword [esp+3A8h], 6
       mov     esi, [esp+64h]
       cmp     esi, edi
       jz      loc_10011CBB
       lea     ecx, [esi+0Ch]
       mov     [esp+64h], edi
       mov     eax, [ecx]
       mov     edx, [eax+4]
       call    edx
       test    al, al
       jnz     loc_10011CBB
       mov     eax, [esi]
       mov     edx, [eax]
       push    1
       mov     ecx, esi
       call    edx
       mov     ecx, [esp+3A0h]
       mov     [fs:0], ecx
       pop     ecx
       pop     edi
       pop     esi
       pop     ebx
       mov     esp, ebp
       pop     ebp
       retn    8
    loc_100107E6:
       mov     eax, [NametagGlobals::m_camera_nodepath]
       cmp     eax, edi
       jnz     loc_100107F3
       xor     esi, esi
       jmp     loc_100107F6
    loc_100107F3:
       mov     esi, [eax+0Ch]
    loc_100107F6:
       mov     eax, [esi+118h]
       sub     eax, [esi+114h]
       test    eax, 0FFFFFFF8h
       jle     loc_10010830
       mov     ecx, [esi+118h]
       sub     ecx, [esi+114h]
       sar     ecx, 3
       test    ecx, ecx
       ja      loc_10010822
       call    _invalid_parameter_noinfo
    loc_10010822:
       mov     edx, [esi+114h]
       mov     eax, [edx]
       mov     [esp+7Ch], eax
       jmp     loc_10010834
    loc_10010830:
       mov     [esp+7Ch], edi
    loc_10010834:
       cmp     [ebx+28h], edi
       lea     eax, [ebx+28h]
       jnz     loc_1001084C
       mov     ecx, [ebx+20h]
       cmp     ecx, edi
       jz      loc_1001084C
       add     ecx, 28h
       mov     [esp+78h], ecx
       jmp     loc_10010850
    loc_1001084C:
       mov     [esp+78h], eax
    loc_10010850:
       mov     ecx, [ThreadSimpleManager::_pointers_initialized]
       cmp     byte [ecx], 0
       jnz     loc_10010861
       call    ThreadSimpleManager::init_pointers
    loc_10010861:
       mov     edx, [ThreadSimpleManager::_global_ptr]
       mov     eax, [edx]
       mov     eax, [eax+80h]
       mov     eax, [eax+4]
       cmp     eax, edi
       jnz     loc_1001088E
       mov     ecx, [Thread::_external_thread]
       cmp     [ecx], edi
       jnz     loc_10010886
       call    Thread::init_external_thread
    loc_10010886:
       mov     edx, [Thread::_external_thread]
       mov     eax, [edx]
    loc_1001088E:
       push    eax
       mov     eax, [ebp+8]
       push    eax
       lea     ecx, [esp+28h]
       push    ecx
       mov     ecx, NametagGlobals::m_camera_nodepath
       call    NodePath::get_transform
       mov     dword [esp+3A8h], 7
       mov     ecx, [esp+20h]
       mov     eax, [ecx+0ECh]
       mov     edx, eax
       shr     edx, 7
       not     dl
       test    dl, 1
       jnz     loc_100108E0
       mov     esi, ecx
       mov     dword [esp+3A8h], 8
       mov     dword [esp+20h], 0
       test    esi, esi
       jmp     loc_100105F3
    loc_100108E0:
       mov     esi, ecx
       test    al, 40h
       jnz     loc_10010909
       lea     eax, [ecx+0F0h]
       mov     [esp+84h], eax
       mov     byte [esp+3A8h], 9
       call    TransformState::do_calc_mat
       mov     byte [esp+3A8h], 7
    loc_10010909:
       lea     ecx, [esp+2E4h]
       push    0
       lea     edi, [esi+0A4h]
       push    ecx
       mov     [esp+74h], edi
       call    LVector3f::up
       push    edi
       lea     esi, [esp+298h]
       mov     byte [esp+3B4h], 0Ah
       sub     esp, 10h
       push    eax
       lea     ecx, [esp+8]
       push    ecx
       mov     ecx, [esp+1Ch]
       mov     dword [esp+8], 0
       call    LMatrix4f::xform_point
       fld     dword [eax]
       fstp    dword [esi]
       fld     dword [eax+4]
       fstp    dword [esi+4]
       fld     dword [eax+8]
       mov     eax, esi
       fstp    dword [esi+8]
       add     esp, 10h
       add     esp, 0Ch
       lea     edx, [esp+2CCh]
       push    0
       push    edx
       mov     byte [esp+3B0h], 0Ch
       call    LVector3f::forward
       push    edi
       lea     esi, [esp+2A4h]
       mov     byte [esp+3B4h], 0Dh
       sub     esp, 10h
       push    eax
       lea     ecx, [esp+8]
       push    ecx
       mov     ecx, [esp+1Ch]
       mov     dword [esp+8], 0
       call    LMatrix4f::xform_point
       fld     dword [eax]
       fstp    dword [esi]
       fld     dword [eax+4]
       fstp    dword [esi+4]
       fld     dword [eax+8]
       mov     eax, esi
       fstp    dword [esi+8]
       add     esp, 10h
       add     esp, 0Ch
       push    0
       lea     eax, [esp+290h]
       push    eax
       mov     ecx, esi
       push    ecx
       lea     edx, [esp+234h]
       push    edx
       mov     byte [esp+3B8h], 0Fh
       call    LVector3f::look_at
       fld     dword [esp+238h]
       fstp    dword [esp+25Ch]
       mov     eax, [ThreadSimpleManager::_pointers_initialized]
       fld     dword [esp+23Ch]
       add     esp, 10h
       cmp     byte [eax], 0
       fstp    dword [esp+250h]
       fld     dword [esp+230h]
       mov     ecx, 10h
       fstp    dword [esp+254h]
       lea     esi, [esp+24Ch]
       fldz    
       lea     edi, [esp+104h]
       fst     dword [esp+258h]
       fld     dword [esp+234h]
       fstp    dword [esp+25Ch]
       fld     dword [esp+238h]
       fstp    dword [esp+260h]
       fld     dword [esp+23Ch]
       fstp    dword [esp+264h]
       fst     dword [esp+268h]
       fld     dword [esp+240h]
       fstp    dword [esp+26Ch]
       fld     dword [esp+244h]
       fstp    dword [esp+270h]
       fld     dword [esp+248h]
       fstp    dword [esp+274h]
       fst     dword [esp+278h]
       fst     dword [esp+27Ch]
       fst     dword [esp+280h]
       fstp    dword [esp+284h]
       fld1    
       fstp    dword [esp+288h]
       rep movsd
       jnz     loc_10010A69
       call    ThreadSimpleManager::init_pointers
    loc_10010A69:
       mov     ecx, [ThreadSimpleManager::_global_ptr]
       mov     eax, [ecx]
       mov     edx, [eax+80h]
       mov     eax, [edx+4]
       test    eax, eax
       jnz     loc_10010A96
       mov     eax, [Thread::_external_thread]
       cmp     dword [eax], 0
       jnz     loc_10010A8E
       call    Thread::init_external_thread
    loc_10010A8E:
       mov     ecx, [Thread::_external_thread]
       mov     eax, [ecx]
    loc_10010A96:
       mov     ecx, [ebp+8]
       push    eax
       push    NametagGlobals::m_camera_nodepath
       lea     edx, [esp+38h]
       push    edx
       call    NodePath::get_transform
       mov     byte [esp+3A8h], 10h
       mov     ecx, [esp+30h]
       test    byte [ecx+0ECh], 40h
       mov     esi, ecx
       jnz     loc_10010AE4
       lea     eax, [ecx+0F0h]
       mov     [esp+0C4h], eax
       mov     byte [esp+3A8h], 11h
       call    TransformState::do_calc_mat
       mov     byte [esp+3A8h], 10h
    loc_10010AE4:
       fld     dword [esi+0D8h]
       lea     eax, [esi+0A4h]
       fstp    dword [esp+64h]
       mov     [esp+1Ch], eax
       fld     dword [esp+64h]
       fcom    0.1000000014901161
       fnstsw  ax
       test    ah, 41h
       jz      loc_10010B11
       fstp    st
       fld     [0.1]
    loc_10010B11:
       fstp    dword [esp+4Ch]
       fld     dword [esp+4Ch]
       fdiv    [50.0]
       fstp    dword [esp+14h]
       fld     dword [esp+14h]
       fld     [0.5]
       call    _CIpow
       fstp    dword [esp+14h]
       fld     dword [esp+14h]
       fstp    dword [esp+14h]
       fld     dword [esp+14h]
       fmul    0.5600000023841858
       fmul    NametagGlobals::_global_nametag_scale
       fstp    dword [esp+4Ch]
       fldz    
       fcomp   dword [ebx+170h]
       fnstsw  ax
       test    ah, 44h
       jnp     loc_10010DB2
       mov     esi, [esp+6Ch]
       mov     byte [esp+3A8h], 12h
       fld     dword [esi]
       fstp    dword [esp+148h]
       fld     dword [esi+4]
       fstp    dword [esp+14Ch]
       fld     dword [esi+8]
       fstp    dword [esp+150h]
       fld     dword [esp+148h]
       fld     dword [esp+14Ch]
       fld     dword [esp+150h]
       fld     st(1)
       fmulp   st(2), st
       fld     st(2)
       fmulp   st(3), st
       fxch    st(1)
       faddp   st(2), st
       fmul    st, st
       faddp   st(1), st
       fstp    dword [esp+14h]
       fld     dword [esp+14h]
       call    _CIsqrt
       fstp    dword [esp+14h]
       fld     dword [esp+14h]
       mov     edi, [esp+7Ch]
       fstp    dword [esp+24h]
       fld     dword [ebx+170h]
       fstp    dword [esp+14h]
       fld     dword [ebx+170h]
       fdiv    dword [esp+24h]
       fstp    dword [esp+84h]
       fldz    
       fcom    dword [esp+64h]
       fnstsw  ax
       test    ah, 5
       jp      loc_10010C9D
       mov     ecx, [PerspectiveLens::_type_handle]
       fstp    st
       mov     edx, [edi]
       mov     esi, [ecx]
       mov     edx, [edx+4]
       lea     eax, [esp+0FCh]
       push    eax
       mov     ecx, edi
       call    edx
       push    edi
       push    ecx
       mov     ecx, esp
       mov     [ecx], esi
       mov     eax, [eax]
       push    ecx
       mov     ecx, esp
       mov     [ecx], eax
       call    TypeRegistry::ptr
       mov     ecx, eax
       call    TypeRegistry::is_derived_from
       test    al, al
       jz      loc_10010C97
       fld     dword [edi+60h]
       fstp    dword [esp+7Ch]
       fld     dword [esp+7Ch]
       fadd    0.001000000047497451
       fld     dword [esp+64h]
       fld     st
       fld     dword [esp+84h]
       fld     st
       fsubp   st(2), st
       fxch    st(1)
       fcomp   st(3)
       fnstsw  ax
       test    ah, 5
       jp      loc_10010CF3
       fstp    st
       fld     st
       fsubrp  st(2), st
       fxch    st(1)
       fstp    dword [esp+84h]
       fld     dword [esp+84h]
       fld     st
       fmul    dword [esp+24h]
       fstp    dword [esp+14h]
       fxch    st(1)
    loc_10010C89:
       fld     st
       fsubrp  st(2), st
       fdivp   st(1), st
       fmul    dword [esp+4Ch]
       fstp    dword [esp+4Ch]
    loc_10010C97:
       mov     esi, [esp+6Ch]
       fldz    
    loc_10010C9D:
       fld     dword [esi+30h]
       fstp    dword [esp+34h]
       fld     dword [esi+34h]
       fstp    dword [esp+38h]
       fld     dword [esi+38h]
       fstp    dword [esp+3Ch]
       fld     dword [esp+34h]
       fld     dword [esp+38h]
       fld     dword [esp+3Ch]
       fld     st(1)
       fmulp   st(2), st
       fld     st(2)
       fmulp   st(3), st
       fxch    st(1)
       faddp   st(2), st
       fmul    st, st
       faddp   st(1), st
       fstp    dword [esp+24h]
       fld     st
       fld     dword [esp+24h]
       fucom   st(1)
       fnstsw  ax
       fstp    st(1)
       test    ah, 44h
       jp      loc_10010CF7
       fstp    st
       fst     dword [esp+34h]
       fst     dword [esp+38h]
       fstp    dword [esp+3Ch]
       jmp     loc_10010D1F
    loc_10010CF3:
       fstp    st(2)
       jmp     loc_10010C89
    loc_10010CF7:
       fstp    st(1)
       fld     [1.0]
       fsubr   st, st(1)
       fld     9.999999949504854e-13
       fcomp   st(1)
       fnstsw  ax
       test    ah, 41h
       jnz     loc_10010D6E
       fcomp   -9.999999949504854e-13
       fnstsw  ax
       test    ah, 41h
       jnz     loc_10010D70
       fstp    st
    loc_10010D1F:
       fld     dword [esp+34h]
       fld     dword [esp+14h]
       fld     st
       fmulp   st(2), st
       fxch    st(1)
       fstp    dword [esp+34h]
       fld     st
       fmul    dword [esp+38h]
       fstp    dword [esp+38h]
       fmul    dword [esp+3Ch]
       fstp    dword [esp+3Ch]
       fld     dword [esp+34h]
       fstp    dword [esp+134h]
       fld     dword [esp+38h]
       fstp    dword [esp+138h]
       fld     dword [esp+3Ch]
       mov     byte [esp+3A8h], 10h
       fstp    dword [esp+13Ch]
       jmp     loc_10010DB6
    loc_10010D6E:
       fstp    st
    loc_10010D70:
       call    _CIsqrt
       fstp    dword [esp+24h]
       fld     dword [esp+24h]
       fstp    dword [esp+24h]
       fld     dword [esp+24h]
       fld1    
       fdivrp  st(1), st
       fstp    dword [esp+24h]
       fld     dword [esp+24h]
       fld     st
       fmul    dword [esp+34h]
       fstp    dword [esp+34h]
       fld     st
       fmul    dword [esp+38h]
       fstp    dword [esp+38h]
       fmul    dword [esp+3Ch]
       fstp    dword [esp+3Ch]
       jmp     loc_10010D1F
    loc_10010DB2:
       mov     edi, [esp+7Ch]
    loc_10010DB6:
       fld     dword [esp+114h]
       fldz    
       fmul    st(1), st
       fxch    st(1)
       fst     qword [esp+64h]
       fld     dword [esp+124h]
       fmul    st, st(2)
       fld     dword [esp+134h]
       fmul    st, st(3)
       fld     dword [esp+104h]
       fld     dword [esp+4Ch]
       fld     st
       fmulp   st(2), st
       fxch    st(1)
       faddp   st(4), st
       fxch    st(3)
       fadd    st, st(2)
       fadd    st, st(1)
       fstp    dword [esp+194h]
       fld     dword [esp+118h]
       fmul    st, st(4)
       fstp    qword [esp+4Ch]
       fld     dword [esp+128h]
       fmul    st, st(4)
       fld     dword [esp+138h]
       fmul    st, st(5)
       fld     st(4)
       fmul    dword [esp+108h]
       fadd    qword [esp+4Ch]
       fadd    st, st(2)
       fadd    st, st(1)
       fstp    dword [esp+198h]
       fld     dword [esp+11Ch]
       fmul    st, st(6)
       fstp    qword [esp+84h]
       fld     dword [esp+12Ch]
       fmul    st, st(6)
       fstp    qword [esp+0F4h]
       fld     dword [esp+13Ch]
       fmul    st, st(6)
       fstp    qword [esp+0C4h]
       fld     dword [esp+10Ch]
       fmul    st, st(5)
       fadd    qword [esp+84h]
       fadd    qword [esp+0F4h]
       fadd    qword [esp+0C4h]
       fstp    dword [esp+19Ch]
       fld     dword [esp+120h]
       fmul    st, st(6)
       fstp    qword [esp+7Ch]
       fld     dword [esp+130h]
       fmul    st, st(6)
       fstp    qword [esp+0FCh]
       fld     dword [esp+140h]
       fmul    st, st(6)
       fstp    qword [esp+34h]
       fld     dword [esp+110h]
       fmul    st, st(5)
       fadd    qword [esp+7Ch]
       fadd    qword [esp+0FCh]
       fadd    qword [esp+34h]
       fstp    dword [esp+1A0h]
       fld     dword [esp+104h]
       fmul    st, st(6)
       fstp    qword [esp+14h]
       fld     st(4)
       fmul    dword [esp+114h]
       fadd    qword [esp+14h]
       fadd    st, st(4)
       fadd    st, st(3)
       fstp    dword [esp+1A4h]
       fld     dword [esp+108h]
       fmul    st, st(6)
       fstp    qword [esp+24h]
       fld     st(4)
       fmul    dword [esp+118h]
       fadd    qword [esp+24h]
       fadd    st, st(2)
       fadd    st, st(1)
       fstp    dword [esp+1A8h]
       fld     dword [esp+10Ch]
       fmul    st, st(6)
       lea     ecx, [esp+194h]
       push    ecx
       lea     ecx, [ebx+174h]
       fstp    qword [esp+70h]
       fld     dword [esp+120h]
       fmul    st, st(5)
       fadd    qword [esp+70h]
       fadd    qword [esp+0F8h]
       fld     qword [esp+0C8h]
       fadd    st(1), st
       fxch    st(1)
       fstp    dword [esp+1B0h]
       fld     dword [esp+114h]
       fmulp   st(7), st
       fxch    st(6)
       fstp    qword [esp+0C8h]
       fld     dword [esp+124h]
       fmul    st, st(5)
       fadd    qword [esp+0C8h]
       fadd    qword [esp+100h]
       fadd    qword [esp+38h]
       fstp    dword [esp+1B4h]
       fld     qword [esp+18h]
       fadd    qword [esp+68h]
       fstp    qword [esp+18h]
       fld     st(4)
       fmul    dword [esp+128h]
       fadd    qword [esp+18h]
       faddp   st(3), st
       fxch    st(2)
       fstp    dword [esp+1B8h]
       fld     qword [esp+28h]
       fadd    qword [esp+50h]
       fld     st(4)
       fmul    dword [esp+12Ch]
       fadd    st, st(1)
       faddp   st(3), st
       fxch    st(2)
       fstp    dword [esp+1BCh]
       fld     qword [esp+70h]
       fadd    qword [esp+88h]
       fld     dword [esp+130h]
       fmul    st, st(5)
       fadd    st, st(1)
       faddp   st(6), st
       fxch    st(5)
       fstp    dword [esp+1C0h]
       fld     qword [esp+80h]
       fadd    qword [esp+0C8h]
       fld     dword [esp+134h]
       fmulp   st(5), st
       fadd    st(4), st
       fxch    st(4)
       fadd    qword [esp+38h]
       fstp    dword [esp+1C4h]
       fxch    st(2)
       fadd    qword [esp+18h]
       fadd    dword [esp+138h]
       fstp    dword [esp+1C8h]
       faddp   st(1), st
       fadd    dword [esp+13Ch]
       fstp    dword [esp+1CCh]
       fld     qword [esp+0F8h]
       faddp   st(2), st
       fld     dword [esp+140h]
       faddp   st(2), st
       fxch    st(1)
       fstp    dword [esp+1D0h]
       fadd    qword [esp+100h]
       fadd    dword [esp+144h]
       fstp    dword [esp+1D4h]
       call    NodePath::set_mat
       mov     ecx, [ebx+20h]
       mov     al, [ebx+1Ch]
       mov     byte [esp+12h], 0
       test    ecx, ecx
       jz      loc_1001108E
       test    al, al
       jz      loc_100110F5
       call    NametagGroup::display_as_active
       test    al, al
       jnz     loc_1001109B
       jmp     loc_100110F5
    loc_1001108E:
       test    al, al
       jz      loc_100110F5
       cmp     [NametagGlobals::_master_nametags_active], 0
       jz      loc_100110F5
    loc_1001109B:
       test    byte [ebx+0Ch], 6
       jz      loc_100110F0
       test    ecx, ecx
       jz      loc_100110F0
       cmp     byte [ecx+128h], 0
       jnz     loc_100110F0
       mov     eax, [ecx+0D8h]
       test    al, 30h
       jz      loc_100110F0
       test    eax, 100h
       jz      loc_100110F5
       mov     esi, [ecx+0D0h]
       sub     esi, [ecx+0CCh]
       mov     edx, [ecx+124h]
       mov     [esp+14h], edx
       mov     eax, 92492493h
       imul    esi
       add     edx, esi
       sar     edx, 4
       mov     eax, edx
       shr     eax, 1Fh
       lea     ecx, [edx+eax-1]
       cmp     [esp+14h], ecx
       jl      loc_100110F5
    loc_100110F0:
       mov     byte [esp+12h], 1
    loc_100110F5:
       push    0
       mov     ecx, edi
       mov     byte [esp+3ACh], 13h
       mov     byte [esp+17h], 0
       mov     dword [esp+28h], 0
       call    Lens::get_projection_mat
       cmp     byte [esp+12h], 0
       mov     edi, eax
       mov     [esp+14h], edi
       jz      loc_10011710
       fld     -0.5
       fstp    dword [esp+0A4h]
       fldz    
       fst     dword [esp+0A8h]
       fld1    
       fst     dword [esp+0ACh]
       fld     2.5
       fstp    dword [esp+58h]
       fxch    st(1)
       fstp    dword [esp+5Ch]
       fstp    dword [esp+60h]
       lea     edx, [esp+0A4h]
       push    edx
       lea     eax, [esp+2F4h]
       push    eax
       lea     ecx, [esp+19Ch]
       mov     byte [esp+3B0h], 15h
       call    LMatrix4f::xfrom_vec
       fld     dword [eax]
       fstp    dword [esp+0A4h]
       lea     ecx, [esp+58h]
       fld     dword [eax+4]
       push    ecx
       fstp    dword [esp+0ACh]
       lea     edx, [esp+30Ch]
       fld     dword [eax+8]
       push    edx
       lea     ecx, [esp+19Ch]
       fstp    dword [esp+0B4h]
       call    LMatrix4f::xfrom_vec
       fld     dword [eax]
       fstp    dword [esp+58h]
       fld     dword [eax+4]
       fstp    dword [esp+5Ch]
       fld     dword [eax+8]
       mov     eax, [ThreadSimpleManager::_pointers_initialized]
       fstp    dword [esp+60h]
       cmp     byte [eax], 0
       jnz     loc_100111D8
       call    [ThreadSimpleManager::init_pointers]
    loc_100111D8:
       mov     ecx, [ThreadSimpleManager::_global_ptr]
       mov     eax, [ecx]
       mov     edx, [eax+80h]
       mov     eax, [edx+4]
       test    eax, eax
       jnz     loc_10011205
       mov     eax, [Thread::_external_thread]
       cmp     dword [eax], 0
       jnz     loc_100111FD
       call    Thread::init_external_thread
    loc_100111FD:
       mov     ecx, [Thread::_external_thread]
       mov     eax, [ecx]
    loc_10011205:
       mov     edx, [esp+78h]
       mov     ecx, [ebp+8]
       push    eax
       push    edx
       lea     eax, [esp+50h]
       push    eax
       call    NodePath::get_transform
       mov     byte [esp+3A8h], 16h
       mov     ecx, [esp+48h]
       mov     eax, [ecx+0ECh]
       mov     edx, eax
       shr     edx, 7
       not     dl
       test    dl, 1
       jnz     loc_100112B0
       xor     edi, edi
       mov     esi, ecx
       mov     byte [esp+3A8h], 17h
       mov     [esp+48h], edi
       cmp     esi, edi
       jz      loc_10011264
       mov     eax, [esi+0Ch]
       mov     edx, [eax+4]
       lea     ecx, [esi+0Ch]
       call    edx
       test    al, al
       jnz     loc_10011264
       mov     eax, [esi]
       mov     edx, [eax]
       push    1
       mov     ecx, esi
       call    edx
    loc_10011264:
       mov     byte [esp+3A8h], 18h
       mov     esi, [esp+30h]
       cmp     esi, edi
       jz      loc_10011290
       lea     ecx, [esi+0Ch]
       mov     [esp+30h], edi
       mov     eax, [ecx]
       mov     edx, [eax+4]
       call    edx
       test    al, al
       jnz     loc_10011290
       mov     eax, [esi]
       mov     edx, [eax]
       push    1
       mov     ecx, esi
       call    edx
    loc_10011290:
       mov     dword [esp+3A8h], 19h
       mov     esi, [esp+20h]
       cmp     esi, edi
       jz      loc_10011CBB
       mov     [esp+20h], edi
       jmp     loc_100105F9
    loc_100112B0:
       mov     esi, ecx
       test    al, 40h
       jnz     loc_100112D9
       lea     eax, [ecx+0F0h]
       mov     [esp+0F4h], eax
       mov     byte [esp+3A8h], 1Ah
       call    TransformState::do_calc_mat
       mov     byte [esp+3A8h], 16h
    loc_100112D9:
       lea     edi, [esi+0A4h]
       push    edi
       lea     eax, [esp+0A8h]
       lea     esi, [esp+348h]
       sub     esp, 10h
       push    eax
       lea     ecx, [esp+8]
       push    ecx
       mov     ecx, [esp+1Ch]
       mov     dword [esp+8], 0
       call    LMatrix4f::xform_point
       fld     dword [eax]
       fstp    dword [esi]
       fld     dword [eax+4]
       fstp    dword [esi+4]
       fld     dword [eax+8]
       mov     eax, esi
       fstp    dword [esi+8]
       add     esp, 10h
       fld     dword [eax]
       fstp    dword [esp+0A8h]
       add     esp, 4
       fld     dword [eax+4]
       push    edi
       fstp    dword [esp+0ACh]
       lea     esi, [esp+300h]
       fld     dword [eax+8]
       lea     eax, [esp+5Ch]
       fstp    dword [esp+0B0h]
       sub     esp, 10h
       push    eax
       lea     ecx, [esp+8]
       push    ecx
       mov     ecx, [esp+1Ch]
       mov     dword [esp+8], 0
       call    LMatrix4f::xform_point
       fld     dword [eax]
       fstp    dword [esi]
       fld     dword [eax+4]
       fstp    dword [esi+4]
       fld     dword [eax+8]
       mov     eax, esi
       fstp    dword [esi+8]
       add     esp, 10h
       fld     dword [eax]
       add     esp, 4
       fstp    dword [esp+58h]
       fld     dword [eax+4]
       fstp    dword [esp+5Ch]
       fld     dword [eax+8]
       fstp    dword [esp+60h]
       fld     dword [esp+0A4h]
       fstp    dword [esp+0D0h]
       fld     dword [esp+0A8h]
       fstp    dword [esp+0D4h]
       fldz    
       fstp    dword [esp+0D8h]
       mov     byte [esp+3A8h], 1Bh
       mov     ecx, [ThreadSimpleManager::_pointers_initialized]
       cmp     byte [ecx], 0
       jnz     loc_10011379
       call    ThreadSimpleManager::init_pointers
    loc_10011379:
       mov     edx, [ThreadSimpleManager::_global_ptr]
       mov     eax, [edx]
       mov     eax, [eax+80h]
       mov     eax, [eax+4]
       test    eax, eax
       jnz     loc_100113A6
       mov     ecx, [Thread::_external_thread]
       cmp     [ecx], eax
       jnz     loc_1001139E
       call    Thread::init_external_thread
    loc_1001139E:
       mov     edx, [Thread::_external_thread]
       mov     eax, [edx]
    loc_100113A6:
       mov     ecx, [esp+78h]
       push    eax
       push    NametagGlobals::m_camera_nodepath
       lea     eax, [esp+5Ch]
       push    eax
       call    NodePath::get_transform
       mov     byte [esp+3A8h], 1Ch
       mov     ecx, [esp+54h]
       mov     eax, [ecx+0ECh]
       mov     edx, eax
       shr     edx, 7
       not     dl
       test    dl, 1
       jnz     loc_10011482
       xor     edi, edi
       mov     esi, ecx
       mov     byte [esp+3A8h], 1Dh
       mov     [esp+54h], edi
       cmp     esi, edi
       jz      loc_1001140A
       mov     eax, [esi+0Ch]
       mov     edx, [eax+4]
       lea     ecx, [esi+0Ch]
       call    edx
       test    al, al
       jnz     loc_1001140A
       mov     eax, [esi]
       mov     edx, [eax]
       push    1
       mov     ecx, esi
       call    edx
    loc_1001140A:
       mov     byte [esp+3A8h], 1Eh
       mov     esi, [esp+48h]
       cmp     esi, edi
       jz      loc_10011436
       lea     ecx, [esi+0Ch]
       mov     [esp+48h], edi
       mov     eax, [ecx]
       mov     edx, [eax+4]
       call    edx
       test    al, al
       jnz     loc_10011436
       mov     eax, [esi]
       mov     edx, [eax]
       push    1
       mov     ecx, esi
       call    edx
    loc_10011436:
       mov     byte [esp+3A8h], 1Fh
       mov     esi, [esp+30h]
       cmp     esi, edi
       jz      loc_10011462
       lea     ecx, [esi+0Ch]
       mov     [esp+30h], edi
       mov     eax, [ecx]
       mov     edx, [eax+4]
       call    edx
       test    al, al
       jnz     loc_10011462
       mov     eax, [esi]
       mov     edx, [eax]
       push    1
       mov     ecx, esi
       call    edx
    loc_10011462:
       mov     dword [esp+3A8h], 20h
       mov     esi, [esp+20h]
       cmp     esi, edi
       jz      loc_10011CBB
       mov     [esp+20h], edi
       jmp     loc_100105F9
    loc_10011482:
       mov     edi, ecx
       test    al, 40h
       jnz     loc_100114A8
       lea     eax, [ecx+0F0h]
       mov     [esp+34h], eax
       mov     byte [esp+3A8h], 21h
       call    TransformState::do_calc_mat
       mov     byte [esp+3A8h], 1Ch
    loc_100114A8:
       add     edi, 0A4h
       push    edi
       lea     eax, [esp+5Ch]
       lea     esi, [esp+330h]
       sub     esp, 10h
       push    eax
       lea     ecx, [esp+8]
       push    ecx
       mov     ecx, [esp+1Ch]
       mov     dword [esp+8], 0
       call    LMatrix4f::xform_point
       fld     dword [eax]
       fstp    dword [esi]
       fld     dword [eax+4]
       fstp    dword [esi+4]
       fld     dword [eax+8]
       mov     eax, esi
       fstp    dword [esi+8]
       add     esp, 10h
       fld     dword [eax]
       fstp    dword [esp+5Ch]
       add     esp, 4
       fld     dword [eax+4]
       push    edi
       fstp    dword [esp+60h]
       lea     esi, [esp+318h]
       fld     dword [eax+8]
       lea     eax, [esp+0D4h]
       fstp    dword [esp+64h]
       sub     esp, 10h
       push    eax
       lea     ecx, [esp+8]
       push    ecx
       mov     ecx, [esp+1Ch]
       mov     dword [esp+8], 0
       call    LMatrix4f::xform_point
       fld     dword [eax]
       fstp    dword [esi]
       fld     dword [eax+4]
       fstp    dword [esi+4]
       fld     dword [eax+8]
       mov     eax, esi
       fstp    dword [esi+8]
       add     esp, 10h
       fld     dword [eax]
       add     esp, 4
       fstp    dword [esp+0D0h]
       fld     dword [eax+4]
       fstp    dword [esp+0D4h]
       fld     dword [eax+8]
       fstp    dword [esp+0D8h]
       fld     dword [esp+58h]
       fstp    dword [esp+1D8h]
       fld     dword [esp+5Ch]
       fstp    dword [esp+1DCh]
       fld     dword [esp+60h]
       fstp    dword [esp+1E0h]
       fld1    
       fstp    dword [esp+1E4h]
       mov     esi, [esp+14h]
       lea     ecx, [esp+1D8h]
       push    ecx
       lea     edx, [esp+360h]
       push    edx
       mov     ecx, esi
       mov     byte [esp+3B0h], 22h
       call    LMatrix4f::xfrom
       fld     dword [eax]
       fstp    dword [esp+154h]
       fld     dword [eax+4]
       fstp    dword [esp+158h]
       fld     dword [eax+8]
       fstp    dword [esp+15Ch]
       fld     dword [eax+0Ch]
       fstp    dword [esp+160h]
       fld     dword [esp+0D0h]
       fstp    dword [esp+218h]
       fld     dword [esp+0D4h]
       fstp    dword [esp+21Ch]
       fld     dword [esp+0D8h]
       fstp    dword [esp+220h]
       fld1    
       fstp    dword [esp+224h]
       lea     eax, [esp+218h]
       push    eax
       lea     ecx, [esp+390h]
       push    ecx
       mov     ecx, esi
       mov     byte [esp+3B0h], 25h
       call    LMatrix4f::xfrom
       fld     dword [eax]
       fstp    dword [esp+164h]
       fld     dword [eax+4]
       fstp    dword [esp+168h]
       fld     dword [eax+8]
       fstp    dword [esp+16Ch]
       fld     dword [eax+0Ch]
       fstp    dword [esp+170h]
       mov     byte [esp+3A8h], 27h
       fldz    
       fld     dword [esp+170h]
       fcom    st(1)
       fnstsw  ax
       test    ah, 41h
       jnp     loc_10011A3A
       fld     dword [esp+160h]
       fcom    st(2)
       fnstsw  ax
       fstp    st(2)
       test    ah, 41h
       jnp     loc_10011A3A
       fld1    
       mov     byte [esp+13h], 1
       fld     st
       fdivrp  st(2), st
       fxch    st(1)
       fstp    dword [esp+6Ch]
       fdivrp  st(1), st
       fstp    dword [esp+4Ch]
       fld     dword [esp+4Ch]
       fld     st
       fmul    dword [esp+158h]
       fstp    dword [esp+84h]
       fld     dword [esp+6Ch]
       fld     st
       fmul    dword [esp+168h]
       fstp    dword [esp+6Ch]
       fld     dword [esp+154h]
       fmulp   st(2), st
       fxch    st(1)
       fstp    dword [esp+4Ch]
       fmul    dword [esp+164h]
       fstp    dword [esp+0B0h]
       fld     dword [esp+4Ch]
       fstp    dword [esp+0B4h]
       fld     dword [esp+6Ch]
       fstp    dword [esp+0B8h]
       fld     dword [esp+84h]
       fstp    dword [esp+0BCh]
       mov     byte [esp+3A8h], 2Ch
       mov     esi, [esp+54h]
       xor     edi, edi
       cmp     esi, edi
       jz      loc_100116D8
       lea     ecx, [esi+0Ch]
       mov     [esp+54h], edi
       mov     edx, [ecx]
       mov     eax, [edx+4]
       call    eax
       test    al, al
       jnz     loc_100116D8
       mov     edx, [esi]
       mov     eax, [edx]
       push    1
       mov     ecx, esi
       call    eax
    loc_100116D8:
       mov     byte [esp+3A8h], 2Dh
       mov     esi, [esp+48h]
       cmp     esi, edi
       jz      loc_10011704
       lea     ecx, [esi+0Ch]
       mov     [esp+48h], edi
       mov     edx, [ecx]
       mov     eax, [edx+4]
       call    eax
       test    al, al
       jnz     loc_10011704
       mov     edx, [esi]
       mov     eax, [edx]
       push    1
       mov     ecx, esi
       call    eax
    loc_10011704:
       mov     edi, [esp+14h]
       mov     byte [esp+3A8h], 13h
    loc_10011710:
       cmp     byte [ebx+1A4h], 0
       mov     byte [esp+12h], 0
       jz      loc_10011C01
       fld     dword [ebx+1B0h]
       fld1    
       fsub    st(1), st
       fxch    st(1)
       fstp    dword [esp+14h]
       fld     dword [ebx+1A8h]
       fld     0.5
       fsub    st(1), st
       fxch    st(1)
       fstp    dword [esp+8Ch]
       fldz    
       fst     dword [esp+90h]
       fld     dword [esp+14h]
       fstp    dword [esp+94h]
       fld     dword [ebx+1B4h]
       faddp   st(3), st
       fxch    st(2)
       fstp    dword [esp+14h]
       fadd    dword [ebx+1ACh]
       fstp    dword [esp+98h]
       fstp    dword [esp+9Ch]
       fld     dword [esp+14h]
       fstp    dword [esp+0A0h]
       lea     ecx, [esp+8Ch]
       push    ecx
       lea     edx, [esp+2DCh]
       push    edx
       lea     ecx, [esp+19Ch]
       mov     byte [esp+3B0h], 2Fh
       call    LMatrix4f::xfrom_point
       fld     dword [eax]
       fstp    dword [esp+8Ch]
       lea     ecx, [esp+2C0h]
       fld     dword [eax+4]
       fstp    dword [esp+90h]
       fld     dword [eax+8]
       lea     eax, [esp+98h]
       push    eax
       fstp    dword [esp+98h]
       push    ecx
       lea     ecx, [esp+19Ch]
       call    LMatrix4f::xfrom_point
       fld     dword [eax]
       mov     edx, [esp+1Ch]
       fstp    dword [esp+98h]
       fld     dword [eax+4]
       push    edx
       fstp    dword [esp+0A0h]
       lea     esi, [esp+324h]
       fld     dword [eax+8]
       lea     eax, [esp+90h]
       fstp    dword [esp+0A4h]
       sub     esp, 10h
       push    eax
       lea     ecx, [esp+8]
       push    ecx
       mov     ecx, [esp+1Ch]
       mov     dword [esp+8], 0
       call    LMatrix4f::xform_point
       fld     dword [eax]
       fstp    dword [esi]
       fld     dword [eax+4]
       fstp    dword [esi+4]
       fld     dword [eax+8]
       mov     eax, esi
       fstp    dword [esi+8]
       add     esp, 10h
       fld     dword [eax]
       add     esp, 4
       fstp    dword [esp+8Ch]
       lea     esi, [esp+338h]
       fld     dword [eax+4]
       fstp    dword [esp+90h]
       fld     dword [eax+8]
       mov     eax, [esp+1Ch]
       push    eax
       fstp    dword [esp+98h]
       lea     eax, [esp+9Ch]
       sub     esp, 10h
       push    eax
       lea     ecx, [esp+8]
       push    ecx
       mov     ecx, [esp+1Ch]
       mov     dword [esp+8], 0
       call    LMatrix4f::xform_point
       fld     dword [eax]
       fstp    dword [esi]
       fld     dword [eax+4]
       fstp    dword [esi+4]
       fld     dword [eax+8]
       mov     eax, esi
       fstp    dword [esi+8]
       add     esp, 10h
       fld     dword [eax]
       fstp    dword [esp+9Ch]
       add     esp, 4
       fld     dword [eax+4]
       fstp    dword [esp+9Ch]
       fld     dword [eax+8]
       fstp    dword [esp+0A0h]
       fld     dword [esp+98h]
       fstp    dword [esp+208h]
       fld     dword [esp+9Ch]
       fstp    dword [esp+20Ch]
       fld     dword [esp+0A0h]
       fstp    dword [esp+210h]
       fld1    
       fstp    dword [esp+214h]
       lea     ecx, [esp+208h]
       push    ecx
       lea     edx, [esp+380h]
       push    edx
       mov     ecx, edi
       mov     byte [esp+3B0h], 30h
       call    LMatrix4f::xfrom
       fld     dword [eax]
       fstp    dword [esp+174h]
       fld     dword [eax+4]
       fstp    dword [esp+178h]
       fld     dword [eax+8]
       fstp    dword [esp+17Ch]
       fld     dword [eax+0Ch]
       fstp    dword [esp+180h]
       fld     dword [esp+8Ch]
       fstp    dword [esp+1E8h]
       fld     dword [esp+90h]
       fstp    dword [esp+1ECh]
       fld     dword [esp+94h]
       fstp    dword [esp+1F0h]
       fld1    
       fstp    dword [esp+1F4h]
       lea     eax, [esp+1E8h]
       push    eax
       lea     ecx, [esp+370h]
       push    ecx
       mov     ecx, edi
       mov     byte [esp+3B0h], 33h
       call    LMatrix4f::xfrom
       fld     dword [eax]
       fstp    dword [esp+184h]
       fld     dword [eax+4]
       fstp    dword [esp+188h]
       fld     dword [eax+0Ch]
       fstp    dword [esp+190h]
       fldz    
       fld     dword [esp+190h]
       fcom    st(1)
       fnstsw  ax
       test    ah, 41h
       jnp     loc_10011BF0
       fld     dword [esp+180h]
       fcom    st(2)
       fnstsw  ax
       fstp    st(2)
       test    ah, 41h
       jnp     loc_10011BF0
       fld1    
       fld     st
       fdivrp  st(2), st
       fxch    st(1)
       fstp    dword [esp+14h]
       fdivrp  st(1), st
       fstp    dword [esp+1Ch]
       fld     dword [esp+1Ch]
       fld     st
       fmul    dword [esp+178h]
       fstp    dword [esp+1Ch]
       fld     dword [esp+14h]
       fld     st
       fmul    dword [esp+188h]
       fstp    dword [esp+24h]
       fld     dword [esp+174h]
       fmulp   st(2), st
       fxch    st(1)
       fstp    dword [esp+14h]
       fmul    dword [esp+184h]
       fstp    dword [esp+2A4h]
       fld     [-1.0]
       fld     dword [esp+2A4h]
       fcom    st(1)
       fnstsw  ax
       fld     dword [esp+1Ch]
       fld     dword [esp+14h]
       fld     dword [esp+24h]
       test    ah, 5
       jnp     loc_10011B0A
       fld1    
       fcom    st(2)
       fnstsw  ax
       test    ah, 5
       jnp     loc_10011B08
       fxch    st(5)
       fcomp   st(1)
       fnstsw  ax
       test    ah, 41h
       jz      loc_10011B0A
       fxch    st(4)
       fcomp   st(2)
       fnstsw  ax
       test    ah, 5
       jnp     loc_10011B0C
       mov     byte [esp+12h], 0
       jmp     loc_10011B11
    loc_10011A3A:
       mov     esi, [esp+0C0h]
       fstp    st(1)
       mov     eax, [esi+14Ch]
       fstp    st
       xor     ecx, ecx
       xor     edi, edi
       cmp     eax, edi
       setle   cl
       dec     ecx
       and     eax, ecx
       mov     ecx, ebx
       mov     [esi+14Ch], eax
       call    Nametag::clear_region
       mov     byte [esp+3A8h], 28h
       mov     esi, [esp+54h]
       cmp     esi, edi
       jz      loc_10011A90
       lea     ecx, [esi+0Ch]
       mov     [esp+54h], edi
       mov     edx, [ecx]
       mov     eax, [edx+4]
       call    eax
       test    al, al
       jnz     loc_10011A90
       mov     edx, [esi]
       mov     eax, [edx]
       push    1
       mov     ecx, esi
       call    eax
    loc_10011A90:
       mov     byte [esp+3A8h], 29h
       mov     esi, [esp+48h]
       cmp     esi, edi
       jz      loc_10011ABC
       lea     ecx, [esi+0Ch]
       mov     [esp+48h], edi
       mov     edx, [ecx]
       mov     eax, [edx+4]
       call    eax
       test    al, al
       jnz     loc_10011ABC
       mov     edx, [esi]
       mov     eax, [edx]
       push    1
       mov     ecx, esi
       call    eax
    loc_10011ABC:
       mov     byte [esp+3A8h], 2Ah
       mov     esi, [esp+30h]
       cmp     esi, edi
       jz      loc_10011AE8
       lea     ecx, [esi+0Ch]
       mov     [esp+30h], edi
       mov     edx, [ecx]
       mov     eax, [edx+4]
       call    eax
       test    al, al
       jnz     loc_10011AE8
       mov     edx, [esi]
       mov     eax, [edx]
       push    1
       mov     ecx, esi
       call    eax
    loc_10011AE8:
       mov     dword [esp+3A8h], 2Bh
       mov     esi, [esp+20h]
       cmp     esi, edi
       jz      loc_10011CBB
       mov     [esp+20h], edi
       jmp     loc_10011CA2
    loc_10011B08:
       fstp    st(5)
    loc_10011B0A:
       fstp    st(4)
    loc_10011B0C:
       mov     byte [esp+12h], 1
    loc_10011B11:
       cmp     byte [esp+13h], 0
       jz      loc_10011BB1
       fld     dword [esp+0BCh]
       fcom    st(2)
       fnstsw  ax
       test    ah, 41h
       jnz     loc_10011B30
       fstp    st(2)
       jmp     loc_10011B32
    loc_10011B30:
       fstp    st
    loc_10011B32:
       fxch    st(1)
       fstp    dword [esp+4Ch]
       fld     dword [esp+0B8h]
       fcom    st(3)
       fnstsw  ax
       test    ah, 5
       jp      loc_10011B4C
       fstp    st(3)
       jmp     loc_10011B4E
    loc_10011B4C:
       fstp    st
    loc_10011B4E:
       fxch    st(2)
       fstp    dword [esp+24h]
       fld     dword [esp+0B4h]
       fcom    st(2)
       fnstsw  ax
       test    ah, 41h
       jnz     loc_10011B68
       fstp    st(2)
       jmp     loc_10011B6A
    loc_10011B68:
       fstp    st
    loc_10011B6A:
       fxch    st(1)
       fstp    dword [esp+14h]
       fld     dword [esp+0B0h]
       fcom    st(1)
       fnstsw  ax
       test    ah, 5
       jp      loc_10011B84
       fstp    st(1)
       jmp     loc_10011B86
    loc_10011B84:
       fstp    st
    loc_10011B86:
       fstp    dword [esp+1Ch]
       fld     dword [esp+1Ch]
       fstp    dword [esp+0B0h]
       fld     dword [esp+14h]
       fstp    dword [esp+0B4h]
       fld     dword [esp+24h]
       fstp    dword [esp+0B8h]
       fld     dword [esp+4Ch]
       jmp     loc_10011BD1
    loc_10011BB1:
       fxch    st(2)
       mov     byte [esp+13h], 1
       fstp    dword [esp+0B0h]
       fxch    st(1)
       fstp    dword [esp+0B4h]
       fxch    st(1)
       fstp    dword [esp+0B8h]
    loc_10011BD1:
       fstp    dword [esp+0BCh]
       fld     dword [esp+90h]
       fmul    -100.0
       call    __ftol2_sse
       mov     [esp+24h], eax
       jmp     loc_10011BF9
    loc_10011BF0:
       fstp    st
       mov     byte [esp+12h], 1
       fstp    st
    loc_10011BF9:
       mov     byte [esp+3A8h], 13h
    loc_10011C01:
       cmp     byte [esp+13h], 0
       jz      loc_10011C27
       mov     ecx, ebx
       call    Nametag::display_as_active
       test    al, al
       jz      loc_10011C27
       mov     edx, [esp+24h]
       push    edx
       lea     eax, [esp+0B4h]
       push    eax
       mov     ecx, ebx
       call    Nametag::set_region
    loc_10011C27:
       cmp     byte [esp+12h], 0
       mov     ecx, [esp+0C0h]
       mov     eax, [ecx+14Ch]
       jz      loc_10011C47
       cmp     eax, 1
       jg      loc_10011C51
       mov     eax, 1
       jmp     loc_10011C51
    loc_10011C47:
       cmp     eax, 2
       jg      loc_10011C51
       mov     eax, 2
    loc_10011C51:
       mov     [ecx+14Ch], eax
       mov     byte [esp+3A8h], 34h
       mov     esi, [esp+30h]
       test    esi, esi
       jz      loc_10011C87
       lea     ecx, [esi+0Ch]
       mov     dword [esp+30h], 0
       mov     edx, [ecx]
       mov     eax, [edx+4]
       call    eax
       test    al, al
       jnz     loc_10011C87
       mov     edx, [esi]
       mov     eax, [edx]
       push    1
       mov     ecx, esi
       call    eax
    loc_10011C87:
       mov     dword [esp+3A8h], 35h
       mov     esi, [esp+20h]
       test    esi, esi
       jz      loc_10011CBB
       mov     dword [esp+20h], 0
    loc_10011CA2:
       mov     edx, [esi+0Ch]
       mov     eax, [edx+4]
       lea     ecx, [esi+0Ch]
       call    eax
       test    al, al
       jnz     loc_10011CBB
       mov     edx, [esi]
       mov     eax, [edx]
       push    1
       mov     ecx, esi
       call    eax
    loc_10011CBB:
       mov     ecx, [esp+3A0h]
       mov     [fs:0], ecx
       pop     ecx
       pop     edi
       pop     esi
       pop     ebx
       mov     esp, ebp
       pop     ebp
       retn    8};
    **/
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