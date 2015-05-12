#ifndef PARTICLE_EMITTER_mesh_H
#define PARTICLE_EMITTER_mesh_H

#include "particle_emitter.h"

namespace octet {


  class particle_emitter_mesh : public particle_emitter
  {
    vec3 direction;    
    vec3* mesh_vert;
    int vert_num;
    vec3* mesh_vert_temp;

    void create_heap(vec3 a[], int size)
    {
      for (int i = 0; i<size; ++i)
      {
        int parent = (i - 1);
        if (parent > 0)
        {
          parent /= 2;
        }
        int tempi = i;
        while (parent >= 0)
        {
          int toswap = -1;
          if (a[parent].y()<a[tempi].y())
            toswap = parent;
          if (toswap != -1)
          {
            vec3 temp = a[toswap];
            a[toswap] = a[tempi];
            a[tempi] = temp;
          }
          tempi = parent;
          parent = (parent - 1);
          if (parent  > 0)
          {
            parent /= 2;
          }
        }
      }

    }
    void deheap(vec3 a[], int size)
    {
      while (size > 0)
      {
        for (int i = 0; i<size; ++i)
        {
          int child1 = 2 * i + 1;
          int child2 = 2 * i + 2;
          int toswap = -1;
          if (child1 < size)
          {
            if (child2 < size)
            {
              toswap = a[child1].y()>a[child2].y() ? child1 : child2;
              if (a[toswap].y()<a[i].y())
              {
                toswap = -1;
              }
            }
            else
            {
              if (a[child1].y()>a[i].y())
              {
                toswap = child1;
              }
            }
            if (toswap != -1)
            {
              vec3 temp = a[toswap];
              a[toswap] = a[i];
              a[i] = temp;
            }
          }
        }
        vec3 temp = a[0];
        a[0] = a[size - 1];
        a[size - 1] = temp;
        --size;
      }
    }
    void vert_srt(vec3 vert[], int size)
    {
      create_heap(vert, size);
      deheap(vert, size);
    }

  public:
    particle_emitter_mesh(mesh* newmesh,vec3 _direction = vec3(0, 0, 1),
      vec3 _position = vec3(0, 0, 0), float _nu_particles_per_sec = 1, float _particles_lifetime = 0.1f,
      float _friction_particle = 1.0f, float res_ = 1.0f, float _particle_mass = 1.0f, float _speed = 1.0f) :
      particle_emitter(_position, _nu_particles_per_sec, _particles_lifetime, _friction_particle, res_, _particle_mass, _speed)
    {
      direction = _direction*_speed;
      
      vert_num = newmesh->get_num_vertices();
      mesh_vert = new vec3[vert_num];
      newmesh->dump1(mesh_vert);
      vert_srt(mesh_vert, vert_num);

      mesh_vert_temp = mesh_vert;
    }
    ~particle_emitter_mesh()
    {
      delete mesh_vert;
    }

    void create_shape(Particle *spawn_particles)
    {
      static int vert_index = 0;
      vec3 emmiter_pos = Get_position();
      spawn_particles->SetPos(vec3(  (emmiter_pos.x() + mesh_vert_temp->x()) / 20, (emmiter_pos.z() + mesh_vert_temp->z()) / 20, emmiter_pos.y()));

      
      spawn_particles->SetForce(direction);
      if (vert_index == vert_num)
      {
        vert_index = 0;
        mesh_vert_temp = mesh_vert;
      }
      vert_index++;
      mesh_vert_temp++;
    }
  };
  }
  #endif