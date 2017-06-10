#pragma once

#include "util.h"
#include "Nametag.h"
#include <notifyCategoryProxy.h>

#include <pandabase.h>

NotifyCategoryDecl(Nametag3d, EXPCL_LIBOTP, EXPTP_LIBOTP);

class Nametag3d;

class EXPCL_LIBOTP Nametag3d : public virtual Nametag {
    
    PUBLISHED:
        Nametag3d();
        Nametag3d(const Nametag3d& tag);
        ~Nametag3d();
        Nametag3d& operator=(const Nametag3d& tag);
        
    public:
        virtual void tick();
        void set_billboard_offset(float offset);
        float get_billboard_offset();
        bool safe_to_flatten_below();
        void update_contents();
        
    protected:
        virtual ChatBalloon* get_speech_balloon();
        virtual ChatBalloon* get_thought_balloon();
        virtual void billboard();
        
        float m_bb_offset;
        
        LVecBase4f frame;
        
    TYPE_HANDLE(Nametag3d, TypedObject);
};