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

        void Init(int mp,ref<visual_scene> scene)
        {
            maxParticles_=mp;

            
            meshy_ = new mesh_box(vec3(1,1,1));
            material* mat=new material(vec4(1,0,0,1));

            particles_.resize(maxParticles_);
            openPool_.reserve(maxParticles_);
            meshies_.reserve(maxParticles_);
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
        }

        void Update(float dt)
        {
            
            for (int i = 0; i < emitters_.size(); ++i)
            {
                int num=emitters_[i]->Update(dt);
                if (num > 0)
                {
                    for (int i = 0; i < num; ++i)
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
            printf("%d",openPool_.size());
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

      ref<mesh>  meshy_;

      dynarray<ref<mesh_instance>> meshies_;

      int maxParticles_;

      vec3 gravity_;
      dynarray<vec3> forces_;
    };
}

#endif