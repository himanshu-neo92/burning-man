#ifndef INCLUDED_FIREWORK_H
#define INCLUDE_FIREWORK_H

#include"particle_man.h"
#include "particle_sys.h"
#include "particle_emitter_cone.h"
namespace octet
{
  class firework_pm : public Particle_man
  {

    particle_sys *the_system;  
    int the_emitter;
    float kill_time;
    float temp_time=0;
    particle_emitter *emitter_ptr;
    int num_times = 0;
    public :  
    firework_pm(particle_sys *_system, int _emitter, float time_to_kill,particle_emitter* _emitter_ptr,int times_num,vec3 pos, float lifetime, float mass, float speed) : Particle_man(pos, vec3(), lifetime, mass, speed)
    {
      the_system = _system;
      the_emitter = _emitter;
      kill_time = time_to_kill;
      emitter_ptr = _emitter_ptr;
      num_times = times_num;
    }
    virtual void manage_particles(Particle * p1)
    {
      if (p1->GetLifetime() < 0.1f)
      {
        if (num_times<1)
        {
        particle_emitter *em = new particle_emitter_cone(90, p1->Get_vel(),
          p1->GetPos(),//position
          20000, //particles per sec
         ((float) rand()/RAND_MAX)*0.7f, //lifetime
          1.0f,//friction
          0.5f,//restitution
          1.0f,//mass
          10.0f);//speed
        int emitter_num = the_system->AddEmitter(em);
        Particle_man *temppart;
        temppart = new firework_pm(the_system, emitter_num, 1.2f, em, num_times+1, em->Get_position(), em->Get_particles_lifetime(), em->Get_mass_particle(), em->Get_speed());
        em->Set_particle_man(temppart);
        }
        if (num_times!=0)
        {
          if (the_system->GetEmitter(the_emitter)!=nullptr)
          {
            the_system->GetEmitter(the_emitter)->Set_nu_particles_per_sec(0);
//              the_system->RemoveEmitter(the_emitter);
          }
        }
      }
    }

  };
}
#endif