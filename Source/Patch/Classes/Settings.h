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

		static Setting GetMSAA();
		static Setting GetLightQuality();
		static Setting GetShadowQuality();
		static Setting GetTessellationQuality();
		static Setting GetTerrainQuality();
		static Setting GetWaterQuality();
		static bool GetHDR();
		static float GetFov();

		static void Apply(Camera* camera);
		static std::string GetSettingString(Setting setting);
	};

	extern Camera* MainCamera;
}