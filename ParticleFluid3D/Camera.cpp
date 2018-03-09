#include "Camera.h"

Camera::Camera(double Width, double Height, dvec3 eye_pos, dvec3 eye_look_at, dvec3 up_vec, double zNear, double zFar)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_pos[0], eye_pos[1], eye_pos[2], eye_look_at[0], eye_look_at[1], eye_look_at[2], up_vec[0], up_vec[1], up_vec[2]);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, Width/Height, zNear, zFar);
}

void Camera::translate(dvec3 translation)
{
    double temp[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, temp);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-translation.x, -translation.y, -translation.z);
    glMultMatrixd(temp);
}

void Camera:: rollX(double angle)
{
    double temp[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, temp);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-angle, 1, 0, 0);
    glMultMatrixd(temp);
}

void Camera:: rollY(double angle)
{
    double temp[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, temp);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-angle, 0, 1, 0);
    glMultMatrixd(temp);
}

void Camera:: rollZ(double angle)
{
    double temp[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, temp);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-angle, 0, 0, 1);
    glMultMatrixd(temp);
}
