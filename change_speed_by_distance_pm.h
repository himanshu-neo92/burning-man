#ifndef PARTICLE_MAN_child_H
#define PARTICLE_MAN_child_H

#include"particle_man.h"

namespace octet {
  class change_speed_by_distance_pm :public Particle_man
  {
  public :
    change_speed_by_distance_pm(vec3 pos, float lifetime, float mass, float speed) : Particle_man(pos,  lifetime,  mass,  speed)
    {
      
    }
    virtual void manage_particles(Particle * p1)
    {
      if (math::length( p1->GetPos() - Get_emitter_pos()) > 0.2f )
      {
        p1->AddForce(vec3(0,10,0));
      }
    }
  };
  }

  #endif