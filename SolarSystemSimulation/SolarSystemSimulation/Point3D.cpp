#include <GL/glut.h>
#include "Point3D.h"

Point3D::Point3D(void) {}

Point3D::Point3D(GLfloat x, GLfloat y, GLfloat z) :
	x(x),
	y(y),
	z(z)
{

}

void Point3D::set(Point3D point) {
	this->x = point.x;
	this->y = point.y;
	this->z = point.z;
}