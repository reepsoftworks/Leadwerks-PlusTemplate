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
    bool hdr = false;

	void Settings::SetMSAA(Setting setting)
	{
        msaa = setting;
		if (ActiveCamera)
		{
            switch (setting)
            {
            case SETTING_ULTRA:
                ActiveCamera->SetMultisampleMode(16);
                break;

            case SETTING_HIGH:
                ActiveCamera->SetMultisampleMode(8);
                break;

            case SETTING_MEDIUM:
                ActiveCamera->SetMultisampleMode(4);
                break;

            case SETTING_LOW:
                ActiveCamera->SetMultisampleMode(2);
                break;

            case SETTING_DISABLED:
                ActiveCamera->SetMultisampleMode(1);
                break;

            default:
                break;
            }
		}
	}

    void Settings::SetLightQuality(Setting setting)
    {
        light = setting;
        if (ActiveCamera)
        {
            switch (setting)
            {
            case SETTING_ULTRA:
            case SETTING_HIGH:
                ActiveCamera->GetWorld()->SetLightQuality(2);
                break;

            case SETTING_MEDIUM:
                ActiveCamera->GetWorld()->SetLightQuality(1);
                break;

            case SETTING_LOW:
            case SETTING_DISABLED:
                ActiveCamera->GetWorld()->SetLightQuality(0);
                break;

            default:
                break;
            }
        }
    }

    static void SetShadowScale(const int size)
    {
        if (ActiveCamera)
        {
            for (const auto& p : ActiveCamera->GetWorld()->entities)
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
        if (ActiveCamera)
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
        if (ActiveCamera)
        {
            switch (setting)
            {
            case SETTING_ULTRA:
            case SETTING_HIGH:
                ActiveCamera->GetWorld()->SetTessellationQuality(2);
                break;

            case SETTING_MEDIUM:
                ActiveCamera->GetWorld()->SetTessellationQuality(1);
                break;

            case SETTING_LOW:
            case SETTING_DISABLED:
                ActiveCamera->GetWorld()->SetTessellationQuality(0);
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
            ActiveCamera->GetWorld()->SetTerrainQuality(2);
            break;

        case SETTING_MEDIUM:
            ActiveCamera->GetWorld()->SetTerrainQuality(1);
            break;

        case SETTING_LOW:
        case SETTING_DISABLED:
            ActiveCamera->GetWorld()->SetTerrainQuality(0);
            break;

        default:
            break;
        }
    }

    void Settings::SetWaterQuality(Setting setting)
    {
        water = setting;
        if (ActiveCamera)
        {
            switch (setting)
            {
            case SETTING_ULTRA:
            case SETTING_HIGH:
                ActiveCamera->GetWorld()->SetWaterQuality(2);
                break;

            case SETTING_MEDIUM:
                ActiveCamera->GetWorld()->SetWaterQuality(1);
                break;

            case SETTING_LOW:
            case SETTING_DISABLED:
                ActiveCamera->GetWorld()->SetWaterQuality(0);
                break;

            default:
                break;
            }
        }
    }

    void Settings::SetHDR(const bool mode)
    {
        hdr = mode;
        if (ActiveCamera) ActiveCamera->SetHDRMode(mode);
    }

    void Settings::Apply(Camera* camera)
    {
        ActiveCamera = camera;
        SetMSAA(msaa);
        SetLightQuality(light);
        SetShadowQuality(shadow);
        SetTessellationQuality(tessellation);
        SetTerrainQuality(terrain);
        SetHDR(hdr);
    }
}