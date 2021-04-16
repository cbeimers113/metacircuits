#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_LED()
{
	Identifier = "DEFAULT_PT_LED";
	Name = "LED";
	Colour = PIXPACK(0x007900);
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
	Description = "Light emitting diode. Lights up when it recieves a circuit signal.";

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
	Graphics = &graphics;
	Create = &create;
}

static int update(UPDATE_FUNC_ARGS)
{
	if (!parts[i].inputs.size()) return 0;

	WireIn input = parts[i].inputs[0];

	if (input.connI != -1) {
		int tx = parts[i].x + input.connX;
		int ty = parts[i].y + input.connY;
		int target = pmap[ty][tx];
		if (!target) return 0;
		int data = parts[ID(target)].data(UPDATE_FUNC_SUBCALL_ARGS)[input.connI];
		parts[i].tmp = data;
	}
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	if (cpart->tmp)
	{
		*colr = 0;
		*colg = 255;
		*colb = 0;
		*cola = 255;
		*pixel_mode |= PMODE_GLOW;
	}
	return 0;
}

static void create(ELEMENT_CREATE_FUNC_ARGS) {
	sim->parts[i].inputs.push_back(WireIn{ "IN" });
}

