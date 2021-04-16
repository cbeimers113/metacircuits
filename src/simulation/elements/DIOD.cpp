#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_DIOD()
{
	Identifier = "DEFAULT_PT_DIOD";
	Name = "DIOD";
	Colour = PIXPACK(0x0756b0);
	MenuVisible = 1;
	MenuSection = SC_CRACKER;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f * CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 100;

	HeatConduct = 251;
	Description = "Diode. Converts circuit signal to an electrical signal.";

	Properties = TYPE_SOLID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
	Create = &create;
}

static int update(UPDATE_FUNC_ARGS)
{
	if (!parts[i].inputs.size()) return 0;

	int r, rx, ry, rt;
	WireIn input = parts[i].inputs[0];

	if (input.connI != -1) {
		int tx = parts[i].x + input.connX;
		int ty = parts[i].y + input.connY;
		int target = pmap[ty][tx];
		if (!target) return 0;

		for (rx = -1; rx < 2; rx++)
			for (ry = -1; ry < 2; ry++)
				if (BOUNDS_CHECK)
				{
					int data = parts[ID(target)].data(UPDATE_FUNC_SUBCALL_ARGS)[input.connI];
					parts[i].tmp = data;
					r = pmap[y + ry][x + rx];
					rt = TYP(r);

					if (data && (sim->elements[rt].Properties & PROP_CONDUCTS) && !parts[ID(r)].life) {
						parts[ID(r)].ctype = rt;
						parts[ID(r)].life = 4;
						sim->part_change_type(ID(r), x + rx, y + ry, PT_SPRK);
					}
					else if (data && rt == PT_INST) {
						sim->FloodINST(x + rx, y + ry);
					}
				}
	}
	return 0;
}

static void create(ELEMENT_CREATE_FUNC_ARGS) {
	sim->parts[i].inputs.push_back(WireIn{ "IN" });
}