#ifndef CAMERA_H
#define CAMERA_H

#include "Point3D.h"
#include "Vector3D.h"

class Camera
{
private:
	Point3D eye;
	Vector3D u, v, n;
	/* private method to set up modelview matrix according n,u,v */
	void setModelViewMatrix();
public:
	Camera() {};
	/* this method set up camera */
	void set(float ex, float ey, float ez, float lx, float ly, float
		lz, float ux, float uy, float uz);
	void roll(float angle); /* n- axis rotation */
	void pitch(float angle); /* u- axis rotation */
	void yaw(float angle); /* v- axis rotation */
	void slide(float du, float dv, float dn);
	/*left,right,forward,backward,up and down movements*/
	void setOrthoViewVolume(float left, float right, float bottom, float
		top, float near, float far, float asp);
	void setPersViewVolume(float vAng, float asp, float nearD, float farD);
	GLfloat* getEye();
};

#endif