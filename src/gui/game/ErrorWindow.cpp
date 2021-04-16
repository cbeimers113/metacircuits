#include "ErrorWindow.h"

ErrorWindow::ErrorWindow(bool noInputs_, bool noOutputs_, String inName_, String outName_) :
	ui::Window(ui::Point(-1, -1), ui::Point(120, 75)),
	noInputs(noInputs_),
	noOutputs(noOutputs_),
	inName(inName_),
	outName(outName_)
{
	ui::Label* messageLabel = new ui::Label(ui::Point(4, 5), ui::Point(Size.X - 8, 15), "Connection Error");
	messageLabel->SetTextColour(style::Colour::InformationTitle);
	messageLabel->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	messageLabel->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	AddComponent(messageLabel);

	if (noInputs) {
		String msg = inName + " has no inputs!";
		if (inName.empty())
			msg = "No input selected!";
		ui::Label* inErrLabel = new ui::Label(ui::Point(4, 15), ui::Point(Size.X - 8, 15), msg);
		inErrLabel->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
		inErrLabel->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
		AddComponent(inErrLabel);
	}

	if (noOutputs) {
		String msg = outName + " has no outputs!";
		if (outName.empty())
			msg = "No output selected!";
		ui::Label* outErrLabel = new ui::Label(ui::Point(4, 25), ui::Point(Size.X - 8, 15), msg);
		outErrLabel->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
		outErrLabel->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
		AddComponent(outErrLabel);
	}

	ui::Button* okayButton = new ui::Button(ui::Point(0, Size.Y - 16), ui::Point(Size.X, 16), "OK");
	okayButton->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	okayButton->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	okayButton->Appearance.BorderInactive = (ui::Colour(200, 200, 200));
	okayButton->SetActionCallback({ [this] {
		CloseActiveWindow();
		SelfDestruct();
	} });
	AddComponent(okayButton);
	SetOkayButton(okayButton);

	FocusComponent(okayButton);
	MakeActiveWindow();
}

void ErrorWindow::OnDraw()
{
	Graphics* g = GetGraphics();

	g->clearrect(Position.X - 2, Position.Y - 2, Size.X + 3, Size.Y + 3);
	g->drawrect(Position.X, Position.Y, Size.X, Size.Y, 200, 200, 200, 255);
}

void ErrorWindow::DoDraw()
{
	ui::Window::DoDraw();
}

void ErrorWindow::DoMouseMove(int x, int y, int dx, int dy)
{
	ui::Window::DoMouseMove(x, y, dx, dy);
}

void ErrorWindow::DoMouseDown(int x, int y, unsigned button)
{
	ui::Window::DoMouseDown(x, y, button);
}

void ErrorWindow::OnTryExit(ui::Window::ExitMethod method)
{
	CloseActiveWindow();
	SelfDestruct();
}