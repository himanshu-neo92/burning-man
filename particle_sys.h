#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Particle.h"
#include "particle_emitter.h"
namespace octet {
    class particle_sys : public resource 
    {
    public:
        typedef ref<Particle> smtPart;
        typedef ref<particle_emitter> smtEmitter;
    public:
        Particle* GetNewParticle()
        {
            if (openPool_.size() > 1)
            {
                Particle* p = openPool_.back();
                //flag is enabled
                p->SetEnabledFlag(true);
                openPool_.pop_back();

                return p;
            }
            return NULL;
        
        }

        void Init(int mp)
        {
            maxParticles_=mp;

            
            particles_.resize(maxParticles_);
            openPool_.reserve(maxParticles_);
            for (int i = 0; i < maxParticles_; ++i)
            {
                particles_[i]=new Particle();
                openPool_.push_back(particles_[i]);
            }
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
        {}

        void Update(float dt)
        {
            for (int i = 0; i < emitters_.size(); ++i)
            {
                emitters_[i]->Update(dt);
            }
            

            for (int i = 0; i < particles_.size(); ++i)
            {
                if (particles_[i]->IsDead()&&particles_[i]->GetEnabledFlag())
                {
                    ReturnParticle(particles_[i]);
                }
                else
                {
                    particles_[i]->Update(dt);
                }
            }

        }
    private:
        void ReturnParticle(smtPart p)
        {
            p->SetEnabledFlag(false);
            openPool_.push_back(p);
        }

    private:
      dynarray<Particle*> openPool_;

      dynarray<smtPart> particles_;
      dynarray<smtEmitter> emitters_;

      int maxParticles_;
    };
}

#endif