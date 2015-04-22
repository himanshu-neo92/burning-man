#ifndef PARTICLE_EMITTER_cone_H
#define PARTICLE_EMITTER_cone_H

#include "particle_emitter.h"

namespace octet {


  class particle_emitter_cone : public particle_emitter
  {
    vec3 direction;
    float spread;
  public :

    particle_emitter_cone(float _spread = 1.0f, vec3 _direction = vec3(0, 0, 1),
    vec3 _position = vec3(0,0,0),int _max_particles = 1, float _nu_particles_per_sec = 1, float _particles_lifetime = 0.1f, 
    float _friction_particle = 1.0f, float _particle_mass = 1.0f,float _speed = 1.0f) : 
    particle_emitter(_position, _max_particles, _nu_particles_per_sec, _particles_lifetime, _friction_particle, _particle_mass, _speed)
    {
      direction = _direction;
      spread = _spread;
    }
    ~particle_emitter_cone()
    {
    }
    void create_shape(Particle *spawn_particles)
    {
      mat4t temp_rotation_mat;
      int temp_ran;
      float temp_rot;

      temp_ran = rand();
      float timesNum = ((((float)temp_ran / (float)RAND_MAX) * 2) - 1);

      temp_rot = ((((float)temp_ran / (float)RAND_MAX) * 2) - 1) * (spread / 2);
      temp_rotation_mat.rotateX(temp_rot);

      temp_ran = rand();
      temp_rot = ((((float)temp_ran / (float)RAND_MAX) * 2) - 1) * (spread / 2);
      temp_rotation_mat.rotateY(temp_rot);

      temp_ran = rand();
      temp_rot = ((((float)temp_ran / (float)RAND_MAX) * 2) - 1) * (spread / 2);
      temp_rotation_mat.rotateZ(temp_rot);

      temp_rotation_mat.translate(direction.x()*Get_speed(), direction.y()*Get_speed(), direction.z()*Get_speed());

      spawn_particles->SetForce(vec3(temp_rotation_mat.w().x(), temp_rotation_mat.w().y(), temp_rotation_mat.w().z()));
    }

  };
  }
#endif