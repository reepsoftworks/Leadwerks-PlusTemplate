#include "pch.h"
#include "FirstPersonControls.h"

using namespace Leadwerks;
using namespace App;

extern ConVar mousesmoothing;
extern ConVar mousesensitivity;
extern ConVar mouseinvert;
extern ConVar fov;

void FirstPersonControls::PauseCallback(Object* source, Object* extra)
{
	auto player = CastObject<FirstPersonControls>(source);
	if (!player) return;

	auto ui = SampleUI::currentui;
	if (!ui) return;

	if (Scene::GetCurrent()->GetPauseState())
	{
		ui->ShowMenuPanel(true);
		player->freelookstarted = false;
	}
	else
	{
		ui->HideMenuPanel();
		Input::SetActionSet("InGameControls");
	}
}

void FirstPersonControls::Start()
{
	// Entity Setup
	GetEntity()->SetPhysicsMode(Entity::CharacterPhysics);
	if (GetEntity()->GetMass() == 0.0f) GetEntity()->SetMass(78);
	GetEntity()->SetCollisionType(COLLISION_CHARACTER);
	GetEntity()->SetShadowMode(0);

	SetCallback(CALLBACK_PAUSE, this, FirstPersonControls::PauseCallback);

	// Camera Setup
	if (MainCamera == NULL) MainCamera = Camera::Create();
	Vec3 pos = GetEntity()->GetPosition();
	pos.y += eyeheight;
	MainCamera->SetPosition(pos);
	MainCamera->SetFOV(fov.GetFloat());
	topangle = -90.0f;
	bottomangle = 90.0f;
	freelookstarted = false;

	// Events
	Listen(Event::WidgetOpen, NULL);
	Listen(Event::WidgetClose, NULL);

	// Input
	Input::SetActionSet("InGameControls");
	currentcameraposition = MainCamera->GetPosition(true);
	freelookrotation = GetEntity()->GetRotation(true);

	UpdateMatrix();
}

/*
float Mix(const float start, const float stop, const float amount)
{
	// Ensure that amount is in the range [0, 1]
	float clampedAmount = Math::Clamp(amount, 0.0f, 1.0f);

	// Linear interpolation formula
	return start + clampedAmount * (stop - start);
}
*/

void FirstPersonControls::HandleMouseLook()
{
	if (lockmouse == true) return;
	if (MainCamera && Input::GetActionSet() == "InGameControls")
	{
		Vec3 movement;
		float jump = 0;
		bool crouch = false;
		auto window = App::GraphicsWindow::GetCurrent();
		if (window)
		{
			auto cx = Math::Round(window->GetFramebuffer()->GetWidth() / 2);
			auto cy = Math::Round(window->GetFramebuffer()->GetHeight() / 2);
			auto mpos = window->GetMousePosition();
			window->SetMousePosition(cx, cy);
			auto centerpos = window->GetMousePosition();

			if (freelookstarted)
			{
				float looksmoothing = mousesmoothing.GetFloat(); //0.5f;
				float lookspeed = mousesensitivity.GetFloat() / 10.0f;

				if (looksmoothing > 0.00f)
				{
					mpos.x = mpos.x * looksmoothing + freelookmousepos.x * (1 - looksmoothing);
					mpos.y = mpos.y * looksmoothing + freelookmousepos.y * (1 - looksmoothing);
				}

				auto dx = (mpos.x - centerpos.x) * lookspeed;
				auto dy = (mpos.y - centerpos.y) * lookspeed;

				if (mouseinvert.GetBool()) 
					freelookrotation.x = freelookrotation.x - dy;
				else
					freelookrotation.x = freelookrotation.x + dy;
				freelookrotation.x = Math::Clamp(freelookrotation.x, topangle, bottomangle);
				freelookrotation.y = freelookrotation.y + dx;
				MainCamera->SetRotation(freelookrotation, true);
				freelookmousepos = Vec3(mpos.x, mpos.y);
			}
			else
			{
				freelookstarted = true;
				freelookrotation = MainCamera->GetRotation(true);
				freelookmousepos = Vec3(window->GetMousePosition().x, window->GetMousePosition().y);
			}
		}
	}
}

Leadwerks::Vec2 FirstPersonControls::GetMovementAxis()
{
	return Input::ActionsToAxis("MoveForward", "MoveBackward", "MoveLeft", "MoveRight");
}

void FirstPersonControls::HandleMovement()
{
	if (GetEntity()->GetPhysicsMode() == Entity::RigidBodyPhysics)
		return;

	float jump = 0;
	Vec3 eyeangles = MainCamera->GetRotation();
	Vec2 movement = GetMovementAxis();
	float speed = movespeed;

	if (Input::ActionHit("Jump"))
	{
		jump = jumpforce;
		movement.x *= jumplunge;
		if (movement.y != 0) movement.y = movement.y * jumplunge;
	}

	auto l = movement.Length();
	if (l > 0.0)
	{
		movement.x = (movement.x / l) * speed;
		movement.y = (movement.y / l) * speed;
	}

	GetEntity()->SetInput(eyeangles.y, movement.y, movement.x, jump, false, 1.0f, 0.5f, true);
}

void FirstPersonControls::UpdateWorld()
{
	HandleMouseLook();
}

void FirstPersonControls::UpdatePhysics()
{
	HandleMovement();
}

void FirstPersonControls::UpdateMatrix()
{
	Vec3 pos = GetEntity()->GetPosition();
	pos.y += eyeheight;
	MainCamera->SetPosition(pos);
}

void FirstPersonControls::Load(nlohmann::json properties)
{
	if (properties == NULL)
	{
		lockmouse = false;
		eyeheight = GetFloatValue("eyeheight");
		movespeed = GetFloatValue("movespeed");
		jumpforce = GetFloatValue("jumpforce");
		jumplunge = GetFloatValue("jumplunge");
	}
	else
	{
		// Not implemented...
	}
}

void FirstPersonControls::Save(nlohmann::json properties)
{
}

bool FirstPersonControls::ProcessEvent(const Leadwerks::Event& e) 
{
	if (e.id == Event::WidgetOpen)
	{
		// If a gadget is opened panel is shown, enforce the global action set.
		Input::SetActionSet("");
		freelookstarted = false;
	}
	else if (e.id == Event::WidgetClose)
	{
		//Input::SetActionSet("InGameControls");
	}
	else if (e.id == Event::KeyDown)
	{
		if (Input::ActionHit("Pause"))
		{
			Scene::GetCurrent()->SetPauseState(!Scene::GetCurrent()->GetPauseState());
		}
	}

	return true;
}