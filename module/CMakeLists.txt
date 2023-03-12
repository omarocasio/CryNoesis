if(DEDICATED_SERVER)
	return()
endif()

option(PLUGIN_Noesis "Enable the noesis plugin" ON)
if (NOT PLUGIN_Noesis)
	return()
endif()


set(NS_SDK "${SDK_DIR}/Noesis" CACHE PATH "Noesis sdk path")


#set(NS_LICENSE_NAME "" CACHE STRING "The noesis Licsense Name")
#set(NS_LICENSE_KEY "" CACHE STRING "The noesis Licsense Key")

if(NOT EXISTS ${NS_SDK})
 message("Noesis sdk path not found at: ${NS_SDK}")
 return()
endif()

start_sources()

sources_platform(ALL)
add_sources("Code_uber.cpp"
    PROJECTS CryNoesis
    SOURCE_GROUP "Root"
		"Plugin.cpp"
		"StdAfx.cpp"
		"Plugin.h"
		"StdAfx.h"
)
add_sources("Components_uber.cpp"
    PROJECTS CryNoesis
    SOURCE_GROUP "Components"
		"Components/ComponentFontLoader.cpp"
		"Components/ComponentTextureLoad.cpp"
		"Components/ComponentViewDisplay.cpp"
		"Components/ComponentXamlLoader.cpp"
		"Components/ComponentFontLoader.h"
		"Components/ComponentTextureLoad.h"
		"Components/ComponentViewDisplay.h"
		"Components/ComponentXamlLoader.h"
)
add_sources("Controls_uber.cpp"
    PROJECTS CryNoesis
    SOURCE_GROUP "Controls"
		"Controls/ViewContainer.cpp"
		"Controls/ViewContainer.h"
)
add_sources("Core_uber.cpp"
    PROJECTS CryNoesis
    SOURCE_GROUP "Core"
		"Core/ComponentRegistration.cpp"
		"Core/Implementation.cpp"
		"Core/InputHandler.cpp"
		"Core/ViewManager.cpp"
		"Core/ComponentRegistration.h"
		"Core/Handlers.h"
		"Core/Implementation.h"
		"Core/InputHandler.h"
		"Core/ViewManager.h"
)
add_sources("Interfaces_uber.cpp"
    PROJECTS CryNoesis
    SOURCE_GROUP "Interfaces"
		"Interfaces/INoesis.h"
		"Interfaces/INoesisPlugin.h"
		"Interfaces/IViewManager.h"
		"Interfaces/NoesisSDK.h"
)
add_sources("Providers_uber.cpp"
    PROJECTS CryNoesis
    SOURCE_GROUP "Providers"
		"Providers/FileProvider.cpp"
		"Providers/FontProvider.cpp"
		"Providers/TextureProvider.cpp"
		"Providers/XamlProvider.cpp"
		"Providers/FileProvider.h"
		"Providers/FontProvider.h"
		"Providers/TextureProvider.h"
		"Providers/XamlProvider.h"
)
add_sources("Renderer_uber.cpp"
    PROJECTS CryNoesis
    SOURCE_GROUP "Renderer"
		"Renderer/Renderer.cpp"
		"Renderer/Texture.cpp"
		"Renderer/LayoutDefinitions.h"
		"Renderer/Renderer.h"
		"Renderer/StateDefinitions.h"
		"Renderer/Texture.h"
		"Renderer/ViewRenderData.h"
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CVarOverrides.h")
    add_sources("NoUberFile"
        PROJECTS Game
        SOURCE_GROUP "Root"
            "CVarOverrides.h"
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/EngineDefineOverrides.h")
    add_sources("NoUberFile"
        PROJECTS Game
        SOURCE_GROUP "Root"
            "EngineDefineOverrides.h"
    )
endif()
end_sources()

CryEngineModule(CryNoesis PCH "StdAfx.cpp" SOLUTION_FOLDER "CryPlugins")

target_include_directories(${THIS_PROJECT}
 PRIVATE
  "${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
  "${NS_SDK}/Include"
  "../Include"
 PUBLIC
  ../Interface
)


unset(${NoesisLib} CACHE)
find_library(NoesisLib NAMES Noesis PATHS "${NS_SDK}/Lib/windows_x86_64")
target_link_libraries(${THIS_PROJECT} PRIVATE "${NoesisLib}")

#Enable cpp 17 support
set_target_properties(${THIS_PROJECT} PROPERTIES CXX_STANDARD 17)
#Enable edit and continue support
target_compile_options( ${THIS_PROJECT} PRIVATE /ZI )

#target_compile_definitions(${THIS_PROJECT} PRIVATE NS_LICENSE_KEY=${NS_LICENSE_KEY} NS_LICENSE_NAME=${NS_LICENSE_NAME}) 

deploy_runtime_files(${NS_SDK}/Bin/windows_x86_64/Noesis.dll)

