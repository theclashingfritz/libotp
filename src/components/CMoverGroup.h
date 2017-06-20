#pragma once

#include "util.h"
#include <string>
#include <fstream>
#include <istream>
#include <iostream>
#include <notifyCategoryProxy.h>
#include <typedReferenceCount.h>
#include <pandabase.h>
#include <nodePath.h>
#include <clockObject.h>
#include "CMover.h"

NotifyCategoryDecl(CMoverGroup, EXPCL_LIBOTP, EXPTP_LIBOTP);

class CMoverGroup;

class EXPCL_LIBOTP CMoverGroup : public TypedReferenceCount {
    
    PUBLISHED:
        CMoverGroup();
        ~CMoverGroup();
        void add_c_mover(string name, CMover* mover);
        void remove_c_mover(string name);
        void process_c_impulses();
        void process_c_impulses_and_integrate();
        void set_dt(double dt);
        
    private:
        pmap<std::string, CMover*> m_c_movers;
        
        float _dt;
        float _dt_clock;
    
    TYPE_HANDLE(CMoverGroup, TypedObject);
};