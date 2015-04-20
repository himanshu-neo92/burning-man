#include "particle_sys.h"

#include "particle_emitter.h"

void particle_emitter::Spawn_particle(Particle * new_particle)
{
  //new_particle->
  //call the init of the particle we have recived
  //in the particle 0 the acc and the force and then make the vel as force
}

particle_emitter::particle_emitter(vec3 _position, vec3 _direction = vec3(0, 0, 1), int _max_particles = 1, float _nu_particles_per_sec = 1, float _spread = 1, float _particles_lifetime = 0.1f, float _friction_particle = 1.0f)
{
  position = _position;
  direction = _direction;
  nu_particles_per_sec = _nu_particles_per_sec;
  max_particles = _max_particles;
  spread = _spread;
  particles_lifetime = _particles_lifetime;
  friction_particle = _friction_particle;

  dt_accumalation = 0.0f;

  enabled = true;
}
particle_emitter::~particle_emitter()
{

}

vec3 particle_emitter::Get_position() const
{
  return position;
}
void particle_emitter::Set_position(vec3 _position)
{
  position = _position;
}

vec3 particle_emitter::Get_direction() const
{
  return direction;
}
void particle_emitter::Set_direction(vec3 _direction)
{
  direction = _direction;
}

float particle_emitter::Get_nu_particles_per_sec() const
{
  return  nu_particles_per_sec;
}
void particle_emitter::Set__nu_particles_per_sec(float _nu_particles_per_sec)
{
  nu_particles_per_sec = _nu_particles_per_sec;
}

int particle_emitter::Get_max_particles() const
{
  return max_particles;
}
void particle_emitter::Set_max_particles(int _max_particles)
{
  max_particles = _max_particles;
}

float particle_emitter::Get_spread() const
{
  return spread;
}
void particle_emitter::Set_spread(float _spread)
{
  spread = _spread;
}

float particle_emitter::Get_particles_lifetime() const
{
  return particles_lifetime;
}
void particle_emitter::Set_particles_lifetime(float _particles_lifetime)
{
  particles_lifetime = _particles_lifetime;
}

float particle_emitter::Get_friction_particle() const
{
  return friction_particle;
}
void particle_emitter::Set_friction_particle(float _friction_particle)
{
  friction_particle = _friction_particle;
}

particle_sys* particle_emitter::Get_sys_particle() const
{
  return sys_particle;
}
void particle_emitter::Set_sys_particle(particle_sys* _sys_particle)
{
  sys_particle = _sys_particle;
}

bool particle_emitter::isenabled() const
{
  return enabled;
}
void particle_emitter::Set_enabled(bool _enabled)
{
  enabled = _enabled;
}


void particle_emitter::Update(float dt)
{
  dt_accumalation += dt;
  float num_particles_to_spawn = floorf(nu_particles_per_sec*dt_accumalation);
  if (num_particles_to_spawn >= 1)
  {
    for (int i = 0; i<num_particles_to_spawn; i++)
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