#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_TRNS()
{
	Identifier = "DEFAULT_PT_TRNS";
	Name = "TRNS";
	Colour = PIXPACK(0x6e6e6e);
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
	Description = "Transistor. Allows throughput from IN to OUT when enabled.";

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

	WireIn on = parts[i].inputs[0];
	WireIn off = parts[i].inputs[1];

	int onX = parts[i].x + on.connX;
	int onY = parts[i].y + on.connY;
	int onT = pmap[onY][onX];
	int sigOn = on.connI == -1 ? 0 : (onT && parts[ID(onT)].data(UPDATE_FUNC_SUBCALL_ARGS)[on.connI]) ? 1 : 0;

	int offX = parts[i].x + off.connX;
	int offY = parts[i].y + off.connY;
	int offT = pmap[offY][offX];
	int sigOff = off.connI == -1 ? 0 : (offT && parts[ID(offT)].data(UPDATE_FUNC_SUBCALL_ARGS)[off.connI]) ? 1 : 0;

	if (sigOn ^ sigOff)
		parts[i].life = sigOn;

	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS) {
	if (cpart->life)
	{
		*colr = 200;
		*colg = 200;
		*colb = 200;
		*pixel_mode |= PMODE_GLOW;
	}
	return 0;
}

static void create(ELEMENT_CREATE_FUNC_ARGS) {
	sim->parts[i].inputs.push_back(WireIn{ "ON" });
	sim->parts[i].inputs.push_back(WireIn{ "OFF" });
	sim->parts[i].inputs.push_back(WireIn{ "IN" });
	sim->parts[i].outputs.push_back("OUT");
}