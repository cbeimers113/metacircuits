#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <vector>
#include "StructProperty.h"
#include "ElementDefs.h"

class Simulation;

struct WireIn
{
	String name;
	int connX = 0;
	int connY = 0;
	int connT = 0;
	int connI = -1;
};

struct Particle
{
	int type;
	int life, ctype;
	float x, y, vx, vy;
	float temp;
	float pavg[2];
	int flags;
	int tmp;
	int tmp2;
	unsigned int dcolour;
	std::vector<WireIn>inputs;
	std::vector<String>outputs;
	std::vector<int> data(UPDATE_FUNC_ARGS);
	/** Returns a list of properties, their type and offset within the structure that can be changed
	 by higher-level processes referring to them by name such as Lua or the property tool **/
	static std::vector<StructProperty> const& GetProperties();
};

#endif
