#include "pch.h"
#include "Spinner.h"

using namespace Leadwerks;

void Spinner::Start()
{
	Print("Spinner::Start()");
}

void Spinner::UpdateWorld()
{
	if (enabled)
	{
		auto delta = Time::GetSpeed();
		GetEntity()->Turn(spinspeed.x * delta, spinspeed.y * delta, spinspeed.z * delta);
		OnTurn();
	}
}

void Spinner::Load(nlohmann::json properties)
{
	Print("Spinner::Load()");
	if (properties == NULL)
	{
		GetEntity()->SetShadowMode(Light::Dynamic);
		enabled = GetBoolValue("enabled");
		spinspeed = *GetVec3Value("spinspeed");
	}
	else
	{
		// Not implemented...
		enabled = properties["enabled"].get<bool>();
		auto arr = properties["spinspeed"] = nlohmann::json::array();
		spinspeed = Vec3(arr[0], arr[1], arr[2]);
	}
}

exposed_input Spinner::Enable(Entity* sender)
{
	enabled = true;
}

exposed_input Spinner::Disable(Entity* sender)
{
	enabled = false;
}

exposed_output Spinner::OnTurn()
{
	FireOutput("OnTurn");
}

void Spinner::ReceiveSignal(const std::string& inputname, Entity* sender)
{
	ProcessInputSignal(Enable, sender);
}