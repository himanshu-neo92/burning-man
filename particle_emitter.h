namespace octet {

  class particle_emitter : public resource {
  
  float nu_particles_per_sec;
  int max_particles;
  float spread;
  vec3 direction;
  vec3 position;
  float particles_lifetime;
  float dt_accumalation;
  //particle_sys *sys_particle;
  public :

    particle_emitter()
    {
    
    }
    ~particle_emitter()
    {
    
    }

    void update(float dt)
    {
      dt_accumalation+=dt;
      float num_particles_to_spawn = floorf(nu_particles_per_sec*dt_accumalation);
      if (num_particles_to_spawn>=1)
      {
        for (int i = 0; i<num_particles_to_spawn;i++)
        { 
          //get a new particle form the particle system
          spawn_particle();
        }
        dt_accumalation = 0;
      }
    }

    void spawn_particle()
    {
      //call the init of the particle we have recived
    }
  };
  }