#include <GL/glut.h>
#include "Vector3D.h"
#include <math.h>

Vector3D::Vector3D() {}

Vector3D::Vector3D(GLfloat x, GLfloat y, GLfloat z) :
	x(x),
	y(y),
	z(z)
{

}

void Vector3D::set(GLfloat x, GLfloat y, GLfloat z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3D::set(GLfloat* v) {
	this->x = v[0];
	this->y = v[1];
	this->z = v[2];
}

void Vector3D::normalize() {
	float length = sqrtf(x * x + y * y + z * z);
	if (length != 0) {
		this->x /= length;
		this->y /= length;
		this->z /= length;
	}
}


GLfloat Vector3D::dot(Vector3D v) {
	GLfloat result = 0.0;
	GLfloat a[] = { x, y, z };
	GLfloat va[] = { v.x, v.y, v.z };
	for (int i = 0; i < 3; i++) {
		result += a[i] * va[i];
	}
	return result;
}

GLfloat* Vector3D::cross(Vector3D v) {
	GLfloat result[3];
	GLfloat a[] = { x, y, z };
	GLfloat b[] = { v.x, v.y, v.z };

	result[0] = a[1] * b[2] - a[2] * b[1];
	result[1] = a[2] * b[0] - a[0] * b[2];
	result[2] = a[0] * b[1] - a[1] * b[0];

	return result;
}