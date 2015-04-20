#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include "../../octet.h"

namespace octet {
    class Particle:public resource
    {
    public:
        //construct with mass, mass will be converted to inverse mass
        Particle(float mass = 1.0f) :lifetime_(0), friction_(1), enabledFlag_(false)
        {
            if (mass != 0)
            {
                invMass_ = 1.0f / mass;
            }
            else
            {
                invMass_ = 0;
            }
        }
        void SetMass(float mass)
        {
            if (mass == 0)
            {
                invMass_ = 0;
                return;
            }
            invMass_ = 1 / mass;
        }
        void Update(float dt)
        {
            //check for infinite mass
            if (invMass_ == 0)
            {
                return;
            }
            //add force to acceleration depending on mass
            acc_ = force_*invMass_;
            force_ = vec3(); //null the force

            //calculate the difference between positions using veloicty + new acceleration
            vec3 diff = (pos_ - oldPos_)*friction_ + acc_*dt;
            oldPos_ = pos_;
            pos_ += diff;
            //decrease lifetime
            lifetime_-=dt;
        }

        vec3 Get_acc() const 
        {
          return acc_;
        }
        void Set_acc(vec3 _acc)
        {
          acc_ = _acc;
        }
        float GetInvMass()const
        {
            return invMass_;
        }

        void AddForce(const vec3& force)
        {
            force_ += force;
        }

        void SetForce(const vec3& force)
        {
            force_ = force;
        }

        vec3 GetForce()const
        {
            return force_;
        }

        void SetPos(const vec3& pos)
        {
            vec3 diff = pos - pos_;
            pos_ += diff;
            oldPos_ += diff;
        }
        vec3 GetPos()const
        {
            return pos_;
        }

        float GetLifetime()const
        {
            return lifetime_;
        }

        void SetLifetime(float lifetime)
        {
            lifetime_ = lifetime;
        }

        bool IsDead()const
        {
            return lifetime_ < 0;
        }

        float GetFriction()const
        {
            return friction_;
        }

        void SetFriction(float frict)
        {
            friction_ = frict;
        }

        bool GetEnabledFlag()const
        {return enabledFlag_;}

        void SetEnabledFlag(bool flag)
        {enabledFlag_=flag;}

    private:
        vec3 pos_;
        vec3 oldPos_;
        vec3 acc_;

        


        vec3 force_;
        float invMass_;
        float lifetime_;
        float friction_;
        bool enabledFlag_;
    };
}
#endif