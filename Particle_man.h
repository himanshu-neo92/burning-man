#ifndef PARTICLE_MAN_H
#define PARTICLE_MAN_H

#include "../../octet.h"
#include "Particle.h"

namespace octet {
  class Particle_man :public resource
  {
    vec3 emitter_pos;
    vec3 emitter_dir;
    float start_life_time;
    float start_mass;
    float start_speed;

    void saferemove(int index)
    {

      Particle* tempptr = particle_pointers[index];
      particle_pointers[index] = particle_pointers[particle_pointers.size() - 1];
      particle_pointers[particle_pointers.size()-1] = tempptr;
      particle_pointers.pop_back();
    }

    public :
      
      virtual ~Particle_man()
      {
        
      }
      Particle_man(vec3 pos,vec3 dir, float lifetime,float mass,float speed)
      {
        emitter_pos=pos;
        emitter_dir=dir;
        start_life_time=lifetime;
        start_mass=mass;
        start_speed=speed;
      }
      void Update_particles()
      {
        for (int i = 0;i<particle_pointers.size();i++)
        {      
          if (particle_pointers[i]->GetLifetime()>0.0f)
            manage_particles(particle_pointers[i]);
            else
              saferemove(i);
        }
      }            
      void add_particle(Particle *new_particle)
      {
        particle_pointers.push_back(new_particle);
      }
      virtual void manage_particles(Particle *) = 0;

      ///seters and getters
      vec3 Get_emitter_pos() const
      {
        return emitter_pos;
      }
      void Set_emitter_pos(vec3 pos)
      {
        emitter_pos=pos;
      }
      vec3 Get_emitter_dir() const
      {
        return emitter_dir;
      }
      void Set_emitter_dir(vec3 dir)
      {
        emitter_dir = dir;
      }
      float Get_start_life_time() const
      {
        return start_life_time;
      }
      void Set_start_life_time(float sslt)
      {
        start_life_time = sslt;
      }
      float Get_start_mass() const
      {
        return start_mass;
      }
      void Set_start_mass(float mass)
      {
        start_mass = mass;
      }
      float Get_start_speed() const
      {
        return start_speed;
      }
      void Set_start_speed(float speed)
      {
        start_speed =speed;
      }
      protected :
        dynarray<Particle*> particle_pointers;
  };
  }

#endif