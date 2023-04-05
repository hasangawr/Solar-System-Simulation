#include <GL/glut.h>
#include <math.h>

#include "Camera.h"

void Camera::setModelViewMatrix(void)
{
	float m[16];
	Vector3D eVec(eye.x, eye.y, eye.z);
	m[0] = u.x; m[4] = u.y; m[8] = u.z; m[12] = -eVec.dot(u);
	m[1] = v.x; m[5] = v.y; m[9] = v.z; m[13] = -eVec.dot(v);
	m[2] = n.x; m[6] = n.y; m[10] = n.z; m[14] = -eVec.dot(n);
	m[3] = 0; m[7] = 0; m[11] = 0; m[15] = 1;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m);
}

void Camera::set(float ex, float ey, float ez, float lx, float ly, float lz, float ux, float uy, float uz)
{
	Point3D Eye(ex, ey, ez);
	Point3D look(lx, ly, lz);
	Vector3D up(ux, uy, uz);
	eye.set(Eye);
	n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);
	u.set(up.cross(n));
	n.normalize();
	u.normalize();
	v.set(n.cross(u));
	setModelViewMatrix();
}

void Camera::slide(float du, float dv, float d)
{
	eye.x += du * u.x + dv * v.x + d * n.x;
	eye.y += du * u.y + dv * v.y + d * n.y;
	eye.z += du * u.z + dv * v.z + d * n.z;
	setModelViewMatrix();
}

void Camera::roll(float angle)
{
	float cs = cos(3.14 / 180 * angle);
	float sn = sin(3.14 / 180 * angle);
	Vector3D t = u;
	u.set(cs * t.x - sn * v.x, cs * t.y - sn * v.y, cs * t.z - sn * v.z);
	v.set(sn * t.x + cs * v.x, sn * t.y + cs * v.y, sn * t.z + cs * v.z);
	setModelViewMatrix();
}

void Camera::pitch(float angle)
{
	float cs = cos(3.14 / 180 * angle);
	float sn = sin(3.14 / 180 * angle);
	Vector3D t = v;
	v.set(cs * t.x - sn * n.x, cs * t.y - sn * n.y, cs * t.z - sn * n.z);
	n.set(sn * t.x + cs * n.x, sn * t.y + cs * n.y, sn * t.z + cs * n.z);
	setModelViewMatrix();
}

void Camera::yaw(float angle)
{
	float cs = cos(3.14 / 180 * angle);
	float sn = sin(3.14 / 180 * angle);
	Vector3D t = u;
	u.set(cs * t.x + sn * n.x, cs * t.y + sn * n.y, cs * t.z + sn * n.z);
	n.set(cs * n.x - sn * t.x, cs * n.y - sn * t.y, cs * n.z - sn * t.z);
	setModelViewMatrix();
}

GLfloat* Camera::getEye() {
	GLfloat result[] = { eye.x, eye.y, eye.z };
	return result;
}
