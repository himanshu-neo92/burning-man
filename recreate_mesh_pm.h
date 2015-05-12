#ifndef INCLUDED_RECREATE_MESH_H
#define INCLUDE_RECREATE_MESH_H

#include"particle_man.h"

namespace octet
{
  class recreate_mesh_pm : public Particle_man
  {
    vec3* mesh_vert;
    bool original_done = false;
    int count=0;
    int total_parts;
    bool all_reformed=false;
    public :
    recreate_mesh_pm(int num_particles,vec3 pos, float lifetime, float mass, float speed) : Particle_man(pos, vec3(), lifetime, mass, speed)
    {
      total_parts = num_particles;
      mesh_vert = new vec3[total_parts];
    }

    ~recreate_mesh_pm()
    {
      delete mesh_vert;
    }
    virtual void manage_particles(Particle * p1)
    {
      if (!original_done)
      {
        mesh_vert[count] = p1->GetPos();
        count++;
        if (count == total_parts)
        {
          original_done = true;
          count=0;
        }
      }

      if (p1->GetLifetime() < 1)
      {
        if (!all_reformed)
        {
          p1->SetVel(vec3(-p1->Get_vel()));
          p1->Set_acc(vec3(-p1->Get_acc()));
          p1->SetPos(vec3(mesh_vert[count].x(), mesh_vert[count].y(),p1->GetPos().z()));
          p1->SetForce(vec3(0,1,0));
          p1->SetLifetime(2.0f);
          count++;
          if (count == total_parts)
          {
            all_reformed = true;
            count = 0;
          }
        }
      }
    }  
  };
}

#endif