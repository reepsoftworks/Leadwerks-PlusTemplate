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
		auto arr = properties["spinspeed"];
		spinspeed = Vec3(arr[0], arr[1], arr[2]);
	}
}

nlohmann::json Spinner::Save()
{
	nlohmann::json properties = nlohmann::json::object();
	properties["enabled"] = enabled;
	properties["spinspeed"] = nlohmann::json::array();
	properties["spinspeed"][0] = spinspeed.x;
	properties["spinspeed"][1] = spinspeed.y;
	properties["spinspeed"][2] = spinspeed.z;
	return properties;
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