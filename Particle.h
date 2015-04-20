#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include"../../octet.h"

class Particle
{
public:
    //construct with mass, mass will be converted to inverse mass
    Particle(float mass = 1.0f) :lifetime_(0), friction_(1)
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
        if(invMass_ == 0)
        {
            return;
        }
        acc_=force_*invMass_;
        force_=octet::vec3();

        octet::vec3 diff = (pos_-oldPos_)*friction_+acc_*dt;
        oldPos_=pos_;
        pos_+=diff;
    }
    float GetInvMass()const
    {return invMass_;}

    void AddForce(const octet::vec3& force)
    {force_ += force;}

    void SetForce(const octet::vec3& force)
    {force_=force;}

    octet::vec3 GetForce()const
    {return force_;}

    void SetPos(const octet::vec3& pos)
    {
        octet::vec3 diff = pos - pos_;
        pos_ +=diff;
        oldPos_ += diff;
    }
    octet::vec3 GetPos()const
    {return pos_;}

    float GetLifetime()const
    {return lifetime_;}

    void SetLifetime(float lifetime)
    {lifetime_=lifetime;}

    bool IsDead()const
    {return lifetime_ > 0;}

    float GetFriction()const
    {return friction_;}

    void SetFriction(float frict)
    {friction_=frict;}

private:
    octet::vec3 pos_;
    octet::vec3 oldPos_;
    octet::vec3 acc_;


    octet::vec3 force_;
    float invMass_;
    float lifetime_;
    float friction_;
};

#endif