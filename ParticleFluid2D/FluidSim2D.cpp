#include <cmath>
#include "FluidSim2D.h"

#define _USE_MATH_DEFINES
#define H 0.05
#define HH (H / 1.3)
#define DAMP 0.75
#define MAX_VEL 3.0


bool square(double x, double y)
{
    return x <= 0.5 && y <= 0.5;
}

bool circle(double x, double y)
{
    return ((x - 0.5)*(x - 0.5) + (y - 0.5)*(y - 0.5)) <= 0.25 * 0.25;
}

dvec3 clamp(dvec3 vec, double len){
    dvec3 temp;
    if (length(vec)>len)
        temp = vec/length(vec)*len;
    else
        temp = vec;
    return temp;
}

bool check(double val)
{
    return (val >= 0.0 && val <= 1.0);
}

double getBarrier(double val)
{
    if(val < 0.0)
        return 0.0;
    else if(val > 1.0)
        return 1.0;
    else
        return -1.0;
}


Particle::Particle(dvec3 position)
{
    this->position = position;
    velocity = dvec3(0.0);
    velocity_half_step = dvec3(0.0);
    acceleration = dvec3(0.0);
    force = dvec3(0.0);
    density = 0.0;
    neighbours.reserve(10);
}

Simulation::Simulation()
{
    init();
}

void Simulation::init()
{
    h = H;
    rho0 = 1000;
    nframes = 400;
    stepsperframe = 100;
    dt = 1e-4;
    k = 1000;
    mu = 0.1;
    g = dvec3(0.0, -9.8, 0.0);
    initPositions();
    setNeighbours();
    getMass();
    timestep = 0;
    cradius = 0.1;
    cmass = 10;
    ccenter = dvec3(0.5, 0.75, 0.0);
    att_force = 1e4;
}

void Simulation::initPositions()
{
    for(double x = 0.0; x <= 1.0; x += HH)
    {
        for(double y = 0.0; y <= 1.0; y += HH)
        {
            if(square(x, y))
                particles.push_back(Particle(dvec3(x, y, 0.0)));
        }
    }
}

void Simulation::setNeighbours()
{
    for(int i = 0; i < particles.size(); i++)
    {
        particles[i].neighbours.clear();
    }
    for(int i = 0; i < particles.size(); i++)
    {
        for(int j = i + 1; j < particles.size(); j++)
        {
            if(length(particles[i].position - particles[j].position) <= h)
            {
                particles[i].neighbours.push_back(j);
                particles[j].neighbours.push_back(i);
            }
        }
    }
    
}

void Simulation::getMass()
{
    m = 1.0;
    double rhosqsum = 0.0;
    double rhosum = 0.0;
    setDensities();
    for(int i = 0; i < particles.size(); i++)
    {
        rhosqsum += particles[i].density * particles[i].density;
        rhosum += particles[i].density;
    }
    
    m = (rho0 * rhosum) / rhosqsum;
}

void Simulation::setDensities()
{
    for(int i = 0; i < particles.size(); i++)
    {
        particles[i].density = 0.0;
    }
    for(int i = 0; i < particles.size(); i++)
    {
        for(int j = 0; j < particles[i].neighbours.size(); j++)
        {
            double r = length(particles[i].position - particles[particles[i].neighbours[j]].position);
            particles[i].density += (h*h - r*r)*(h*h - r*r)*(h*h - r*r);
        }
        particles[i].density *= 4.0 * m / M_PI / pow(h, 8) ;
    }
}

void Simulation::setForces()
{
    for(int i = 0; i < particles.size(); i++)
    {
        particles[i].force = dvec3(0.0);
    }
    for(int i = 0; i < particles.size(); i++)
    {
        for(int j = 0; j < particles[i].neighbours.size(); j++)
        {
            int neighbour = particles[i].neighbours[j];
            double rhoi = particles[i].density;
            double rhoj = particles[neighbour].density;
            dvec3 rij = particles[i].position - particles[neighbour].position;
            dvec3 vij = particles[i].velocity - particles[neighbour].velocity;
            double qij = length(rij) / h;
            particles[i].force += ((1 - qij)/rhoj)* ((15 * k * (rhoi + rhoj - 2*rho0) *(1 - qij)/ qij) * rij - 40.0 * mu * vij);
        }
        particles[i].force *= m/(M_PI * pow(h, 4));
    }
    if(attract)
    {
        for(int i = 0; i < particles.size(); i++)
        {
            dvec3 dir = attraction - particles[i].position;
            dvec3 unit = normalize(dir);
            particles[i].force += unit * att_force / (length(dir) * length(dir));
        }
    }
}

void Simulation::setAcceleration()
{
    for(int i = 0; i < particles.size(); i++)
    {
        particles[i].acceleration = particles[i].force / (particles[i].density) + g;
    }
}

void Simulation::bounce(int index)
{
    dvec3 pos = particles[index].position;
    int which = -1;
    double barrier = -1;
    if(!check(pos[0]))
    {
        which = 0;
        barrier = getBarrier(pos[0]);
    }
    else if(!check(pos[1]))
    {
        which = 1;
        barrier = getBarrier(pos[1]);
    }
    else
        return;
    if(particles[index].velocity[which] == 0)
        return;
    double tbounce = (pos[which]-barrier)/particles[index].velocity[which];
    particles[index].position[which] = barrier - particles[index].velocity[which] * tbounce * DAMP;
    //particles[index].position[0] -= particles[index].velocity[0]*(1-DAMP)*tbounce;
    //particles[index].position[1] -= particles[index].velocity[1]*(1-DAMP)*tbounce;
    //particles[index].position[which]  = 2*barrier-pos[which];
    particles[index].velocity[which]  = -particles[index].velocity[which];
    particles[index].velocity_half_step[which] = -particles[index].velocity_half_step[which];
    particles[index].velocity[which] *= DAMP;
    particles[index].velocity_half_step[which] *= DAMP;
}

void Simulation::cBounce(int i)
{
    dvec3 pos = particles[i].position;
    if(length(pos - ccenter) > cradius)
        return;
    dvec3 unit = normalize(ccenter - pos);
    dvec3 vpar = unit * dot(particles[i].velocity, unit);
    dvec3 vper = particles[i].velocity - vpar;
    double tbounce = (cradius - length(ccenter - pos) )/ length(vpar);
    //cout << ccenter[0] << " " << unit[0] << length << " " << pos[0]<< endl;
    particles[i].position -= tbounce * vpar + tbounce * vpar * DAMP;
    vpar = -1.0 * vpar * DAMP;
    cvelocity += m * (particles[i].velocity - (vper + vpar)) / cmass;
    particles[i].velocity = vper + vpar;
}

void Simulation::reflectAll()
{
    for(int i = 0; i < particles.size(); i++)
        bounce(i);
    //for(int i = 0; i < particles.size(); i++)
    //    cBounce(i);
    //cvelocity += g * dt;
    //cout << cvelocity[0] << endl;
    //ccenter += cvelocity * dt;
}

void Simulation::leapIntegrate()
{
    if(timestep == 0)
    {
        for(int i = 0; i < particles.size(); i++)
        {
            particles[i].velocity_half_step = clamp(particles[i].velocity + particles[i].acceleration * dt / 2.0,MAX_VEL);            
            particles[i].velocity += particles[i].acceleration * dt;
            particles[i].velocity = clamp(particles[i].velocity,MAX_VEL);
            particles[i].position += particles[i].velocity_half_step * dt;
        }
    }
    else
    {
        for(int i = 0; i < particles.size();  i++)
        {
            particles[i].velocity_half_step += particles[i].acceleration * dt;
            particles[i].velocity_half_step = clamp(particles[i].velocity_half_step,MAX_VEL);
            particles[i].velocity = clamp(particles[i].velocity_half_step + particles[i].acceleration * dt / 2.0,MAX_VEL);
            particles[i].position += particles[i].velocity_half_step * dt;
        }
    }
    reflectAll();
}

void Simulation::forwardEuler(){
    for(int i=0; i<particles.size(); i++){
        particles[i].velocity += particles[i].acceleration * dt;
        particles[i].velocity = clamp(particles[i].velocity, MAX_VEL);
        particles[i].position += particles[i].velocity * dt;
    }
    reflectAll();
}

void Simulation::update()
{
    setNeighbours();
    setDensities();
    setForces();
    setAcceleration();
    leapIntegrate();
    timestep++;
}

void Simulation::setAttraction(double x, double y)
{
    attraction = dvec3(x, y, 0.0);
}









