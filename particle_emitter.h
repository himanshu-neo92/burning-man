#ifndef PARTICLE_EMMITER_H
#define PARTICLE_EMMITER_H


#include "Particle.h"

namespace octet {

  class particle_emitter : public resource {
  
  float nu_particles_per_sec;
  int max_particles;
  float spread;
  vec3 direction;
  vec3 position;
  float particles_lifetime;
  float dt_accumalation;
  particle_sys *sys_particle;
  float friction_particle;
  bool isenabled;
  public :

    particle_emitter(vec3 _position, int max_particles=1, float _nu_particles_per_sec = 1, float _spread = 1, float _particles_lifetime = 0.1f, float _friction_particle = 1.0f)
    {
      position = _position;
      nu_particles_per_sec = _nu_particles_per_sec;
      max_particles = max_particles;
      spread = _spread;
      particles_lifetime = _particles_lifetime;
      friction_particle = _friction_particle;
      isenabled=true;
    }
    ~particle_emitter()
    {
    
    }

    //setter and gtetter for the vars
    void Update(float dt)
    {
      dt_accumalation+=dt;
      float num_particles_to_spawn = floorf(nu_particles_per_sec*dt_accumalation);
      if (num_particles_to_spawn>=1)
      {
        for (int i = 0; i<num_particles_to_spawn;i++)
        { 
          Particle * new_particle; //= sys_particle.get_new_particle();
          if (new_particle)
          {
            Spawn_particle(new_particle);
          }
          else
          break;
          
        }
        dt_accumalation = 0;
      }
    }

    void Spawn_particle(Particle * new_particle)
    {
       //new_particle->
      //call the init of the particle we have recived
      //in the particle 0 the acc and the force and then make the vel as force
    }
  };
  }

#endif