#include <iostream>
#include <math.h>
#include <GL/glut.h>

#include "Planet.h"
#include "texture.h"

Planet::Planet(string name, double aphelion, double perihelion, double orbitalPeriod,
    double rotationPeriod,
    string texturePath, double size, double inclination, GLboolean drawOrbitPath, Planet* satelliteFrom) :
    texture(new Texture(texturePath)),
    rotation(rotationPeriod),
    name(name),
    aphelion(aphelion),
    currentAxisLocation(0),
    perihelion(perihelion),
    orbitalPeriod(orbitalPeriod),
    texturePath(texturePath),
    size(size),
    satelliteFrom(satelliteFrom),
    inclinationZ(inclination),
    inclinationY(rand() % 180),
    drawOrbitPath(drawOrbitPath)
{

}

Planet::~Planet() {
    delete texture;
    delete satelliteFrom;
}

void Planet::Orbit(double degree, double scaleAxisRotation)
{
    currentAxisLocation += degree * scaleAxisRotation / rotation;
    currentSystemLocation += degree / (orbitalPeriod == 0 ? 1 : orbitalPeriod);

    currentSystemLocation = currentSystemLocation > 360 ? currentSystemLocation - 360 : currentSystemLocation;
    currentAxisLocation = currentAxisLocation > 360 ? currentAxisLocation - 360 : currentAxisLocation;
}

void Planet::Draw()
{
    GLfloat position[] = { -2.0, 0.0, 0.0, 1.0 };
    GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1.0 };

    const double pi = 3.1416;
    const double degreesPerARadian = 57.29577951;
    transferSystemX = 0.05 * (aphelion + perihelion);
    transferSystemZ = 0.05 * (aphelion + perihelion);

    if (name == "Sun")
        pX = -2;
    else
        pX = -cos((double)(currentSystemLocation / degreesPerARadian + pi)) * transferSystemX;

    pY = 0;

    if (satelliteFrom == NULL)
        pZ = sin((double)(currentSystemLocation / degreesPerARadian + pi)) * transferSystemZ;
    else
        pZ = sin((double)(currentSystemLocation / degreesPerARadian + pi)) * transferSystemX;


    if (name == "Sun")
    {
        glLightfv(GL_LIGHT0, GL_POSITION, position);
        glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
    }
    else
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        if (satelliteFrom == NULL)
        {
            pX += -(transferSystemX + transferSystemZ) / 3;
        }
    }

    if (satelliteFrom != NULL)
    {
        glPushMatrix();
        glTranslated(satelliteFrom->pX, satelliteFrom->pY, satelliteFrom->pZ);
    }

    //create the orbit
    createOrbit = false;
    glPushMatrix();
    if (satelliteFrom == NULL && name != "Sun")
    {
        createOrbit = true;
    }
    glPopMatrix();

    glPushMatrix();
    if (satelliteFrom != NULL)
    {
        glRotated(inclinationY, 0, 1, 0);
        glRotated(inclinationZ, 1, 0, 0);
    }
    glTranslated(pX, pY, pZ);

    //create the planet
    GLUquadric* qobj = gluNewQuadric();
    gluQuadricTexture(qobj, GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    glRotated(270.0f, 1.0f, 0.0f, 0.0f);
    texture->Bind();

    glPushMatrix();
    glRotated(currentAxisLocation, 0, 0, 1);
    gluSphere(qobj, size / 3, 30, 30);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    gluDeleteQuadric(qobj);

    if (name != "Sun")
    {
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);
    }
    glPopMatrix();

    if (satelliteFrom != NULL)
    {
        glPopMatrix();
    }
}

float Planet::getTransferSystemX() {
    return transferSystemX;
}

float Planet::getTransferSystemZ() {
    return transferSystemZ;
}
