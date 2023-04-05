#ifndef Planet_h
#define Planet_h

#include <stdio.h>
#include <string>
#include "texture.h"

using namespace std;

class Planet
{
public:
    double _aphelion;
    float radio = 100.0;
    string name;
    double orbitalSpeed;
    double aphelion;
    double perihelion;
    double orbitalPeriod;
    double rotation;
    double inclinationZ;
    double inclinationY;
    double size;
    double timeDelta;
    GLboolean drawOrbitPath;
    GLboolean createOrbit;

    double currentAxisLocation;
    double currentSystemLocation;

    double pX;
    double pY;
    double pZ;

    double transferSystemX;
    double transferSystemZ;

    string texturePath;
    GLuint TexturePath;
    Texture* texture;
    Planet* satelliteFrom;

    Planet(string name, double aphelion, double perihelion, double orbitalPeriod, double rotationPeriod,
        const std::string texturePath, double size, double inclination, GLboolean drawOrbitPath, Planet* satelliteFrom = NULL);

    virtual ~Planet();

    void Orbit(double degree, double scaleAxisRotation);

    void Draw();

    float getTransferSystemX();

    float getTransferSystemZ();

private:
    Planet(const Planet& planet) {}
    void operator=(const Planet& planet) {}

};


#endif
