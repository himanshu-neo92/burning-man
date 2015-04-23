#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "../../octet.h"

#include <random>
#include <time.h>

#include "Particle.h"
#include "particle_emitter.h"


namespace octet {

  struct collision_sphere
  {
    float _rad;
    vec3 _center;
    
  };
    class particle_sys :public resource
    {
    public:
        typedef ref<Particle> smtPart;
        typedef ref<particle_emitter> smtEmitter;
        typedef ref<collision_sphere> smtcollider;
    public:
        Particle* GetNewParticle()
        {
            if (openPool_.size() >= 1)
            {
                Particle* p = openPool_.back();
                //flag is enabled
                p->SetEnabledFlag(true);
                openPool_.pop_back();

                return p;
            }
            return NULL;
        
        }

        void AdvancedInit()
        {
            meshy_ = new mesh();
            int size=sizeof(float)*7;//9 3+4+2
            meshy_->allocate(size * 4, 0);
            meshy_->set_params(size, 0, 4, GL_TRIANGLE_STRIP, NULL);
            meshy_->clear_attributes();

            meshy_->add_attribute(octet::attribute_pos, 3, GL_FLOAT, 0);
            meshy_->add_attribute(octet::attribute_color, 4, GL_FLOAT, 12,0,1);
            //meshy_->add_attribute(attribute_uv,2,GL_FLOAT,28,0,1);
            
            meshy_->set_num_instances(4);
            

            const GLfloat vertData[] = {
                -0.5f, -0.5f, 0.0f,
                0.0f, 0, 1.0f, 1.0f,
               // 0.0f,0.0f,

                0.5f, -0.5f, 0.0f,
                0, 1.0f, 0, 1.0f,
                //1.0f,0.0f,

                -0.5f, 0.5f, 0.0f,
                0, 0, 1.0f, 1.0f,
                //2.0f,0.0f,

                0.5f, 0.5f, 0.0f,
                1.0f, 1.0f, 0.0f, 1.0f,
                //3.0f,0.0f,
            };

            { //scope destroy the lock
                octet::gl_resource::wolock vt(meshy_->get_vertices());
                float* vtx = (float*)vt.u8();

                memcpy(vtx, &vertData[0], size * 4);
            }

            const float uvData[] = {
                0.0f, 0,
                1.0f, 0,
                2.0f, 0,
                3.0f, 0,
            };
            
            bufferA = new gl_resource();

            
            int loc = glGetAttribLocation(sh->get_program(), "uv");
            //setup the instance buffer
            
            bufferA->bind();
            
            bufferA->allocate(GL_ARRAY_BUFFER, sizeof(float)*2 * 4, GL_DYNAMIC_DRAW);
            bufferA->assign(&uvData[0], 0, sizeof(float)*2 * 4);
            
           
            //it is now bound and as such these commands work
            glVertexAttribPointer(loc, 2, GL_FLOAT, FALSE, 0, NULL);
            
            glEnableVertexAttribArray(loc);
            
            glVertexAttribDivisor(loc, 1);
            
   
        }

        void Init(int mp,ref<visual_scene> scene)
        {
            maxParticles_=mp;

            
            //meshy_ = new mesh_box(vec3(1,1,1));
            //mat=new material(vec4(1,0,0,1));
            //meshy_->add_attribute(attribute_color,4,GL_FLOAT,32,0,1);

            particles_.resize(maxParticles_);
            openPool_.reserve(maxParticles_);
            meshies_.reserve(maxParticles_);

            
        
           sh = new param_shader("src/examples/BurningMan/Particle.vs", "src/examples/BurningMan/Particle_solid.fs");
            
            mat=new material(vec4(1,0,0,1),sh);
            AdvancedInit();
            
            for (int i = 0; i < maxParticles_; ++i)
            {
                particles_[i]=new Particle();
                openPool_.push_back(particles_[i]);
                scene_node* s=new scene_node();
                scene->add_scene_node(s);
                meshies_.push_back(new mesh_instance(s,meshy_,mat));
               scene->add_mesh_instance(meshies_.back());
            }



            srand(time(NULL));
        }

        int AddEmitter(smtEmitter em)
        {
            if (em)
            {
                emitters_.push_back(em);
                return emitters_.size()-1;//index of the emitter is the new size -1
            }
        }


        smtEmitter GetEmitter(unsigned index)
        {
            if (index < emitters_.size())
            {
                return emitters_[index];
            }

            return nullptr;//no emmiter at index
        }

        int AddCollider(smtcollider cs)
        {
          if (cs)
          {
            collider_.push_back(cs);
            return collider_.size()-1;
          }
        }

        smtcollider GetCollider(unsigned index)
        {
          if (index < collider_.size())
          {
            return collider_[index];
          }
          return nullptr;
        }


        void Draw()
        {
            for (int i = 0; i < particles_.size(); ++i)
            {
                if (particles_[i]->GetEnabledFlag())
                {
                    meshies_[i]->get_node()->access_nodeToParent().w() = particles_[i]->GetPos().xyz1();
                }
            }

            //gl resource update
        }

        void Update(float dt)
        {
            
            for (int i = 0; i < emitters_.size(); ++i)
            {
                int num=emitters_[i]->Update(dt);
                if (num > 0)
                {
                    for (int j= 0; j < num; ++j)
                    {
                       emitters_[i]->Spawn_particle(GetNewParticle());
                    }
                }
            }
            

            for (int i = 0; i < particles_.size(); ++i)
            {
                if (particles_[i]->IsDead()&&particles_[i]->GetEnabledFlag())
                {
                    ReturnParticle(particles_[i]);
                }
                else if(particles_[i]->GetEnabledFlag())
                {
                    
                    particles_[i]->AddForce(gravity_+SumForces());
                    particles_[i]->Update(dt);
                    calculate_coll_test(particles_[i]);
                }
            }
            printf("%d\n",openPool_.size());
        }

        void SetGravity(const vec3& v)
        {
            gravity_=v;
        }

        vec3 GetGravity()const
        {
           return gravity_;
        }

        void AddForce(const vec3& force)
        {
            forces_.push_back(force);
        }

    private:
        void ReturnParticle(smtPart p)
        {
            p->SetEnabledFlag(false);
            openPool_.push_back(p);
        }
        vec3 SumForces()
        {
            vec3 sum;
            for (int i = 0; i < forces_.size(); ++i)
            {
                sum+=forces_[i];
            }
            return sum;
        }

        void calculate_coll_test(smtPart particle)
        {
          for (int i=0;i<collider_.size();i++)
          {
            if (squared(particle->GetPos().x() - collider_[i]->_center.x()) + squared(particle->GetPos().y() - collider_[i]->_center.y()) + squared(particle->GetPos().z() - collider_[i]->_center.z()) <= squared(squared(0.1f + collider_[i]->_rad)))
            {
              
              vec3 current_vel = particle->Get_vel();
              vec3 nortosurface = vec3(particle->GetPos().x() - collider_[i]->_center.x(), particle->GetPos().y() - collider_[i]->_center.y(), particle->GetPos().z() - collider_[i]->_center.z());
              vec3 force_nor = nortosurface * nortosurface.dot(current_vel);
              vec3 force_par = current_vel - force_nor;

              force_nor *= -1;

              force_nor *= particle->GetFriction();
              force_par *= particle->GetRestitution();


              vec3 force_to_add = force_nor+force_par;
              particle->SetPos(particle->GetPos());
              particle->AddForce(force_to_add);
            }
          }
        }
    private:
      dynarray<Particle*> openPool_;

      dynarray<smtPart> particles_;
      dynarray<smtEmitter> emitters_;
      dynarray<smtcollider> collider_;

      ref<mesh> instanceMesh_;

      ref<gl_resource> bufferA;
      ref<mesh>  meshy_;
      ref<material> mat;
      param_shader* sh;
      dynarray<ref<mesh_instance>> meshies_;

      int maxParticles_;

      vec3 gravity_;
      dynarray<vec3> forces_;
    };
}

#endif