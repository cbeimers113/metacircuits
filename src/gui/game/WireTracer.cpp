#include "Tool.h"

#include "ErrorWindow.h"
#include "..//..//simulation/Simulation.h"
#include "..//..//simulation/ElementCommon.h"
#include "..//..//simulation/Circuit.h"

VideoBuffer* WireTracer::GetIcon(int toolID, int width, int height)
{
	VideoBuffer* newTexture = new VideoBuffer(width, height);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			pixel pc = x == 0 || x == width - 1 || y == 0 || y == height - 1 ? PIXPACK(0xA0A0A0) : PIXPACK(0x000000);
			newTexture->SetPixel(x, y, PIXR(pc), PIXG(pc), PIXB(pc), 255);
		}
	}
	newTexture->AddCharacter(2, (height / 2) - 5, 0xE06a, 0, 85, 200, 255);
	return newTexture;
}

void WireTracer::Click(Simulation* sim, Brush* brush, ui::Point position) {
	int r = sim->pmap[position.Y][position.X];
	int id = ID(r);
	if (r) 
		sim->debugIndex = id == sim->debugIndex ? -1 : id;
	else
		sim->debugIndex = -1;
}