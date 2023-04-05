#include <GL/glut.h>
#include "Universe.h"

Universe::Universe(const std::string texturePath, double size) :
	texture(new Texture(texturePath)),
	size(size)
{

}


Universe::~Universe() {
	delete texture;
}

void Universe::draw() {
	GLUquadric* qobj = gluNewQuadric();
	gluQuadricOrientation(qobj, GLU_INSIDE);
	gluQuadricTexture(qobj, GL_TRUE);
	glEnable(GL_TEXTURE_2D);

	glRotated(270.0f, 1.0f, 0.0f, 0.0f);
	texture->Bind();

	glPushMatrix();
	gluSphere(qobj, size, 30, 30);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	gluDeleteQuadric(qobj);
	glPopMatrix();
}