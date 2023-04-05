#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <GL/glut.h>

class Vector3D
{
private:


public:
	GLfloat x;
	GLfloat y;
	GLfloat z;

	Vector3D();

	Vector3D(GLfloat x, GLfloat y, GLfloat z);

	void set(GLfloat x, GLfloat y, GLfloat z);

	void set(GLfloat* v);

	void normalize();

	GLfloat dot(Vector3D v);

	GLfloat* cross(Vector3D v);

};

#endif