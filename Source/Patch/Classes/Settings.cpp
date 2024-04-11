#include "pch.h"
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
    // TODO: Do proper Get() functions.
    Setting msaa = SETTING_LOW;
    Setting light = SETTING_MEDIUM;
    Setting shadow = SETTING_MEDIUM;
    Setting tessellation = SETTING_MEDIUM;
    Setting terrain = SETTING_MEDIUM;
    Setting water = SETTING_MEDIUM;
    bool hdr = true;
    float fov = 70.0f;
    Camera* MainCamera;

    Setting Settings::GetMSAA()
    {
        return msaa;
    }

    Setting Settings::GetLightQuality()
    {
        return light;
    }

    Setting Settings::GetShadowQuality()
    {
        return shadow;
    }

    Setting Settings::GetTessellationQuality()
    {
        return tessellation;
    }

    Setting Settings::GetTerrainQuality()
    {
        return terrain;
    }

    Setting Settings::GetWaterQuality()
    {
        return water;
    }

    bool Settings::GetHDR()
    {
        return hdr;
    }

    float Settings::GetFov()
    {
        return fov;
    }

	void Settings::SetMSAA(Setting setting)
	{
        msaa = setting;
		if (MainCamera)
		{
            switch (setting)
            {
            //case SETTING_ULTRA:
                //MainCamera->SetMultisampleMode(16);
                break;

            case SETTING_HIGH:
                MainCamera->SetMultisampleMode(4);
                break;

            case SETTING_MEDIUM:
                MainCamera->SetMultisampleMode(2);
                break;

            case SETTING_LOW:
                MainCamera->SetMultisampleMode(1);
                break;

            //case SETTING_DISABLED:
                //MainCamera->SetMultisampleMode(1);
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
            //case SETTING_ULTRA:
            case SETTING_HIGH:
                MainCamera->GetWorld()->SetLightQuality(2);
                break;

            case SETTING_MEDIUM: 
                MainCamera->GetWorld()->SetLightQuality(1);
                break;

            case SETTING_LOW:
            //case SETTING_DISABLED:
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

                    if (light)
                    {
                        // Ensure the static lights remain static...
                        if (light->GetShadowMode() == Light::Static)
                        {
                            light->FreezeChanges = true;
                            light->InvalidateShadowMap(Light::Static);
                        }

                        auto lq = MainCamera->GetWorld()->GetLightQuality();
                        if (lq > 0)
                        {
                            MainCamera->GetWorld()->SetLightQuality(0);
                            light->SetShadowMapSize(size);
                            MainCamera->GetWorld()->SetLightQuality(lq);
                        }
                    }                    
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
            case SETTING_HIGH:
                SetShadowScale(1024);
                break;

            case SETTING_MEDIUM:
                SetShadowScale(512);
                break;

            case SETTING_LOW:
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
            case SETTING_HIGH:
                MainCamera->GetWorld()->SetTessellationQuality(2);
                break;

            case SETTING_MEDIUM:
                MainCamera->GetWorld()->SetTessellationQuality(1);
                break;

            case SETTING_LOW:
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
        if (MainCamera)
        {
            switch (setting)
            {
            case SETTING_HIGH:
                MainCamera->GetWorld()->SetTerrainQuality(2);
                break;

            case SETTING_MEDIUM:
                MainCamera->GetWorld()->SetTerrainQuality(1);
                break;

            case SETTING_LOW:
                MainCamera->GetWorld()->SetTerrainQuality(0);
                break;

            default:
                break;
            }
        }
    }

    void Settings::SetWaterQuality(Setting setting)
    {
        water = setting;
        if (MainCamera)
        {
            switch (setting)
            {
            case SETTING_HIGH:
                MainCamera->GetWorld()->SetWaterQuality(2);
                break;

            case SETTING_MEDIUM:
                MainCamera->GetWorld()->SetWaterQuality(1);
                break;

            case SETTING_LOW:
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
        MainCamera = camera;
        SetMSAA(msaa);
        SetLightQuality(light);
        SetShadowQuality(shadow);
        SetTessellationQuality(tessellation);
        SetTerrainQuality(terrain);
        SetWaterQuality(water);
        SetHDR(hdr);
        SetFov(fov);
    }

    std::string Settings::GetSettingString(Setting setting)
    {
        std::string s = "";
        switch (setting)
        {
        case SETTING_HIGH:
            s = "High";
            break;

        case SETTING_MEDIUM:
            s = "Medium";
            break;

        case SETTING_LOW:
            s = "Low";
            break;

        default:
            break;
        }

        return s;
    }
}