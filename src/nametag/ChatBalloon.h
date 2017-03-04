#pragma once

#include "util.h"
#include <pandabase.h>
#include <nodePath.h>
#include <textNode.h>
#include <luse.h>
#include <typedObject.h>

class ChatBalloon : public TypedObject
{
    PUBLISHED:
        ChatBalloon(NodePath& model);
        ~ChatBalloon();
        
        NodePath generate(const std::wstring& text, PT(TextFont) font, LVecBase4* text_color=NULL, LVecBase4* balloon_color=NULL, float wordwrap=10.0, NodePath* button=NULL);
        
    private:
        NodePath& m_model;
        
    TYPE_HANDLE(ChatBalloon, TypedObject);
};