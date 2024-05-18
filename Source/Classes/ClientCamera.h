#include "pch.h"
#include "../SampleApp.h"

class ClientCamera
{
public:
	static void Initalize(Leadwerks::Camera* camera = NULL);
	static Leadwerks::Camera* GetEntity();
	static void SetFreeLook(const bool b);
	static void ResetLook();
	static void LookAt(Leadwerks::Entity* entity, const float rate = 0.25f);
	static Leadwerks::Entity* GetTargetEntity();
	static void ZoomIn();
	static void ZoomOut();
	static void QuickSpin();
};