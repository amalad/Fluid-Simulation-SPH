/* 
 * File:   main.cpp
 * Author: amalad
 *
 * Created on November 26, 2017, 6:47 PM
 */

#include <cstdlib>
#include <GL/glut.h>
#include <GL/gl.h>
#include "FluidSim3D.h"
#include "Camera.h"

using namespace std;
using namespace glm;

Simulation *s;
Camera* cam;
int width=500;
int height=500;

void drawSphere(double radius, int slices, int stacks) {
	GLUquadricObj *sphere;
    sphere = gluNewQuadric();
    gluSphere(sphere, radius, slices, stacks);
    gluDeleteQuadric(sphere);
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1,0,0);
    glBegin(GL_LINE_STRIP);
    	glVertex3f(0,0,0);
    	glVertex3f(1.0,0,0);
    	glVertex3f(1.0,0,1.0);
    	glVertex3f(0,0,1.0);
    glEnd();
    glBegin(GL_LINE_STRIP);
    	glVertex3f(0,1,0);
    	glVertex3f(1,1,0);
    	glVertex3f(1,1,1);
    	glVertex3f(0,1,1);
    glEnd();
    glBegin(GL_LINES);
    	glVertex3f(0,0,0);
    	glVertex3f(0,1,0);
    	glVertex3f(1,0,0);
    	glVertex3f(1,1,0);
    	glVertex3f(1,0,1);
    	glVertex3f(1,1,1);
    	glVertex3f(0,0,1);
    	glVertex3f(0,1,1);
    glEnd();
    glColor3f(0,0,1);
    for(int i=0; i<s->particles.size(); i++){
        glPushMatrix();
        glTranslatef(s->particles[i].position[0], s->particles[i].position[1], s->particles[i].position[2]);
        drawSphere(0.0125, 20, 20);
        glPopMatrix();
    }
    glutSwapBuffers();
}

void timer(int state)
{
    for(int i = 0; i < 25; i++)
    s->update();
    glutPostRedisplay();
    glutTimerFunc(10, timer, 1);
}

void pressKey(unsigned char key,int x,int y)
{
    switch(key)
    {
        //Go forward
        case 'w': cam->translate(dvec3(0,0,-0.1)); break;
        //Go backward
        case 's': cam->translate(dvec3(0,0,0.1)); break;
        //Roll right
        case 'r': cam->rollY(-1.0); break;
        //Roll left
        case 'l': cam->rollY(1.0); break;
        //Roll anticlockwise
        case 'a': cam->rollZ(1.0); break;
        //Roll clockwise
        case 'c': cam->rollZ(-1.0); break;
        //Look up
        case 'u': cam->rollX(1.0); break;
        //Look down
        case 'd': cam->rollX(-1.0); break;
        default: break;
    }
    glutPostRedisplay();
}

void pressSpecialKey(int key,int x,int y)
{
    switch(key)
    {
        //Pan up
        case GLUT_KEY_UP: cam->translate(dvec3(0,0.1,0)); break;
        //Pan down
        case GLUT_KEY_DOWN: cam->translate(dvec3(0,-0.1,0)); break;
        //Pan left
        case GLUT_KEY_LEFT: cam->translate(dvec3(-0.1,0,0)); break;
        //Pan right
        case GLUT_KEY_RIGHT: cam->translate(dvec3(0.1,0,0)); break;
        default: break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    s = new Simulation();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(width, height);
    glutCreateWindow("FluidSim");
    glEnable(GL_DEPTH_TEST);
    cam = new Camera(width, height, dvec3(0.5, 0.1, 2), dvec3(0.5, 0.1, 0), dvec3(0,1,0), 0.1, 50.0);
    glClearColor(0, 0, 0, 0);
    glColor3f(1.0,0,0);
    glutDisplayFunc(draw);
    glutKeyboardFunc(pressKey);
    glutSpecialFunc(pressSpecialKey);
    glutTimerFunc(10,timer,1);
    glutMainLoop();
    
    return 0;
}

