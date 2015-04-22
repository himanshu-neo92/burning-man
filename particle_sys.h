#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "../../octet.h"

#include <random>
#include <time.h>

#include "Particle.h"
#include "particle_emitter.h"


namespace octet {
    class particle_sys :public resource
    {
    public:
        typedef ref<Particle> smtPart;
        typedef ref<particle_emitter> smtEmitter;
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

    private:
      dynarray<Particle*> openPool_;

      dynarray<smtPart> particles_;
      dynarray<smtEmitter> emitters_;


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