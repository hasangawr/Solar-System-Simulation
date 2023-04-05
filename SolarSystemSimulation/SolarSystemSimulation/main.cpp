#include <iostream>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctime>
#include "List.h"
#include "Planet.h"
#include "Universe.h"
#include "Camera.h"

#define PI 3.1416

// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 100.0f, z = 300.0f;
float speed = 0.5;
Camera camera;
string cameraMode = "FIRST_PERSON";
int camSetCount = 0;

Planet* planetHover;
List<Planet*>* planets;
Universe* universe;

GLuint stars;
double multiplyTime = 0.002;
double clockCounter = 0;
double zoom = 5;
double dragX = -1;
double dragY = -1;
double mouseX;
double mouseY;
double eliminateA = 0;
double eliminateB = 0;
GLfloat distanceStars = 600;
GLboolean drawOrbitPath = false;

using namespace std;

void initPlanets();
void menu(unsigned char key);
void initSystem();
void resize(int w, int h);
void display();
void keyboard(unsigned char key, int A, int B);
void goToPlanet(int index);
void mouseMovement(int x, int y);
void orbit();
void mouse(int button, int state, int x, int y);

void initPlanets()
{
    planets = new List<Planet*>();

    int scale = 30;
    int sizeScale = 1.5;

    planets->Add(new Planet("Sun", 0, 0, 0, 20, "textures/2k_sun.jpg", 109/4, 1.0, drawOrbitPath));
    planets->Add(new Planet("Mercury", 7.0 * scale, 4.6 * scale, 0.24, 58.82, "textures/2k_mercury.jpg", .38 * sizeScale, 1.0, drawOrbitPath));
    planets->Add(new Planet("Venus", 10.893 * scale, 10.747 * scale, 0.61, 243, "textures/2k_venus_atmosphere.jpg", .815 * sizeScale, 1.0, drawOrbitPath));
    planets->Add(new Planet("Earth", 15.19 * scale, 14.95 * scale, 4.15, 1, "textures/Tierra.jpg", 1 * sizeScale, 25.0, drawOrbitPath));
    planets->Add(new Planet("Mars", 20.67 * scale, 24.92 * scale, 1.8821, 1, "textures/2k_mars.jpg", .53 * sizeScale, 23.0, drawOrbitPath));
    planets->Add(new Planet("Jupiter", 81.6 * scale, 74.05 * scale, 11.86, 0.41, "textures/2k_jupiter.jpg", 11 * sizeScale, 1, drawOrbitPath));
    planets->Add(new Planet("Saturn", 135.36 * scale, 151.3 * scale, 29.5, 0.445, "textures/2k_saturn.jpg", 9.13 * sizeScale, 23.0, drawOrbitPath));
    planets->Add(new Planet("Uranus", 300.6 * scale, 273.5 * scale, 84, -0.720, "textures/2k_uranus.jpg", 4 * sizeScale, 98, drawOrbitPath));
    planets->Add(new Planet("Neptune", 453.7 * scale, 445.9 * scale, 164.7, 1, "textures/2k_neptune.jpg", 3.88 * sizeScale, 1, drawOrbitPath));

    planets->Add(new Planet("Pluto", 730 * scale, 440 * scale, 247.68, 6.41, "textures/2k_eris_fictional.jpg", 0.186 * sizeScale, 120, drawOrbitPath));

    // Moons
    planets->Add(new Planet("Moon", 8, 8, 0.07, 2.5, "textures/2k_moon.jpg", 0.18 * sizeScale, 15.0, false, planets->Get(3)));

    planetHover = planets->Get(0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

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

void drawOrbitPathOnPlanet(float transferSystemX, float transferSystemZ) {
    glPushMatrix();
    glTranslated(-(transferSystemX + transferSystemZ) / 3, 0, 0);
    glScaled(1, 1, transferSystemZ / transferSystemX);
    glRotated(90, 1.0, 0.0, 0.0);

    glPushMatrix();
    glColor3f(0.1f, 0.3f, 0.5f);
    if (drawOrbitPath)
    {
        drawCircle(transferSystemX);
    }
    glColor3f(1.0f, 1.0f, 1.0f);

    glPopMatrix();
    glPopMatrix();
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 3000.0);
}

//start of spaceship code
void drawCircle1(double radius) {
    int d;
    glBegin(GL_POLYGON);
    for (d = 0; d < 32; d++)
    {
        double angle = 2 * PI / 32 * d;
        glVertex2d(radius * cos(angle), radius * sin(angle));
    }
    glEnd();
}

void drawCylinder(GLfloat baseRadius, GLfloat topRadius, GLfloat height, GLint slices, GLint stacks) {
    const GLfloat silver[4] = { 0.75, 0.75, 0.75, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, silver);
    const GLfloat matwhite[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, matwhite);
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0f);

    GLUquadricObj* quadric = gluNewQuadric();
    gluCylinder(quadric, baseRadius, topRadius, height, slices, stacks);
    gluDeleteQuadric(quadric);

    drawCircle1(baseRadius);

    glPushMatrix();
    glTranslatef(0.0, 0.0, height);
    drawCircle1(topRadius);
    glPopMatrix();
}

void drawSpaceShip() {
    const GLfloat silver[4] = { 0.75, 0.75, 0.75, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, silver);
    const GLfloat matwhite[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, matwhite);
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0f);

    //body
    drawCylinder(0.3, 0.3, 0.8, 32, 1);

    //front
    glutSolidSphere(0.3, 32, 32);

    //engines
    glPushMatrix();
    glTranslatef(0.25, 0.0, 0.0);
    drawCylinder(0.05, 0.15, 0.1, 32, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.25, 0.0, 0.0);
    drawCylinder(0.05, 0.15, 0.1, 32, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.25, 0.0, 0.1);
    drawCylinder(0.15, 0.20, 0.5, 32, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.25, 0.0, 0.1);
    drawCylinder(0.15, 0.20, 0.5, 32, 1);
    glPopMatrix();

    //back
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.8);
    drawCylinder(0.20, 0.25, 0.15, 32, 1);
    glPopMatrix();

    //top
    const GLfloat blue[4] = { 0.18, 0.5, 0.9, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matwhite);
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0f);

    glPushMatrix();
    glTranslatef(0.0, 0.2, 0.2);
    glutSolidSphere(0.2, 32, 32);
    glPopMatrix();
}
//end of spaceship code

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_LIGHT2);
    GLfloat position[] = { 100.0,100.0,100.0,1.0 };
    glLightfv(GL_LIGHT2, GL_POSITION, position);

    GLfloat paleYellow[] = { 1.0,1.0,0.75,1.0 };
    glLightfv(GL_LIGHT2, GL_DIFFUSE, paleYellow);
    GLfloat white[] = { 1.0,1.0,1.0,1.0 };
    glLightfv(GL_LIGHT2, GL_SPECULAR, white);


    //cam
    if (cameraMode == "THIRD_PERSON") {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(x, 1.0f, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);

        //draw the spaceship attached to camera
        glEnable(GL_LIGHTING);
        glPushMatrix();
        glTranslatef(x, 0.0, z); // Translation to the camera center
        glScalef(0.2, 0.2, 0.2);
        glRotatef(-angle * 57.2957795, 0, 1, 0); // Rotate to correspond to the camera
        glTranslatef(0.0, 2.0, -8.5); // Offset to draw the object
        drawSpaceShip();
        glPopMatrix();
        glDisable(GL_LIGHTING);
    }

    if ((cameraMode == "FIRST_PERSON") && (camSetCount < 1))
    {
        camera.set(15.0, 2.0, 2.0, 14.0, -5.0, -5.0, 0.0, 1.0, 0.0);
        camSetCount++;
    }
    //

    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
    universe->draw();
    for (int i = 0; i < planets->Length(); i++)
        if (planets->Get(i)->name == "Sun")
        {
            glPushMatrix();
            glTranslatef(-4.0, 0.0, 0.0);
            planets->Get(i)->Draw();
            glPopMatrix();
        }
        else
        {
            planets->Get(i)->Draw();
        }
        
    glPopMatrix();

    glPushMatrix();
    for (int i = 0; i < planets->Length(); i++)
        if (planets->Get(i)->createOrbit)
        {
            drawOrbitPathOnPlanet(planets->Get(i)->getTransferSystemX(), planets->Get(i)->getTransferSystemZ());
        }
    glPopMatrix();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int A, int B)
{
    int number = key - '0';
    switch (key)
    {
    case 'w': camera.slide(0, 0, -speed);
        glutPostRedisplay();
        break;
    case 's': camera.slide(0, 0, speed);
        glutPostRedisplay();
        break;
    case 'a': camera.slide(-speed, 0, 0.0);
        glutPostRedisplay();
        break;
    case 'd': camera.slide(speed, 0, 0.0);
        glutPostRedisplay();
        break;
    case 'f': cameraMode = "FIRST_PERSON";
        glutPostRedisplay();
        break;
    case 't': cameraMode = "THIRD_PERSON";
        glutPostRedisplay();
        break;
    case 'o': drawOrbitPath = true;
        glutPostRedisplay();
        break;
    case 'n': drawOrbitPath = false;
        glutPostRedisplay();
        break;
    case '-': multiplyTime /= multiplyTime < 0.001 ? 1 : 2;
        break;
    case '+': multiplyTime *= multiplyTime > 10 ? 1 : 2;
        break;
    case 'q': speed++;
        break;
    case 'e': speed--;
        break;
    case 27:  exit(0);
        break;
    default:  break;
    }
    if (number >= 0 && number < 10) {
        planetHover = planets->Get(number);
    }
}

//cam
void processSpecialKeys(int key, int xx, int yy) {

    float fraction = 0.1f;

    switch (key) {
    case GLUT_KEY_LEFT:
        angle -= 0.01f;
        lx = sin(angle);
        lz = -cos(angle);
        break;
    case GLUT_KEY_RIGHT:
        angle += 0.01f;
        lx = sin(angle);
        lz = -cos(angle);
        break;
    case GLUT_KEY_UP:
        x += lx * speed;
        z += lz * speed;
        break;
    case GLUT_KEY_DOWN:
        x -= lx * speed;
        z -= lz * speed;
        break;
    }
}
//

void orbit()
{
    long newClockCounter = clock();

    for (int i = 0; i < planets->Length(); i++)
        planets->Get(i)->Orbit(multiplyTime * ((newClockCounter - clockCounter) / 20), 30);

    clockCounter = newClockCounter;
    glutPostRedisplay();
}

void initSystem()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Solar System Simulation");

    initPlanets();
    universe = new Universe("textures/2k_stars+milky_way.jpg", 1200);
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(processSpecialKeys);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    glutIdleFunc(orbit);
    glutMainLoop();

}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    initSystem();
    return 0;
}

