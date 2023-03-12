#pragma once

#include <CryCore/Project/CryModuleDefs.h>
#define eCryModule eCryM_EnginePlugin
#define GAME_API   DLL_EXPORT

#include <CryCore/Platform/platform.h>
#include <CrySystem/ISystem.h>
#include <CryNetwork/ISerialize.h>

#pragma warning( push )
#pragma warning( disable : 4244)
#pragma warning( disable : 4266)
#include <CrySystem/Profilers/ICryProfilingSystem.h>
#include <Cry3DEngine/I3DEngine.h>
#pragma  pop

#include <NsApp/InteractivityApi.h>
#include <NsApp/MediaElementApi.h>

#define CHAR8_T_DEFINED 1
using char8_t = char;

#include <NoesisPCH.h>

#define NS_APP_INTERACTIVITY_API NS_DLL_IMPORT
#define NS_APP_MEDIAELEMENT_API NS_DLL_IMPORT