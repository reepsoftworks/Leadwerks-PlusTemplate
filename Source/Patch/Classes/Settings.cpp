#include "pch.h"
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
    // TODO: Do proper Get() functions.
    Setting msaa = SETTING_DISABLED;
    Setting light = SETTING_MEDIUM;
    Setting shadow = SETTING_MEDIUM;
    Setting tessellation = SETTING_MEDIUM;
    Setting terrain = SETTING_MEDIUM;
    Setting water = SETTING_MEDIUM;
    bool hdr = true;
    float fov = 70.0f;
    Camera* MainCamera;

	void Settings::SetMSAA(Setting setting)
	{
        msaa = setting;
		if (MainCamera)
		{
            switch (setting)
            {
            case SETTING_ULTRA:
                MainCamera->SetMultisampleMode(16);
                break;

            case SETTING_HIGH:
                MainCamera->SetMultisampleMode(8);
                break;

            case SETTING_MEDIUM:
                MainCamera->SetMultisampleMode(4);
                break;

            case SETTING_LOW:
                MainCamera->SetMultisampleMode(2);
                break;

            case SETTING_DISABLED:
                MainCamera->SetMultisampleMode(1);
                break;

            default:
                break;
            }
		}
	}

    void Settings::SetLightQuality(Setting setting)
    {
        light = setting;
        if (MainCamera)
        {
            switch (setting)
            {
            case SETTING_ULTRA:
            case SETTING_HIGH:
                MainCamera->GetWorld()->SetLightQuality(2);
                break;

            case SETTING_MEDIUM:
                MainCamera->GetWorld()->SetLightQuality(1);
                break;

            case SETTING_LOW:
            case SETTING_DISABLED:
                MainCamera->GetWorld()->SetLightQuality(0);
                break;

            default:
                break;
            }
        }
    }

    static void SetShadowScale(const int size)
    {
        if (MainCamera)
        {
            for (const auto& p : MainCamera->GetWorld()->entities)
            {
                if (p->GetClass() == Object::PointLightClass || p->GetClass() == Object::SpotLightClass)
                {
                    Light* light = CastObject<Light>(p);
                    light->SetShadowMapSize(size);
                }
            }
        }
       
    }

    void Settings::SetShadowQuality(Setting setting)
    {
        shadow = setting;
        if (MainCamera)
        {
            switch (setting)
            {
            case SETTING_ULTRA:
            case SETTING_HIGH:
                SetShadowScale(1024);
                break;

            case SETTING_MEDIUM:
                SetShadowScale(512);
                break;

            case SETTING_LOW:
            case SETTING_DISABLED:
                SetShadowScale(256);
                break;

            default:
                break;
            }
        }
    }

    void Settings::SetTessellationQuality(Setting setting)
    {
        tessellation = setting;
        if (MainCamera)
        {
            switch (setting)
            {
            case SETTING_ULTRA:
            case SETTING_HIGH:
                MainCamera->GetWorld()->SetTessellationQuality(2);
                break;

            case SETTING_MEDIUM:
                MainCamera->GetWorld()->SetTessellationQuality(1);
                break;

            case SETTING_LOW:
            case SETTING_DISABLED:
                MainCamera->GetWorld()->SetTessellationQuality(0);
                break;

            default:
                break;
            }
        }
    }

    void Settings::SetTerrainQuality(Setting setting)
    {
        terrain = setting;
        switch (setting)
        {
        case SETTING_ULTRA:
        case SETTING_HIGH:
            MainCamera->GetWorld()->SetTerrainQuality(2);
            break;

        case SETTING_MEDIUM:
            MainCamera->GetWorld()->SetTerrainQuality(1);
            break;

        case SETTING_LOW:
        case SETTING_DISABLED:
            MainCamera->GetWorld()->SetTerrainQuality(0);
            break;

        default:
            break;
        }
    }

    void Settings::SetWaterQuality(Setting setting)
    {
        water = setting;
        if (MainCamera)
        {
            switch (setting)
            {
            case SETTING_ULTRA:
            case SETTING_HIGH:
                MainCamera->GetWorld()->SetWaterQuality(2);
                break;

            case SETTING_MEDIUM:
                MainCamera->GetWorld()->SetWaterQuality(1);
                break;

            case SETTING_LOW:
            case SETTING_DISABLED:
                MainCamera->GetWorld()->SetWaterQuality(0);
                break;

            default:
                break;
            }
        }
    }

    void Settings::SetHDR(const bool mode)
    {
        hdr = mode;
        if (MainCamera) MainCamera->SetHDRMode(mode);
    }

    void Settings::SetFov(const float fovvalue)
    {
        fov = fovvalue;
        if (MainCamera) MainCamera->SetFOV(fov);
    }

    void Settings::Apply(Camera* camera)
    {
        if (camera != MainCamera) MainCamera = camera;
        SetMSAA(msaa);
        SetLightQuality(light);
        SetShadowQuality(shadow);
        SetTessellationQuality(tessellation);
        SetTerrainQuality(terrain);
        SetHDR(hdr);
        SetFov(fov);
    }
}