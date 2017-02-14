#pragma once

#include "util.h"
#include <nodepath.h>
#include <pandanode.h>
#include <mouseWatcher.h>
#include <typedObject.h>
#include <pandaNode.h>
#include <pandabase.h>

class Nametag;

class EXPCL_LIBOTP Nametag : public TypedObject {
    
    PUBLISHED:
        Nametag();
        ~Nametag();

        void set_draw_order(uint8_t draw_order);
        void clear_draw_order();
        void set_contents(unsigned int contents);
        void set_active(bool active);
        void set_chat_wordwrap(uint16_t chat_wordwrap);
        void set_avatar(NodePath *avatar);
        void clear_avatar();

        uint8_t get_draw_order();
        unsigned int get_contents();
        bool get_active();
        uint16_t get_chat_wordwrap();
        NodePath *get_avatar();

    public:
        uint8_t CSpeech;
        uint8_t CThought;
        uint8_t CName;
        uint8_t m_draw_order;
        unsigned int m_contents;
        bool m_active;
        uint16_t m_chat_wordwrap;
        NodePath *m_avatar;

    private:
        //PandaNode m_pandaNode;

    TYPE_HANDLE(NametagGlobals, TypedObject);
};