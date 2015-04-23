#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "../../octet.h"

#include <random>
#include <time.h>

#include "Particle.h"
#include "particle_emitter.h"


namespace octet {
    class particle_sys :public resource
    {
    public:
        typedef ref<Particle> smtPart;
        typedef ref<particle_emitter> smtEmitter;
    public:
        Particle* GetNewParticle()
        {
            if (openPool_.size() >= 1)
            {
                Particle* p = openPool_.back();
                //flag is enabled
                p->SetEnabledFlag(true);
                openPool_.pop_back();

                return p;
            }
            return NULL;
        
        }



        void Init(int mp, ref<visual_scene> scene, string texture = "")
        {
            maxParticles_ = mp;

            particlePosBuffer_ = new gl_resource();
            particleColorBuffer_ = new gl_resource();

            particlePosBuffer_->bind();
            //VEC3 for position
            particlePosBuffer_->allocate(GL_ARRAY_BUFFER, maxParticles_*sizeof(float) * 3, GL_DYNAMIC_DRAW);
            particleColorBuffer_->bind();
            //VEC4 for color
            particleColorBuffer_->allocate(GL_ARRAY_BUFFER, maxParticles_*sizeof(float) * 4, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            particles_.resize(maxParticles_);
            openPool_.reserve(maxParticles_);


            //init mesh
            meshy_ = new mesh();
            int size = sizeof(float) * 5;
            meshy_->allocate(size * 4, 0);
            meshy_->set_params(size, 0, 4, GL_TRIANGLE_STRIP, NULL);
            meshy_->clear_attributes();
            meshy_->add_attribute(octet::attribute_pos, 3, GL_FLOAT, 0);
            meshy_->add_attribute(octet::attribute_uv, 2, GL_FLOAT, 12);
            const GLfloat vertData[] = {
                -0.5f, -0.5f, 0.0f,
                0.0f, 0.0f,
                0.5f, -0.5f, 0.0f,
                1.0f, 0.0f,
                -0.5f, 0.5f, 0.0f,
                0.0f, 1.0f,
                0.5f, 0.5f, 0.0f,
                1.0f, 1.0f
            };

            { //scope destroy the lock
                octet::gl_resource::wolock vt(meshy_->get_vertices());
                float* vtx = (float*)vt.u8();

                memcpy(vtx, &vertData[0], size * 4);
            }
            //end mesh


            if (texture.size() > 0)
            {
                //create shader
                sh = new param_shader("src/examples/BurningMan/Particle.vs", "src/examples/BurningMan/Particle_textured.fs");
                mat = new material(new image(texture), NULL, sh);
            }
            else
            {
                sh = new param_shader("src/examples/BurningMan/Particle.vs", "src/examples/BurningMan/Particle_solid.fs");
                mat = new material(vec4(1,1,1,1), sh);
            }
            
            mat->add_uniform(NULL, atom_cameraToProjection, GL_FLOAT_MAT4, 1, param::stage_vertex);
        
            
            //end shader

            scene_node* sc = new scene_node();
            scene->add_scene_node(sc);

            meshInst_ = new mesh_instance(sc,meshy_,mat);
            scene->add_mesh_instance(meshInst_);
            
            for (int i = 0; i < maxParticles_; ++i)
            {
                particles_[i]=new Particle();
                if (texture.size())
                {
                    particles_[i]->SetColor(vec4(0,0,0,0));
                }
                openPool_.push_back(particles_[i]);
            }
            srand(time(NULL));
        }

        int AddEmitter(smtEmitter em)
        {
            if (em)
            {
                emitters_.push_back(em);
                return emitters_.size()-1;//index of the emitter is the new size -1
            }
            return 0;
        }


        smtEmitter GetEmitter(unsigned index)
        {
            if (index < emitters_.size())
            {
                return emitters_[index];
            }

            return nullptr;//no emmiter at index
        }


        void Draw(camera_instance* cam)
        {

            int posLoc=glGetAttribLocation(sh->get_program(),"particlePos");
            assert(posLoc!=-1);

            int colLoc=glGetAttribLocation(sh->get_program(),"particleColor");
            assert(colLoc != -1);

            mat4t cToP =cam->get_cameraToProjection();
            mat->set_uniform(mat->get_param_uniform(atom_cameraToProjection),&cToP,sizeof(mat4t));

            int numAlive = 0;
            //scope to break locks
            {//the first scope is the pos lock
             // the locks need to be broken in reverse order to their setup
                particlePosBuffer_->bind();
                gl_resource::wolock pt(particlePosBuffer_);
                vec3p* pos = (vec3p*)pt.u8();

                {//color lock scope
                    particleColorBuffer_->bind();
                    gl_resource::wolock ct(particleColorBuffer_);
                    vec4* col = (vec4*)ct.u8();

                    for (int i = 0; i < particles_.size(); ++i)
                    {
                        if (!particles_[i]->IsDead())
                        {
                            *pos++ = particles_[i]->GetPos();
                            *col++ = particles_[i]->GetColor();
                            ++numAlive;
                        }
                    }
                }//end of color loc scope
                particlePosBuffer_->bind();//bind particle to GL_ARRAY_BUFFER for
                //correct unlocking
            }
            particlePosBuffer_->bind();
            glVertexAttribPointer(posLoc,3,GL_FLOAT,FALSE,0,NULL);
            glEnableVertexAttribArray(posLoc);
            glVertexAttribDivisor(posLoc,1);

            particleColorBuffer_->bind();
            glVertexAttribPointer(colLoc, 4, GL_FLOAT, FALSE, 0, NULL);
            glEnableVertexAttribArray(colLoc);
            glVertexAttribDivisor(colLoc, 1);
            glBindBuffer(GL_ARRAY_BUFFER,NULL);

            meshy_->set_num_instances(numAlive);
            //gl resource update
        }

        void Update(float dt)
        {
            
            for (int i = 0; i < emitters_.size(); ++i)
            {
                int num=emitters_[i]->Update(dt);
                if (num > 0)
                {
                    for (int j= 0; j < num; ++j)
                    {
                       emitters_[i]->Spawn_particle(GetNewParticle());
                    }
                }
            }
            

            for (int i = 0; i < particles_.size(); ++i)
            {
                if (particles_[i]->IsDead()&&particles_[i]->GetEnabledFlag())
                {
                    ReturnParticle(particles_[i]);
                }
                else if(particles_[i]->GetEnabledFlag())
                {
                    particles_[i]->AddForce(gravity_+SumForces());
                    particles_[i]->Update(dt);
                }
            }
            printf("%d\n",openPool_.size());
        }

        void SetGravity(const vec3& v)
        {
            gravity_=v;
        }

        vec3 GetGravity()const
        {
           return gravity_;
        }

        void AddForce(const vec3& force)
        {
            forces_.push_back(force);
        }

    private:
        void ReturnParticle(smtPart p)
        {
            p->SetEnabledFlag(false);
            openPool_.push_back(p);
        }
        vec3 SumForces()
        {
            vec3 sum;
            for (int i = 0; i < forces_.size(); ++i)
            {
                sum+=forces_[i];
            }
            return sum;
        }

    private:
      dynarray<Particle*> openPool_;

      dynarray<smtPart> particles_;
      dynarray<smtEmitter> emitters_;

      ref<gl_resource> particlePosBuffer_;
      ref<gl_resource> particleColorBuffer_;
      ref<mesh>  meshy_;
      ref<mesh_instance> meshInst_;

      ref<material> mat;
      param_shader* sh;

      int maxParticles_;

      vec3 gravity_;
      dynarray<vec3> forces_;
    };
}

#endif