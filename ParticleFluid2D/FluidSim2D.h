#ifndef FLUIDSIM2D_H
#define FLUIDSIM2D_H

#include <bits/stdc++.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

/**
 * Fluid particle for SPH system
 */
class Particle
{
public:
	//Position of particle
    dvec3 position;
    //Velocity of particle
    dvec3 velocity;
    //Half step velocity of particle
    dvec3 velocity_half_step;
    //Acceleration of particle
    dvec3 acceleration;
    //Force acting on particle
    dvec3 force;
    //Dynamic density of particle
    double density;
    //List of neighbours of particles
    vector<int> neighbours;
    
    /**
     * Class constructor
     * @param position Initial position of particle
     */
    Particle(dvec3 position);
};


/**
 * SPH-based fluid simulator
 */
class Simulation
{
public:
	//Vector of system particles
    vector<Particle> particles;
    //Rest density
    double rho0;
    //Number of frames
    int nframes;
    //Number of steps per frame
    int stepsperframe;
    //Size of particle
    double h;
    //Time step
    double dt;
    //Gas constant
    double k;
    //Viscosity coefficient
    double mu;
    //Gravity
    dvec3 g;
    //Mass of particles
    double m;
    //Radius of ball
    double cradius;
    //Mass of ball
    double cmass;
    //Centre of ball
    dvec3 ccenter;
    //Velocity of ball
    dvec3 cvelocity;
    //Simulator update timestep
    int timestep;
    //Attraction vector
    dvec3 attraction;
    //Attraction on
    bool attract;
    //Attraction force
    double att_force;
    
    /**
     *Class constructor
     */
    Simulation();
    
    /**
     * Initializes all the simulator parameters
     */
    void init();
    
    /**
     * Initializes the particle positions
     */
    void initPositions();
    
    /**
     * Finds the neighbours of each particle
     */
    void setNeighbours();
    
    /**
     * Calculates the mass of the particles
     */
    void getMass();
    
    /**
     * Calculates the densities of all the particles
     */
    void setDensities();
    
    /**
     * Calculates the forces acting on all the particles
     */
    void setForces();
    
    /**
     * Calculates the acceleration of each particle
     */
    void setAcceleration();
    
    /**
     * Leap-frog integration for velocity and position updates
     */
    void leapIntegrate();
    
    /**
     * Forward Euler method for velocity and position updates
     */
    void forwardEuler();
    
    /**
     * Bounces off the particles on collision with boundary
     */
    void bounce(int index);
    
    /**
     * Bounces off particles from a ball
     */
    void cBounce(int i);
    
    /**
     * Performs reflections from all solid surfaces
     */
    void reflectAll();
    
    /**
     * A single simulator update
     */
    void update();
    
    /**
     * Sets the attraction
     * @param x X-coordinate of mouse
     * @param y Y-coordinate of mouse
     */
    void setAttraction(double x, double y);
};



#endif /* FLUIDSIM2D_H */

