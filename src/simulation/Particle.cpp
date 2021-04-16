#include <cstddef>
#include "Particle.h"
#include "ElementCommon.h"

std::vector<int> Particle::data(UPDATE_FUNC_ARGS) {
	int r, rx, ry, data = 0;
	int ox = this->x;
	int oy = this->y;
	tmp = 0;

	std::vector<int>ld;

	for (int nInps = 0; nInps < inputs.size(); nInps++) {
		int tx = ox + inputs[nInps].connX;
		int ty = oy + inputs[nInps].connY;
		int td = inputs[nInps].connI != -1 ? parts[ID(pmap[ty][tx])].data(UPDATE_FUNC_SUBCALL_ARGS)[inputs[nInps].connI] : 0;

		ld.push_back(td);
	}

	// Discard the 'new' input for AND and NAND
	if (inputs.size() > 2 && (type == PT_AND || type == PT_NAND))
		ld.pop_back();

	switch (type) {
	case PT_DIGI:
		for (rx = -1; rx < 2; rx++)
			for (ry = -1; ry < 2; ry++)
				if (BOUNDS_CHECK)
				{
					r = pmap[oy + ry][ox + rx];
					if (TYP(r) == PT_SPRK)
						data = 1;
				}
		break;
	case PT_JOIN:
		for (int d : ld)
			if (d)
				data = 1;
		break;
	case PT_TRNS:
		data = life ? ld[2] : 0;
		break;
	case PT_BUFR:
	case PT_NOT:
		data = ld[0];
		break;
	case PT_AND:
	case PT_NAND:
		data = 1;
		for (int d : ld)
			if (!d)
				data = 0;
		break;
	case PT_OR:
	case PT_NOR:
		for (int d : ld)
			if (d)
				data = 1;
		break;
	case PT_XOR:
	case PT_XNOR:
		data = (ld[0] && !ld[1]) || (ld[1] && !ld[0]);
		break;
	}

	if (type == PT_NOT || type == PT_NOR || type == PT_NAND || type == PT_XNOR)
		data = data ? 0 : 1;
	tmp = data;
	return std::vector<int>(1, data);
};

std::vector<StructProperty> const& Particle::GetProperties()
{
	static std::vector<StructProperty> properties = {
		{ "type"   , StructProperty::ParticleType, offsetof(Particle, type) },
		{ "life"   , StructProperty::ParticleType, offsetof(Particle, life) },
		{ "ctype"  , StructProperty::ParticleType, offsetof(Particle, ctype) },
		{ "x"      , StructProperty::Float       , offsetof(Particle, x) },
		{ "y"      , StructProperty::Float       , offsetof(Particle, y) },
		{ "vx"     , StructProperty::Float       , offsetof(Particle, vx) },
		{ "vy"     , StructProperty::Float       , offsetof(Particle, vy) },
		{ "temp"   , StructProperty::Float       , offsetof(Particle, temp) },
		{ "flags"  , StructProperty::UInteger    , offsetof(Particle, flags) },
		{ "tmp"    , StructProperty::Integer     , offsetof(Particle, tmp) },
		{ "tmp2"   , StructProperty::Integer     , offsetof(Particle, tmp2) },
		{ "dcolour", StructProperty::UInteger    , offsetof(Particle, dcolour) },
		{ "pavg0"  , StructProperty::Float       , offsetof(Particle, pavg[0]) },
		{ "pavg1"  , StructProperty::Float       , offsetof(Particle, pavg[1]) },
	};
	return properties;
}
