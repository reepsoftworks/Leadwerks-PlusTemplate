#include "pch.h"
#include "BackgroundMenu.h"

using namespace Leadwerks;

void BackgroundMenu::Start()
{
	alpha = 1.0f;
	auto ui = SampleUI::currentui;
	if (ui) ui->ShowMenuPanel(false); // Set the argument to false so we still see "New Game" and the right commands fire.
}

void BackgroundMenu::PostRender(Leadwerks::Framebuffer* framebuffer)
{
	if (alpha > 0.0f)
	{
		float speed = GetAppSpeed();
		alpha = Math::Inc(0.0f, alpha, speed * (float)fadeoutspeed);
		if (framebuffer->GetBlendMode() != Blend::Alpha) framebuffer->SetBlendMode(Blend::Alpha);
		framebuffer->SetColor(1.0f, 1.0f, 1.0f, alpha);
		auto scene = App::Scene::GetCurrent();
		if (scene)framebuffer->DrawImage(scene->GetLoadingBackground(), 0, 0, framebuffer->GetWidth(), framebuffer->GetHeight());
		framebuffer->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
}