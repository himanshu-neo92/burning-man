#ifndef PARTICLE_EMITTER_sphere_H
#define PARTICLE_EMITTER_sphere_H
#include "particle_emitter.h"

namespace octet {
  

  class particle_emitter_sphere : public particle_emitter
  {
  public : 
    particle_emitter_sphere(vec3 _position, vec3 _direction = vec3(0, 0, 1), int _max_particles = 1, float _nu_particles_per_sec = 1, float _spread = 1, float _particles_lifetime = 0.1f, float _friction_particle = 1.0f, float _particle_mass = 1.0f, float _speed = 1.0f) : particle_emitter(_position, _direction, _max_particles, _nu_particles_per_sec, _nu_particles_per_sec, _spread, _particle_mass, _speed)
    {
    }
    ~particle_emitter_sphere()
    {
    }
    void create_shape(Particle *spawn_particles)
    {
      mat4t temp_rotation_mat;
      int temp_ran;
      float temp_rot;

      temp_ran = rand();
      float timesNum = ((((float)temp_ran / (float)RAND_MAX) * 2) - 1);

      temp_rot = ((((float)temp_ran / (float)RAND_MAX) * 2) - 1) * (Get_spread() / 2);
      temp_rotation_mat.rotateX(temp_rot);

      temp_ran = rand();
      temp_rot = ((((float)temp_ran / (float)RAND_MAX) * 2) - 1) * (Get_spread() / 2);
      temp_rotation_mat.rotateY(temp_rot);

      temp_ran = rand();
      temp_rot = ((((float)temp_ran / (float)RAND_MAX) * 2) - 1) * (Get_spread() / 2);
      temp_rotation_mat.rotateZ(temp_rot);

      temp_rotation_mat.translate(Get_direction().x()*Get_speed(), Get_direction().y()*Get_speed(), Get_direction().z()*Get_speed());

      spawn_particles->SetForce(vec3(temp_rotation_mat.w().x(), temp_rotation_mat.w().y(), temp_rotation_mat.w().z()));
    }

    
  };
  }
#endif