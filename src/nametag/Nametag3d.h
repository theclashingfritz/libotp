#pragma once

#include "util.h"
#include "Nametag.h"

#include <math.h>
#include <algorithm>

#include <pandabase.h>
#include <boundingVolume.h>
#include <boundingBox.h>
#include <depthWriteAttrib.h>
#include <notifyCategoryProxy.h>

NotifyCategoryDecl(Nametag3d, EXPCL_LIBOTP, EXPTP_LIBOTP);

class Nametag3d;
class PopupMouseWatcherRegion;

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
        virtual void billboard();
        
        virtual void show_name();
        
        virtual ChatBalloon* get_speech_balloon();
        virtual ChatBalloon* get_thought_balloon();
        
        float m_bb_offset;
        
        LVecBase4f frame;
    
    private:
        void cull_callback(CullTraverser *traverser, CullTraverserData *traverser_data);
        void adjust_to_camera(NodePath path, int value);
        
    TYPE_HANDLE(Nametag3d, Nametag);
};