#pragma once

#include "util.h"
#include <typedObject.h>
#include <nodePath.h>
#include <pandabase.h>

NotifyCategoryDecl(CImpulse, EXPCL_LIBOTP, EXPTP_LIBOTP);

class CMover;

class EXPCL_LIBOTP CImpulse : public virtual TypedObject {
    
    PUBLISHED:
        CImpulse();
        ~CImpulse();
        virtual void process(float dt);
        void set_mover(CMover* mover);
        void set_node_path(NodePath path);
        void clear_mover();
        void clear_mover(CMover* mover);
        NodePath get_node_path();
        CMover* get_mover();
        bool is_cpp();
        
    public:
        Vec3 m_vec_type;
        
    protected:
        NodePath m_nodepath;
        CMover* m_mover;
        
    // Type handle
    public:
        static TypeHandle get_class_type()
        {
            return _type_handle;
        }
        
        static void init_type()
        {
            TypedObject::init_type();
            register_type(_type_handle, "CImpulse", TypedObject::get_class_type());
        }
        
        virtual TypeHandle get_type() const
        {
            return get_class_type();
        }
        
        virtual TypeHandle force_init_type()
        {
            init_type();
            return get_class_type();
        }

    private:
        static TypeHandle _type_handle;
};