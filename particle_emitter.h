#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H


#include "../../octet.h"
#include "particle_man.h"
#include "Particle.h"

namespace octet {
    class particle_sys;
  
    class particle_emitter :public resource
    {


        vec3 position;
        vec3 direction;
        float nu_particles_per_sec;        
        int max_particles;
        float spread;
        float particles_lifetime;
        float friction_particle;
        float particle_mass;
        float speed;

        float dt_accumalation;
        particle_sys *sys_particle;

        bool enabled;

        Particle_man* particle_manager;
    public:

      particle_emitter(vec3 _position, vec3 _direction = vec3(0, 0, 1), int _max_particles = 1, float _nu_particles_per_sec = 1, float _spread = 1, float _particles_lifetime = 0.1f, float _friction_particle = 1.0f, float _particle_mass = 1.0f, float _speed=1.0f)
        {
            position = _position;
            direction = _direction.normalize();
            nu_particles_per_sec = _nu_particles_per_sec;
            max_particles = _max_particles;
            spread = _spread;
            particles_lifetime = _particles_lifetime;
            friction_particle = _friction_particle;
            speed=_speed;

            particle_mass = _particle_mass;
            dt_accumalation = 0.0f;
            enabled = true;     
            particle_manager=nullptr;
        }
       virtual ~particle_emitter()
        {
          delete particle_manager;
        }

        vec3 Get_position() const
        {
            return position;
        }

        void Set_position(vec3 _position)
        {
            position = _position;
        }

        vec3 Get_direction() const
        {
            return direction;
        }
    
    void Set_direction(vec3 _direction)
    {
        direction = _direction;
    }

    float Get_nu_particles_per_sec() const
    {
        return  nu_particles_per_sec;
    }
    void Set_nu_particles_per_sec(float _nu_particles_per_sec)
    {
        nu_particles_per_sec = _nu_particles_per_sec;
    }
    int Get_max_particles() const
    {
        return max_particles;
    }
    void Set_max_particles(int _max_particles)
    {
        max_particles = _max_particles;
    }
    float Get_spread() const
    {
        return spread;
    }
    void Set_spread(float _spread)
    {
        spread = _spread;
    }

    float Get_particles_lifetime() const
    {
        return particles_lifetime;
    }
    void Set_particles_lifetime(float _particles_lifetime)
    {
        particles_lifetime = _particles_lifetime;
    }

    float Get_speed() const
    {
      return speed;
    }
    void Set_speed(float _speed)
    {
      speed= _speed;
    }
    float Get_friction_particle() const
    {
        return friction_particle;
    }
    void Set_friction_particle(float _friction_particle)
    {
        friction_particle = _friction_particle;
    }

    float Get_mass_particle() const 
    {
      return particle_mass;
    }
    void Set_mass_particle(float mass)
    {
      particle_mass=mass;
    }


    particle_sys* Get_sys_particle() const
    {
        return sys_particle;
    }
    void Set_sys_particle(particle_sys* _sys_particle)
    {
        sys_particle = _sys_particle;
    }
    bool isenabled() const
    {
        return enabled;
    }
    void Set_enabled(bool _enabled)
    {
        enabled = _enabled;
    }

    void Set_particle_man(Particle_man *the_man)
    {
      particle_manager=the_man;
    }
    Particle_man * Get_particle_man()
    {
      return particle_manager;
    }


    int Update(float dt)
    {
      if (particle_manager != nullptr)
      {
        particle_manager->Update_particles();
      }
        dt_accumalation += dt;
        float num_particles_to_spawn = floorf(nu_particles_per_sec*dt_accumalation);
        if (num_particles_to_spawn >= 1)
        {         
            dt_accumalation = 0;
            return num_particles_to_spawn;

        }        
    }

    virtual void create_shape(Particle *)=0;
    void Spawn_particle(Particle * spawn_particles)
    {
        if (spawn_particles != nullptr)
        {
            spawn_particles->SetMass(particle_mass);
            spawn_particles->SetFriction(friction_particle);
            spawn_particles->SetLifetime(particles_lifetime);
            spawn_particles->SetPos(position);
            spawn_particles->Set_acc(vec3(0, 0, 0));

            create_shape(spawn_particles);
            
            if (particle_manager != nullptr)
            {particle_manager->add_particle(spawn_particles);}
        }
        else
        {
          //printf("no more particles");
            return;
            }
    }
  };
  }

#endif