#include "pch.h"
#include "FirstPersonControls.h"
#include "../Classes/ClientCamera.h"

using namespace Leadwerks;
using namespace App;

ConVar crosshair("crosshair", "1", CVAR_SAVE, "Usage: crosshair <bool>");
ConVar player_speed("player_speed", "4.2");
ConVar player_jumpforce("player_jumpforce", "5.2");
ConVar player_jumplunge("player_jumplunge", "2.5");
ConVar player_maxaccel("player_maxaccel", "0.15");
ConVar player_maxdecel("player_maxdecel", "0.15");

void FirstPersonControls::PauseCallback(Object* source, Object* extra)
{
	auto player = CastObject<FirstPersonControls>(source);
	if (!player) return;

	auto ui = SampleUI::currentui;
	if (!ui) return;

	if (Scene::GetCurrent()->GetPauseState())
	{
		ui->ShowMenuPanel(true);
		ClientCamera::ResetLook();
	}
	else
	{
		ui->HideMenuPanel();
		Input::SetActionSet("InGameControls");
		ClientCamera::SetFreeLook(true);
	}
}

void FirstPersonControls::Start()
{
	// Entity Setup
	GetEntity()->SetPhysicsMode(Entity::CharacterPhysics);
	if (GetEntity()->GetMass() == 0.0f) GetEntity()->SetMass(10);
	GetEntity()->SetCollisionType(Collision::Character);
	GetEntity()->SetShadowMode(0);
	SetCallback(CALLBACK_PAUSE, this, FirstPersonControls::PauseCallback);
	crouchingstate = false;

	// Camera Setup
	ClientCamera::Initalize();
	ClientCamera::SetFreeLook(true);

	// Events
	Listen(Event::WidgetOpen, NULL);
	Listen(Event::WidgetClose, NULL);
	Listen(Event::KeyDown, NULL);

	// Input
	Input::SetActionSet("InGameControls");

	UpdateMatrix();
}

void FirstPersonControls::PostRender(Context* context)
{
	if (VR::Enabled())
		return;

	if (crosshair.GetBool())
	{
		context->SetColor(1, 1, 1, 1);

		// This draws 5 dots in the center of the screen. 
		iVec2 center = iVec2((context->GetWidth() / 2) - 1, context->GetHeight() / 2); // This isn't 100% centered, but that's ok.
		context->Plot(center.x, center.y);
		context->Plot(center.x, center.y + 8);
		context->Plot(center.x, center.y - 8);
		context->Plot(center.x, center.y);
		context->Plot(center.x + 8, center.y);
		context->Plot(center.x - 8, center.y);

		context->SetColor(1, 1, 1, 1);
	}
}

float Mix(const float start, const float stop, const float amount)
{
	// Ensure that amount is in the range [0, 1]
	//float clampedAmount = Math::Clamp(amount, 0.0f, 1.0f);

	// Linear interpolation formula
	return start + amount * (stop - start);
}

Leadwerks::Vec2 FirstPersonControls::GetMovementAxis()
{
	return Input::ActionsToAxis("MoveForward", "MoveBackward", "MoveLeft", "MoveRight");
}

void FirstPersonControls::HandleCrouching()
{
	if (crouchingstate)
	{
		if (currentheight != croucheyeheight && !GetEntity()->GetAirborne())
		{
			currentheight = Math::Inc(croucheyeheight, currentheight, 0.05f * Time::GetSpeed());
			UpdateMatrix();
		}
	}
	else
	{
		if (currentheight != eyeheight && !GetEntity()->GetAirborne())
		{
			currentheight = Math::Inc(eyeheight, currentheight, 0.05f * Time::GetSpeed());
			UpdateMatrix();
		}
	}
}

void FirstPersonControls::HandleMovement()
{
	if (GetEntity()->GetPhysicsMode() == Entity::RigidBodyPhysics)
		return;

	float jump = 0;
	Vec3 eyeangles = MainCamera->GetRotation();
	Vec2 movement = GetMovementAxis();
	float speed = player_speed.GetFloat();
	
	if (Input::ActionHit("Jump") && !GetEntity()->GetAirborne())
	{
		jump = player_jumpforce.GetFloat();
		movement.x *= player_jumplunge.GetFloat();
		if (movement.y != 0) movement.y = movement.y * player_jumplunge.GetFloat();
	}

	bool crouched = false;
	bool cannot_standup = false;
	if (Input::ActionDown("Crouch") || jump > 0) crouched = true;

	if (crouched)
	{
		if (currentheight != croucheyeheight && !GetEntity()->GetAirborne())
		{
			crouchingstate = true;
		}

		PickInfo pick;
		auto p0 = GetEntity()->GetPosition();
		auto p1 = Transform::Point(0, GetEntity()->charactercontroller->height, 0, GetEntity(), NULL);
		cannot_standup = GetEntity()->GetWorld()->Pick(p0, p1, pick, GetEntity()->charactercontroller->radius, true, Collision::Character);

		speed = speed / 2;
	}
	else
	{
		if (currentheight != eyeheight && !GetEntity()->GetAirborne())
		{
			PickInfo pick;
			auto p0 = GetEntity()->GetPosition();
			auto p1 = Transform::Point(0, GetEntity()->charactercontroller->height, 0, GetEntity(), NULL);
			cannot_standup = GetEntity()->GetWorld()->Pick(p0, p1, pick, GetEntity()->charactercontroller->radius, true, Collision::Character);

			if (!cannot_standup)
			{
				crouchingstate = false;
			}
		}
	}

	movement.x = movement.x * speed;
	movement.y = movement.y * speed;
	if (movement.x != 0.0f and movement.y != 0.0f) movement *= 0.707f;
	GetEntity()->SetInput(eyeangles.y, movement.y, movement.x, jump, crouched || cannot_standup, player_maxaccel.GetFloat(), player_maxdecel.GetFloat(), true);
}

void FirstPersonControls::UpdateWorld()
{
	HandleCrouching();
}

void FirstPersonControls::UpdatePhysics()
{
	HandleMovement();
}

void FirstPersonControls::UpdateMatrix()
{
	Vec3 pos = GetEntity()->GetPosition();
	pos.y += currentheight;
	MainCamera->SetPosition(pos);
}

void FirstPersonControls::UpdateInput()
{
	if (Input::ActionHit("ZoomIn") && allowzooming) ClientCamera::ZoomIn();
	if (Input::ActionHit("ZoomOut") && allowzooming) ClientCamera::ZoomOut();
	if (Input::ActionHit("QuickSpin") && allowspinning) ClientCamera::QuickSpin();
	if (Input::ActionHit("Pause")) Scene::GetCurrent()->SetPauseState(!Scene::GetCurrent()->GetPauseState());
}

void FirstPersonControls::Load(nlohmann::json properties)
{
	if (properties == NULL)
	{
		eyeheight = GetFloatValue("eyeheight");
		currentheight = eyeheight;
	}
	else
	{
		// Not implemented...
	}
}

nlohmann::json FirstPersonControls::Save()
{
	return NULL;
}

bool FirstPersonControls::ProcessEvent(const Leadwerks::Event& e) 
{
	if (e.id == Event::WidgetOpen)
	{
		// If a gadget is opened, simply pause the game to prevent things getting complicated. ><
		Scene::GetCurrent()->SetPauseState(true);

	}

	return true;
}