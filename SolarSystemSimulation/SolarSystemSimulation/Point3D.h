#ifndef POINT3D_H
#define POINT3D_H

#include <GL/glut.h>

class Point3D
{
private:


public:
	GLfloat x;
	GLfloat y;
	GLfloat z;

	Point3D(void);

	Point3D(GLfloat x, GLfloat y, GLfloat z);

	void set(Point3D point);

};

#endif