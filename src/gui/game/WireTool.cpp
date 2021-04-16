#include "Tool.h"

#include "ErrorWindow.h"
#include "..//..//simulation/Simulation.h"
#include "..//..//simulation/ElementCommon.h"
#include "..//..//simulation/Circuit.h"

class WireWindow : public ui::Window
{
public:
	ui::DropDown* inputs;
	ui::DropDown* outputs;
	ui::Label* statusLabel;
	WireTool* tool;
	Simulation* sim;
	Particle* fromPart;
	Particle* toPart;
	WireWindow(WireTool* tool_, Simulation* sim_, Particle* fromPart_, Particle* toPart);
	bool IsNewWire();
	String GetStatusMsg();
	void SetConnection();
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
	virtual ~WireWindow() {}
	void OnTryExit(ui::Window::ExitMethod method) override;
};

WireWindow::WireWindow(WireTool* tool_, Simulation* sim_, Particle* fromPart_, Particle* toPart_) :
	ui::Window(ui::Point(-1, -1), ui::Point(125, 90)),
	tool(tool_),
	sim(sim_),
	fromPart(fromPart_),
	toPart(toPart_)
{
	ui::Label* messageLabel = new ui::Label(ui::Point(4, 5), ui::Point(Size.X - 8, 15), "Wire Connection");
	messageLabel->SetTextColour(style::Colour::InformationTitle);
	messageLabel->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	messageLabel->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	AddComponent(messageLabel);

	ui::Button* okayButton = new ui::Button(ui::Point(0, Size.Y - 16), ui::Point(Size.X / 2 + 1, 16), "OK");
	okayButton->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	okayButton->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	okayButton->Appearance.BorderInactive = (ui::Colour(200, 200, 200));
	okayButton->SetActionCallback({ [this] {
		SetConnection();
		CloseActiveWindow();
		SelfDestruct();
	} });
	AddComponent(okayButton);
	SetOkayButton(okayButton);

	ui::Button* cancelButton = new ui::Button(ui::Point(Size.X / 2, Size.Y - 16), ui::Point(Size.X / 2 + 1, 16), "Cancel");
	cancelButton->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	cancelButton->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	cancelButton->Appearance.BorderInactive = (ui::Colour(200, 200, 200));
	cancelButton->SetActionCallback({ [this] {
		CloseActiveWindow();
		SelfDestruct();
	} });
	AddComponent(cancelButton);

	ui::Label* fromLabel = new ui::Label(ui::Point(4, 20), ui::Point(50, 15), sim->ElementResolve(fromPart->type, 0));
	fromLabel->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	fromLabel->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	AddComponent(fromLabel);

	ui::Label* toLabel = new ui::Label(ui::Point(64, 20), ui::Point(50, 15), sim->ElementResolve(toPart->type, 0));
	toLabel->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	toLabel->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	AddComponent(toLabel);

	ui::Label* arrowLabel = new ui::Label(ui::Point(45, 20), ui::Point(50, 15), "->");
	arrowLabel->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	arrowLabel->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	AddComponent(arrowLabel);

	statusLabel = new ui::Label(ui::Point(4, 55), ui::Point(Size.X, 15), "");
	statusLabel->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	statusLabel->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	AddComponent(statusLabel);

	outputs = new ui::DropDown(ui::Point(5, 35), ui::Point(55, 16));
	AddComponent(outputs);
	for (int i = 0; i < fromPart->outputs.size(); i++)
		outputs->AddOption(std::pair<String, int>(fromPart->outputs[i], i));

	outputs->SetOption(0);
	outputs->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;

	inputs = new ui::DropDown(ui::Point(65, 35), ui::Point(55, 16));
	AddComponent(inputs);
	for (int i = 0; i < toPart->inputs.size(); i++)
		inputs->AddOption(std::pair<String, int>(toPart->inputs[i].name, i));

	inputs->SetOption(0);
	inputs->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;

	while (!IsNewWire() && inputs->GetOption().second < toPart->inputs.size() - 1)
		inputs->SetOption(toPart->inputs[inputs->GetOption().second + 1].name);

	if (!IsNewWire() && inputs->GetOption().second == toPart->inputs.size() - 1)
		inputs->SetOption(toPart->inputs[0].name);

	FocusComponent(inputs);
	MakeActiveWindow();
}

VideoBuffer* WireTool::GetIcon(int toolID, int width, int height)
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
	newTexture->AddCharacter((width / 2) - 5, (height / 2) - 5, 0xE042, 70, 185, 85, 255);
	return newTexture;
}

void WireWindow::OnDraw()
{
	Graphics* g = GetGraphics();
	g->clearrect(Position.X - 2, Position.Y - 2, Size.X + 3, Size.Y + 3);
	g->drawrect(Position.X, Position.Y, Size.X, Size.Y, 200, 200, 200, 255);

	statusLabel->SetText(GetStatusMsg());
	if (IsNewWire())
		statusLabel->SetTextColour(style::Colour::StatusGood);
	else
		statusLabel->SetTextColour(style::Colour::WarningTitle);
}

void WireWindow::DoDraw()
{
	ui::Window::DoDraw();
}

void WireWindow::DoMouseMove(int x, int y, int dx, int dy)
{
	ui::Window::DoMouseMove(x, y, dx, dy);
}

void WireWindow::DoMouseDown(int x, int y, unsigned button)
{
	ui::Window::DoMouseDown(x, y, button);
}

void WireTool::Connect(Simulation* sim, Particle* fromPart, Particle* toPart)
{
	if (fromPart == NULL || toPart == NULL) {
		new ErrorWindow(toPart == NULL, fromPart == NULL, "", "");
	}
	else if (fromPart->outputs.size() && toPart->inputs.size())
		new WireWindow(this, sim, fromPart, toPart);
	else {
		String inName = sim->ElementResolve(toPart->type, 0);
		String outName = sim->ElementResolve(fromPart->type, 0);
		new ErrorWindow(toPart->inputs.size() == 0, fromPart->outputs.size() == 0, inName, outName);
	}
}

void WireWindow::OnTryExit(ui::Window::ExitMethod method)
{
	CloseActiveWindow();
	SelfDestruct();
}

bool WireWindow::IsNewWire() {
	return toPart->inputs[inputs->GetOption().second].connI == -1;
}

String WireWindow::GetStatusMsg() {
	return IsNewWire() ? String("New Connection") : String("Overwrite Input");
}

void WireWindow::SetConnection() {
	toPart->inputs[inputs->GetOption().second].connX = fromPart->x - toPart->x;
	toPart->inputs[inputs->GetOption().second].connY = fromPart->y - toPart->y;
	toPart->inputs[inputs->GetOption().second].connT = fromPart->type;
	toPart->inputs[inputs->GetOption().second].connI = outputs->GetOption().second;

	if (toPart->type == PT_JOIN || toPart->type == PT_AND || toPart->type == PT_OR || toPart->type == PT_NAND || toPart->type == PT_OR) {
		if (toPart->inputs[toPart->inputs.size() - 1].connI != -1) {
			int size = toPart->inputs.size();
			std::string name = "IN" + std::to_string(size + 1);
			toPart->inputs.push_back(WireIn{ std_string_to_String(name) });
		}
	}
}