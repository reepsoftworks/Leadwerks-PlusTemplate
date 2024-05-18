#include "pch.h"
#include "ClientCameraTest.h"
#include "../Classes/ClientCamera.h"

using namespace Leadwerks;
using namespace App;

void ClientCameraTest::Start()
{
	auto camera = static_cast<Camera*>(entity);
	ClientCamera::Initalize(camera);

	// Input
	Input::SetActionSet("InGameControls");
	ClientCamera::SetFreeLook(true);

	lookat = GetEntityValue("lookat");

	Listen(Event::KeyDown, NULL);
}

void ClientCameraTest::UpdateInput()
{
	if (Input::ActionHit("Jump") && lookat != NULL)
	{
		ClientCamera::LookAt(lookat);
	}

	if (Input::ActionHit("ZoomIn"))
	{
		ClientCamera::ZoomIn();
	}

	if (Input::ActionHit("ZoomOut")) ClientCamera::ZoomOut();
	if (Input::ActionHit("QuickSpin")) ClientCamera::QuickSpin();
}