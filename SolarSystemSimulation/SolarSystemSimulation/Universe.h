#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <string>
#include "texture.h"

class Universe
{
public:

	Universe(const std::string texturePath, double size);
	virtual ~Universe();

	void draw();

private:
	Universe(const Universe& universe) {}
	void operator=(const Universe& other) {}


	Texture* texture;
	double size;
};

#endif