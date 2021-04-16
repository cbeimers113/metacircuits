#include "Tool.h"

#include "ErrorWindow.h"
#include "..//..//simulation/Simulation.h"

class UnplugWindow : public ui::Window
{
public:
	ui::DropDown* inputs;
	Simulation* sim;
	UnplugTool* tool;
	Particle* part;
	UnplugWindow(UnplugTool* tool_, Simulation* sim_, Particle* part_);
	void RemoveConnection();
	void RemoveAll();
	void OnDraw() override;
	void DoDraw() override;
	void DoMouseMove(int x, int y, int dx, int dy) override;
	void DoMouseDown(int x, int y, unsigned button) override;
	void DoMouseUp(int x, int y, unsigned button) override
	{
		ui::Window::DoMouseUp(x, y, button);
	}
	void DoMouseWheel(int x, int y, int d) override
	{
		ui::Window::DoMouseWheel(x, y, d);
	}
	void DoKeyPress(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt) override
	{
		ui::Window::DoKeyPress(key, scan, repeat, shift, ctrl, alt);
	}
	void DoKeyRelease(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt) override
	{
		ui::Window::DoKeyRelease(key, scan, repeat, shift, ctrl, alt);
	}
	virtual ~UnplugWindow() {}
	void OnTryExit(ui::Window::ExitMethod method) override;
};

UnplugWindow::UnplugWindow(UnplugTool* tool_, Simulation* sim_, Particle* part_) :
	ui::Window(ui::Point(-1, -1), ui::Point(125, 75)),
	tool(tool_),
	sim(sim_),
	part(part_)
{
	ui::Label* messageLabel = new ui::Label(ui::Point(4, 5), ui::Point(Size.X - 8, 15), "Remove Connection");
	messageLabel->SetTextColour(style::Colour::InformationTitle);
	messageLabel->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	messageLabel->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	AddComponent(messageLabel);

	ui::Button* okayButton = new ui::Button(ui::Point(0, Size.Y - 16), ui::Point(Size.X / 3 + 1, 16), "OK");
	okayButton->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	okayButton->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	okayButton->Appearance.BorderInactive = (ui::Colour(200, 200, 200));
	okayButton->SetActionCallback({ [this] {
		RemoveConnection();
		CloseActiveWindow();
		SelfDestruct();
	} });
	AddComponent(okayButton);
	SetOkayButton(okayButton);

	ui::Button* doAllButton = new ui::Button(ui::Point(Size.X / 3, Size.Y - 16), ui::Point(Size.X / 3 + 2, 16), "All");
	doAllButton->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	doAllButton->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	doAllButton->Appearance.BorderInactive = (ui::Colour(200, 200, 200));
	doAllButton->SetActionCallback({ [this] {
		RemoveAll();
		CloseActiveWindow();
		SelfDestruct();
	} });
	AddComponent(doAllButton);

	ui::Button* cancelButton = new ui::Button(ui::Point(2 * Size.X / 3, Size.Y - 16), ui::Point(Size.X / 3 + 1, 16), "Cancel");
	cancelButton->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	cancelButton->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	cancelButton->Appearance.BorderInactive = (ui::Colour(200, 200, 200));
	cancelButton->SetActionCallback({ [this] {
		CloseActiveWindow();
		SelfDestruct();
	} });
	AddComponent(cancelButton);

	ui::Label* infoLabel = new ui::Label(ui::Point(4, 20), ui::Point(50, 15), "Select input to unwire");
	infoLabel->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	infoLabel->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	AddComponent(infoLabel);

	ui::Label* partLabel = new ui::Label(ui::Point(4, 37), ui::Point(50, 15), sim->ElementResolve(part->type, 0));
	partLabel->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	partLabel->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	AddComponent(partLabel);

	inputs = new ui::DropDown(ui::Point(65, 37), ui::Point(55, 16));
	AddComponent(inputs);
	for (int i = 0; i < part->inputs.size(); i++)
		inputs->AddOption(std::pair<String, int>(part->inputs[i].name, i));

	inputs->SetOption(0);
	inputs->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;

	FocusComponent(okayButton);
	MakeActiveWindow();
}

VideoBuffer* UnplugTool::GetIcon(int toolID, int width, int height)
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
	newTexture->AddCharacter((width / 2) - 5, (height / 2) - 5, 0xE042, 255, 50, 50, 255);
	return newTexture;
}

void UnplugWindow::OnDraw()
{
	Graphics* g = GetGraphics();
	g->clearrect(Position.X - 2, Position.Y - 2, Size.X + 3, Size.Y + 3);
	g->drawrect(Position.X, Position.Y, Size.X, Size.Y, 200, 200, 200, 255);
}

void UnplugWindow::DoDraw()
{
	ui::Window::DoDraw();
}

void UnplugWindow::DoMouseMove(int x, int y, int dx, int dy)
{
	ui::Window::DoMouseMove(x, y, dx, dy);
}

void UnplugWindow::DoMouseDown(int x, int y, unsigned button)
{
	ui::Window::DoMouseDown(x, y, button);
}

void UnplugWindow::OnTryExit(ui::Window::ExitMethod method)
{
	CloseActiveWindow();
	SelfDestruct();
}

void UnplugWindow::RemoveConnection() {
	part->inputs[inputs->GetOption().second].connX = 0;
	part->inputs[inputs->GetOption().second].connY = 0;
	part->inputs[inputs->GetOption().second].connT = 0;
	part->inputs[inputs->GetOption().second].connI = -1;
}

void UnplugWindow::RemoveAll() {
	for (int i = 0; i < part->inputs.size(); i++) {
		part->inputs[i].connX = 0;
		part->inputs[i].connY = 0;
		part->inputs[i].connT = 0;
		part->inputs[i].connI = -1;
	}
}

void UnplugTool::Click(Simulation* sim, Brush* brush, ui::Point position) {
	int r = sim->pmap[position.Y][position.X];
	if (r && sim->parts[ID(r)].inputs.size())
		new UnplugWindow(this, sim, &(sim->parts[ID(r)]));
}
