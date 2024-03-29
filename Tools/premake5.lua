-- Define Leadwerks Paths
LeadwerksPath = os.getenv("LEADWERKS")
if LeadwerksPath==nil then return end

-- Define Game Directory (Relative to this premake script)
GameDir = path.getabsolute("..")

-- Define Root Source Directory
SourceDir = GameDir .. "/Source"

-- Define Soultion Directory
SoultionDir = GameDir

-- Define tools Directory
ToolsPath = GameDir .."/Tools"

-- Workspace
workspace "LeadwerksTest"
    location "%{SoultionDir}"
    startproject "LeadwerksTest"
    configurations { "Debug", "Release"}

	filter "configurations:*x86"
		architecture "x86"

    project "LeadwerksTest"
        targetname ("UltraTest")
        location "%{SoultionDir}"
        language "C++"
        cppdialect "C++14"
        staticruntime "on"
        characterset("ASCII")
        editandcontinue "off"

        -- Game Location
        targetdir("%{GameDir}")
        debugdir ("%{GameDir}")
    
        -- OBJ Output Location
        objdir ("%{prj.location}/.vs/%{cfg.buildcfg}")
    
        -- Include Directories
        includedirs
        {
            -- Steamworks (TODO: Update path)
            "%{LeadwerksPath}/Include/Libraries/steamworks/public/steam",
            "%{SourceDir}/Patch/Include/Libraries/steamworks/public/steam",

            -- Leadwerks Game Engine
            "%{LeadwerksPath}/Include",
            "%{LeadwerksPath}/Include/Libraries/NewtonDynamics/sdk/dgCore",
            "%{LeadwerksPath}/Include/Libraries/NewtonDynamics/sdk/dgNewton",
            "%{LeadwerksPath}/Include/Libraries/libvorbis/include",
            "%{LeadwerksPath}/Include/Libraries/libogg/include",
            "%{LeadwerksPath}/Include/Libraries/openssl/include",
            "%{LeadwerksPath}/Include/Libraries/VHACD/src/VHACD_Lib/inc",
            "%{LeadwerksPath}/Include/Libraries/glslang",
            "%{LeadwerksPath}/Include/Libraries/freetype-2.4.7/include",
            "%{LeadwerksPath}/Include/Libraries/OpenAL/include",
            "%{LeadwerksPath}/Include/Libraries/NewtonDynamics/sdk/dMath",
            "%{LeadwerksPath}/Include/Libraries/NewtonDynamics/sdk/dgTimeTracker",
            "%{LeadwerksPath}/Include/Libraries/NewtonDynamics/sdk/dContainers",
            "%{LeadwerksPath}/Include/Libraries/NewtonDynamics/sdk/dCustomJoints",
            "%{LeadwerksPath}/Include/Libraries/RecastNavigation/RecastDemo/Include",
            "%{LeadwerksPath}/Include/Libraries/RecastNavigation/DetourCrowd/Include",
            "%{LeadwerksPath}/Include/Libraries/RecastNavigation/DetourTileCache/Include",
            "%{LeadwerksPath}/Include/Libraries/RecastNavigation/DebugUtils/Include",
            "%{LeadwerksPath}/Include/Libraries/RecastNavigation/Recast/Include",
            "%{LeadwerksPath}/Include/Libraries/RecastNavigation/Detour/Include",
            "%{LeadwerksPath}/Include/Libraries/tolua++-1.0.93/include",
            "%{LeadwerksPath}/Include/Libraries/lua-5.1.4",
            "%{LeadwerksPath}/Include/Libraries/glew-1.6.0/include/GL",
            "%{LeadwerksPath}/Include/Libraries/glew-1.6.0/include",
            "%{LeadwerksPath}/Include/Libraries/enet-1.3.1/include",
            "%{LeadwerksPath}/Include/Libraries/zlib-1.2.5",
            "%{LeadwerksPath}/Include/Libraries/freetype-2.4.3/include",
            
            -- Patch
            "%{SourceDir}/Patch",

            -- Local Source Directory
            "%{SourceDir}"
        }


        files
        {
            "%{SourceDir}/**.h",
            "%{SourceDir}/**.hpp",
            "%{SourceDir}/**.cpp",
        }

        -- Global Defines:
        defines
        {
            "__STEAM__",
            "_CUSTOM_JOINTS_STATIC_LIB",
            "FT2_BUILD_LIBRARY",
            "LEADWERKS_3_1",
            "DG_DISABLE_ASSERT",
            "OPENGL",
            "_NEWTON_STATIC_LIB",
            "_STATICLIB"
        }
        
        -- Shared PCH
        pchheader "pch.h"

    ---------------------------------------------------------
    -- Visual Studio Exclusive
    ---------------------------------------------------------
    filter "action:vs*"
        systemversion "latest"
        entrypoint "mainCRTStartup"
        pchsource "%{SourceDir}/pch.cpp"

        files
        {
            "%{SourceDir}/**.ico",
            "%{SourceDir}/**.rc"
        }

        libdirs 
        { 
            -- Steamworks
            --"%{SourceDir}/Patch/Library/Windows/x86",

            -- Leadwerks Game Engine
            "%{LeadwerksPath}/Library/Windows/x86"
        }

        links 
        {
            -- Steamworks
            "steam_api.lib",

            -- Leadwerks Game Engine
            "libcryptoMT.lib",
            "libsslMT.lib",
            "Rpcrt4.lib",
            "crypt32.lib",
            "libcurl.lib",
            "msimg32.lib",
            "lua51.lib",
            "ws2_32.lib",
            "Glu32.lib",
            "libovrd.lib",
            "OpenGL32.lib",
            "winmm.lib",
            "Psapi.lib",
            "OpenAL32.lib",
            "Leadwerks.lib"
        }
        
        defines
        {
            "PSAPI_VERSION=1",
            "PTW32_STATIC_LIB",
            "PTW32_BUILD",
            "_NEWTON_USE_LIB",
            "_LIB",
            "DG_USE_NORMAL_PRIORITY_THREAD",
            "GLEW_STATIC",
            "WINDOWS",
            "WIN32",
            "OS_WINDOWS",
            "PLATFORM_WINDOWS",
            "_WIN_32_VER"
        }
        
        buildoptions
        {	
            "/D \"SLB_LIBRARY\"" 
        }
    
        flags
        {
        --	"NoMinimalRebuild"
            "MultiProcessorCompile"
        }

        linkoptions
        {
            --"/SAFESEH",
            "/NODEFAULTLIB:MSVCRT.lib",
            "/NODEFAULTLIB:MSVCRTD.lib"
        }

        -- Preprocessor
        prebuildcommands
        {
            --"\"%{ToolsPath}/preprocessor.exe\" +path ../"
        }

    filter {"system:windows", "configurations:Debug"}            
        libdirs 
        { 
            "%{LeadwerksPath}/Library/Windows/x86/Debug"
        }   

    filter {"system:windows", "configurations:Release"}
        libdirs 
        { 
            "%{LeadwerksPath}/Library/Windows/x86/Release"
        }    

    filter "configurations:Debug"
         defines
        {
            "DEBUG",
            "_DEBUG"
        }

        links 
        {
            "newton_d.lib",
            "dContainers_d.lib",
            "dCustomJoints_d.lib"
        } 

        kind "ConsoleApp"
        targetsuffix ".debug"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines
        {
            "NDEBUG"
        }

        links 
        {
			"newton.lib",
			"dContainers.lib",
			"dCustomJoints.lib"
        }

        kind "WindowedApp"
        runtime "Release"
        symbols "off"
        optimize "on"
    ---------------------------------------------------------
    ---------------------------------------------------------