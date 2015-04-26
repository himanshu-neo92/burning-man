#ifndef PARTICLE_EMITTER_sphere_H
#define PARTICLE_EMITTER_sphere_H
#include "particle_emitter.h"

namespace octet {
  

  class particle_emitter_sphere : public particle_emitter
  {

  
  public : 
    particle_emitter_sphere(vec3 _position,
     float _nu_particles_per_sec = 1,
    float _particles_lifetime = 0.1f, float _friction_particle = 1.0f,float res_=1.0f,
     float _particle_mass = 1.0f, float _speed = 1.0f) : 
     particle_emitter(_position, _nu_particles_per_sec, _particles_lifetime, _friction_particle,res_, _particle_mass, _speed)
    {
      
    }
    ~particle_emitter_sphere()
    {
    }
    void create_shape(Particle *spawn_particles)
    {
     
      int temp_ran;
     

      temp_ran = rand();
      float x_dir = (((((float)temp_ran / (float)RAND_MAX) * 2) - 1)*180);
      temp_ran = rand();
      float y_dir = (((((float)temp_ran / (float)RAND_MAX) * 2) - 1) * 180);
      temp_ran = rand();
      float z_dir = (((((float)temp_ran / (float)RAND_MAX) * 2) - 1) * 180);
      
      vec3 dir = vec3(x_dir*Get_speed(), y_dir*Get_speed(), z_dir*Get_speed());
      dir.normalize();
      dir = vec3(dir.x()*Get_speed(), dir.y()*Get_speed(), dir.z()*Get_speed());
      spawn_particles->SetForce(dir);
    }

    
  };
  }
#endif