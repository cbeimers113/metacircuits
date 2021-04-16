#include "simulation/ElementCommon.h"

static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_XNOR()
{
	Identifier = "DEFAULT_PT_XNOR";
	Name = "XNOR";
	Colour = PIXPACK(0x7c008c);
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
	Description = "XNOR gate. Outputs a signal only if both inputs are the same.";

	Properties = TYPE_SOLID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Create = &create;
}

static void create(ELEMENT_CREATE_FUNC_ARGS) {
	sim->parts[i].inputs.push_back(WireIn{ "IN1" });
	sim->parts[i].inputs.push_back(WireIn{ "IN2" });
	sim->parts[i].outputs.push_back("OUT");
}