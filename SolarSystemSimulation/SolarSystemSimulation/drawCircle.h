#include <math.h>
#include<GL/glut.h>

#define PI 3.1416

void drawCircle(float x2) {
    int detail = 180;

    glPushMatrix();
    glScaled(x2, x2, x2);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < detail; ++i) {
        glVertex2d(cos(2 * i * PI / detail), sin(2 * i * PI / detail));
    }
    glEnd();
    glPopMatrix();
}