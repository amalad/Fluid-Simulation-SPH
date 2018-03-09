#ifndef CAMERA_H
#define CAMERA_H

#include <bits/stdc++.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#define PI 3.14159265

using namespace glm;
using namespace std;

class Camera
{
    public:
        /**
         * Class constructor
         * @param width Width of the window
         * @param height Height of the window
         */
        Camera(double Width, double Height, dvec3 eye_pos, dvec3 eye_look_at, dvec3 up_vec, double zNear, double zFar);
        
        /**
         * Rotates the camera around X-axis of camera
         * @param angle Angle of rotation
         */
        void rollX(double angle);
        
        /**
         * Rotates the camera around Y-axis of camera
         * @param angle Angle of rotation
         */
        void rollY(double angle);
        /**
         * Rotates the camera around Z-axis of camera
         * @param angle Angle of rotation
         */
        void rollZ(double angle);
        /**
         * Translates the camera
         * @param displacement Displacement vector to be added to camera position
         */
        void translate(dvec3 translation);
};

#endif
