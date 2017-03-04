#pragma once

#include "util.h"
#include "Nametag.h"

#include <pandabase.h>

class Nametag3d;

class EXPCL_LIBOTP Nametag3d : public virtual Nametag {
    
    PUBLISHED:
        Nametag3d();
        ~Nametag3d();
        
    public:
        virtual void tick();
        void set_billboard_offset(float offset);
        
    protected:
        virtual ChatBalloon* get_speech_balloon();
        virtual ChatBalloon* get_thought_balloon();
        virtual void billboard();
        
        float m_bb_offset;
    
    TYPE_HANDLE(Nametag3d, TypedObject);
};