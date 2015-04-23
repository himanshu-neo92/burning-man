#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H


#include "../../octet.h"
#include "particle_man.h"
#include "Particle.h"

namespace octet {
    class particle_emitter :public resource
    {
        vec3 position;
        float nu_particles_per_sec;
        float particles_lifetime;
        float friction_particle;
        float particle_mass;
        float speed;
        vec4 color;

        float dt_accumalation;

        bool enabled;

        Particle_man* particle_manager;
    public:

        particle_emitter(vec3 _position,
            float _nu_particles_per_sec = 20, float _particles_lifetime = 0.8f,
            float _friction_particle = 1.0f, float _particle_mass = 1.0f, float _speed = 1.0f) :
            color(0.4, 0.4, 0.4, 1)
        {
            position = _position;
            nu_particles_per_sec = _nu_particles_per_sec;
            particles_lifetime = _particles_lifetime;
            friction_particle = _friction_particle;
            speed = _speed;

            particle_mass = _particle_mass;
            dt_accumalation = 0.0f;
            enabled = true;
            particle_manager = nullptr;
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


        float Get_nu_particles_per_sec() const
        {
            return  nu_particles_per_sec;
        }
        void Set_nu_particles_per_sec(float _nu_particles_per_sec)
        {
            nu_particles_per_sec = _nu_particles_per_sec;
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
            speed = _speed;
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
            particle_mass = mass;
        }
        void Set_particle_color(const vec4& col)
        {
            color = col;
        }
        vec4 Get_particle_color()const
        {
            return color;
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
            particle_manager = the_man;
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
            if (enabled)
            {
                dt_accumalation += dt;
                float num_particles_to_spawn = floorf(nu_particles_per_sec*dt_accumalation);
                if (num_particles_to_spawn >= 1)
                {
                    dt_accumalation = 0;
                    return num_particles_to_spawn;

                }
            }
            return 0;
        }


        virtual void create_shape(Particle *) = 0;
        void Spawn_particle(Particle * spawn_particles)
        {
            if (spawn_particles != nullptr)
            {
                spawn_particles->SetMass(particle_mass);
                spawn_particles->SetFriction(friction_particle);
                spawn_particles->SetLifetime(particles_lifetime);
                spawn_particles->SetPos(position);
                spawn_particles->Set_acc(vec3(0, 0, 0));
                spawn_particles->SetColor(color);

                create_shape(spawn_particles);

                if (particle_manager != nullptr)
                {
                    particle_manager->add_particle(spawn_particles);
                }
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