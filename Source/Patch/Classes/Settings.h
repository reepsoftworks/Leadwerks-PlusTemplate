#pragma once
#include "LeadwerksPlus.h"

namespace Leadwerks
{
	class Settings : public Object
	{
	public:
		static void SetMSAA(Setting setting);
		static void SetLightQuality(Setting setting);
		static void SetShadowQuality(Setting setting);
		static void SetTessellationQuality(Setting setting);
		static void SetTerrainQuality(Setting setting);
		static void SetWaterQuality(Setting setting);
		static void SetHDR(const bool mode);
		static void SetFov(const float fovvalue);

		static void Apply(Camera* camera);
	};

	extern Camera* MainCamera;
}