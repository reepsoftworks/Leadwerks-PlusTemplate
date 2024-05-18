#include "pch.h"
#include "ClientCamera.h"

using namespace App;
using namespace Leadwerks;

static bool freelookstarted = false;
static Vec3 freelookmousepos = Vec3(0);
static Vec3 freelookrotation = Vec3(0);
static Vec2 lookchange = Vec2(0);
static Vec3 currentcameraposition = Vec3(0);
static bool freelook = false;
static bool lockmouse = false;
static float topangle = -90;
static float bottomangle = 90;
static PickInfo* lineofsight = NULL;
static Entity* lastentitylookedat = NULL;
static Entity* targetent = NULL;
static float lookrate = 0.25f;
static float zoom = 0;
static float spin = 0;

ConVar mousesmoothing("mousesmoothing", "0", CVAR_SAVE, "Usage: mousesmoothing <float>");
ConVar mousesensitivity("mousesensitivity", "1.0", CVAR_SAVE, "Usage: mousesensitivity <float>");
ConVar mouseinvert("mouseinvert", "0", CVAR_SAVE, "Usage: mouseinvert <bool>");
extern ConVar fov;

static float UserFOV()
{
	return fov.GetFloat();
}

static void CameraUpdateHook(Entity* entity)
{
	Camera* camera = static_cast<Camera*>(entity);

	// Update Line of sight
	if (lineofsight != NULL)
	{
		auto p0 = camera->GetPosition(true);
		auto p1 = Transform::Point(0, 0, 1024, camera, NULL);
		auto radius = 0.15f;
		bool result = camera->GetWorld()->Pick(p0, p1, *lineofsight, radius, true, Collision::Prop); // TODO: This needs it's own Collision type!

		if (result)
		{
			if (lastentitylookedat != lineofsight->entity)
			{
				//if (lastentitylookedat) lastentitylookedat->SetSelected(false);
				//lineofsight->entity->SetSelected(true);
				lastentitylookedat = lineofsight->entity;
			}
		}
		else
		{
			//if (lastentitylookedat) lastentitylookedat->SetSelected(false);
			lastentitylookedat = NULL;
		}
	}

	// If targetent isn't NULL, force the camera to look at the entity.
	if (targetent != NULL)
	{
		if (targetent != lastentitylookedat)
		{
			freelookstarted = false;
			lockmouse = true;
			MainCamera->Point(targetent, 2, lookrate * Time::GetSpeed(), 0.0f);
		}
		else
		{
			freelookstarted = false;
			//Print("Done looking at " + targetent->GetKeyValue("name"));
			lockmouse = false;
			targetent = NULL;

			// Fix Z.
			MainCamera->SetRotation(MainCamera->GetRotation().x, MainCamera->GetRotation().y, 0.0f);
			Print(MainCamera->GetRotation().ToString());
		}
	}

	// Handle zooming
	if (zoom != 0)
	{
		if (MainCamera->GetFOV() != zoom)
		{
			auto v = Math::Inc(zoom, MainCamera->GetFOV(), 4.0f * Time::GetSpeed());
			MainCamera->SetFOV(v);
		}
		else 
		{
			zoom = 0;
		}
	}

	// Spinning
	if (spin != 0)
	{
		auto rot = MainCamera->GetRotation();
		if (Math::Round(rot.y) != spin)
		{
			lockmouse = true;
			auto v = Math::Inc(spin, MainCamera->GetRotation().y, 8.0f * Time::GetSpeed());
			MainCamera->SetRotation(rot.x, v, rot.z);
		}
		else
		{
			freelookstarted = false;
			lockmouse = false;
			spin = 0;
		}
	}

	if (lockmouse == true) return;
	if (camera && freelook)
	{
		if (Input::GetDeviceID() == 0)
		{
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
					if (camera->GetFOV() < 40.0f) lookspeed = lookspeed / 4;

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
					camera->SetRotation(freelookrotation, true);
					freelookmousepos = Vec3(mpos.x, mpos.y);
				}
				else
				{
					freelookstarted = true;
					freelookrotation = camera->GetRotation(true);
					freelookmousepos = Vec3(window->GetMousePosition().x, window->GetMousePosition().y);
				}
			}
		}
		else
		{
			 // TODO: Controller look!
		}
	}
}

static void CameraUpdateMatrixHook(Entity* entity)
{
	//TODO: Update Sound engine..
}

static void CameraDeleteHook(Entity* entity)
{
	Print("CameraDeleteHook");
	lastentitylookedat = NULL;
	FreeObject(lineofsight);
}

void ClientCamera::Initalize(Leadwerks::Camera* camera)
{
	freelookstarted = false;
	freelookmousepos = Vec3(0);
	freelookrotation = Vec3(0);
	lookchange = Vec2(0);
	currentcameraposition = Vec3(0);

	if (camera == NULL)
	{
		MainCamera = Camera::Create();
	}
	else if (MainCamera != camera) MainCamera = camera;

	freelookrotation = MainCamera->GetRotation(true);

	MainCamera->SetFOV(UserFOV());
	MainCamera->AddHook(Entity::UpdateWorldHook, (void*)CameraUpdateHook);
	MainCamera->AddHook(Entity::UpdateMatrixHook, (void*)CameraUpdateMatrixHook);
	MainCamera->AddHook(Object::DeleteHook, (void*)CameraDeleteHook);

	FreeObject(lineofsight);
	lineofsight = new PickInfo();
}

Leadwerks::Camera* ClientCamera::GetEntity()
{
	return MainCamera;
}

void ClientCamera::SetFreeLook(const bool b)
{
	freelookstarted = false;
	freelookmousepos = Vec3(0);
	freelookrotation = Vec3(0);
	lookchange = Vec2(0);
	currentcameraposition = Vec3(0);
	if (MainCamera) freelookrotation = MainCamera->GetRotation(true);

	freelook = b;
}

void ClientCamera::ResetLook()
{
	freelookstarted = false;
}

void ClientCamera::LookAt(Leadwerks::Entity* entity, const float rate)
{
	if (lastentitylookedat != entity)
	{
		targetent = entity;
		lookrate = rate;
		//Print("Looking at " + targetent->GetKeyValue("name"));
	}
}

Leadwerks::Entity* ClientCamera::GetTargetEntity()
{
	return lastentitylookedat;
}

void ClientCamera::ZoomIn()
{
	zoom = UserFOV() / 4;
}

void ClientCamera::ZoomOut()
{
	zoom = UserFOV();
}

void ClientCamera::QuickSpin()
{
	if (MainCamera) spin = Math::Round(MainCamera->GetRotation().y + 180.0f);
}