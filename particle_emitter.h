#ifndef PARTICLE_EMMITER_H
#define PARTICLE_EMMITER_H


#include "Particle.h"

namespace octet {

  class particle_sys;
  class particle_emitter : public resource {


  vec3 position;
  vec3 direction;
  float nu_particles_per_sec;
  int max_particles;
  float spread;  
  float particles_lifetime;
  float friction_particle;

  float dt_accumalation;
  particle_sys *sys_particle;
  
  bool enabled;

  void Spawn_particle(Particle * new_particle);
  
  public :

    particle_emitter(vec3 _position, vec3 _direction , int _max_particles, float _nu_particles_per_sec , float _spread , float _particles_lifetime , float _friction_particle );
    
    ~particle_emitter();
    
    vec3 Get_position() const;
    
    void Set_position(vec3 _position);
  

    vec3 Get_direction() const;
    
    void Set_direction(vec3 _direction);

    float Get_nu_particles_per_sec() const;
    void Set__nu_particles_per_sec(float _nu_particles_per_sec);

    int Get_max_particles() const;
    void Set_max_particles(int _max_particles);
    float Get_spread() const;
    void Set_spread(float _spread);

    float Get_particles_lifetime() const;
    void Set_particles_lifetime(float _particles_lifetime);

    float Get_friction_particle() const;
    void Set_friction_particle(float _friction_particle);

    particle_sys* Get_sys_particle() const;
    void Set_sys_particle(particle_sys* _sys_particle);

    bool isenabled() const;
    void Set_enabled(bool _enabled);
   

    void Update(float dt);

    
  };
  }

#endif