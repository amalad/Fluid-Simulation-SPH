/* 
 * File:   main.cpp
 * Author: amalad
 *
 * Created on November 26, 2017, 4:33 PM
 */

#include <cstdlib>
#include <GL/glut.h>
#include <GL/gl.h>
#include "FluidSim2D.h"

using namespace std;
using namespace glm;

Simulation *s;

/**
 * Draws a circle
 * @param centre Centre of circle
 * @param radius Radius of circle
 */
void drawCircle(dvec3 centre, double radius){
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(centre[0], centre[1], centre[2]);
        for (int i = 0; i <=360; i++)
	{
            glVertex3d(centre[0]+radius*cos(i*M_PI/180), centre[1]+radius*sin(i*M_PI/180), centre[2]);
	}
    glEnd();
	
}

/**
 * Draws the particles in display window
 */
void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    for(int i=0; i<s->particles.size(); i++){
    	glColor3f(1.0, s->particles[i].position[0], s->particles[i].position[1]);
        drawCircle(s->particles[i].position, 0.015);
    }
    
    glFlush();
}

/**
 * Updates the simulator. Input to glutTimerFunc
 */
void timer(int state)
{
    for(int i = 0; i < 15; i++)
    s->update();
    glutPostRedisplay();
    glutTimerFunc(10, timer, 1);
}

/**
 * Callback for mouse movement
 */ 
void mouseMove(int x, int y)
{
    y = 500 - y;
    double xcoor = x / 500.0;
    double ycoor = y / 500.0;
    s->setAttraction(xcoor, ycoor);
}

/**
 * Callback for mouse click
 */
void mouseClick(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
            s->attract = true;
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
            s->attract = false;
}

/**
 * Main function
 */
int main(int argc, char** argv) {
    
    s = new Simulation();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(500, 500);
    glutCreateWindow("FluidSim");
    glClearColor(0,0,0,0);
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(10.0);
    gluOrtho2D(0, 1, 0, 1);    
    glutDisplayFunc(draw);
    glutTimerFunc(10,timer,1);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMove);
    glutMainLoop();
}

