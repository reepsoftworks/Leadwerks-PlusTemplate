#include "pch.h"

using namespace Leadwerks;

void CC_MSAA(std::string pArgV)
{
	if (pArgV.empty())
		return;

	DPrint("Setting msaa to " + pArgV);

	auto setting = (Setting)String::Int(pArgV);
	Settings::SetMSAA(setting);
}
static ConVar r_msaa("r_msaa", "1", CVAR_SAVE, "Usage: r_msaa <Setting (0 = 1x, 1 = 2x, 2 = 4x, 3 = 8x, 4 = 16x)>", CC_MSAA);

void CC_LightQuality(std::string pArgV)
{
	if (pArgV.empty())
		return;

	DPrint("Setting light quality to " + pArgV);

	auto setting = (Setting)String::Int(pArgV);
	Settings::SetLightQuality(setting);
}
static ConVar r_lightquality("r_lightquality", "2", CVAR_SAVE, "Usage: r_lightquality <Setting (0,1 = Low, 2 = Medium, 3 = High)>", CC_LightQuality);

void CC_ShadowQuality(std::string pArgV)
{
	if (pArgV.empty())
		return;

	DPrint("Setting shadow quality to " + pArgV);

	auto setting = (Setting)String::Int(pArgV);
	Settings::SetShadowQuality(setting);
}
static ConVar r_shadowquality("r_shadowquality", "2", CVAR_SAVE, "Usage: r_shadowquality <Setting (0,1 = shadow maps at 256x256 , 2 = shadow maps at 512x512 (Default), 3 = shadow maps at 1024x1024)>", CC_ShadowQuality);

void CC_TessellationQuality(std::string pArgV)
{
	if (pArgV.empty())
		return;

	DPrint("Setting tessellation quality to " + pArgV);

	auto setting = (Setting)String::Int(pArgV);
	Settings::SetTessellationQuality(setting);
}
static ConVar r_tessellationquality("r_tessellationquality", "1", CVAR_SAVE, "Usage: r_tessellationquality <Setting>", CC_TessellationQuality); // Look up good values!!

void CC_TerrainQuality(std::string pArgV)
{
	if (pArgV.empty())
		return;

	DPrint("Setting terrain quality to " + pArgV);

	auto setting = (Setting)String::Int(pArgV);
	Settings::SetTerrainQuality(setting);
}
static ConVar r_terrainquality("r_terrainquality", "2", CVAR_SAVE, "Usage: r_terrainquality <Setting (0,1 = Low, 2 = Medium, 3 = High)>", CC_TerrainQuality);

void CC_WaterQuality(std::string pArgV)
{
	if (pArgV.empty())
		return;

	DPrint("Setting water quality to " + pArgV);

	auto setting = (Setting)String::Int(pArgV);
	Settings::SetWaterQuality(setting);
}
static ConVar r_waterquality("r_waterquality", "2", CVAR_SAVE, "Usage: r_waterquality <Setting (0,1 = Low, 2 = Medium, 3 = High)>", CC_WaterQuality);

void CC_HDRMode(std::string pArgV)
{
	if (pArgV.empty())
		return;

	DPrint("Setting HDR mode to " + pArgV);

	auto setting = (bool)String::Int(pArgV);
	Settings::SetHDR(setting);
}
static ConVar r_hdr("r_hdr", "1", CVAR_SAVE, "Usage: r_hdr <bool>", CC_HDRMode);

void CC_Fov(std::string pArgV)
{
	if (pArgV.empty())
		return;

	DPrint("Setting fov to " + pArgV);

	auto setting = String::Float(pArgV);
	Settings::SetFov(setting);
}
static ConVar fov("fov", "70", CVAR_SAVE, "Usage: fov <float (Default = 70)>", CC_Fov);