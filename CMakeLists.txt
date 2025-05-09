cmake_minimum_required(VERSION 3.14)
set(app_name ahrs.web)
project(${app_name})

# C++17 is required for this project.
set(CMAKE_CXX_STANDARD 17)

# Setup output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY $ENV{FmDev}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $ENV{FmDev}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{FmDev}/bin)

#
# link flag
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS}
    $ENV{fm_links_exe_flags}
)

# 设置全局的 include 路径
include_directories($ENV{FmDev}/source)
include_directories($ENV{FmDev}/libs/core/include)
include_directories($ENV{FmDev}/libs/core/include/Urho3D/ThirdParty)
include_directories($ENV{FmDev}/libs/core/include/Urho3D/ThirdParty/tracy)
include_directories($ENV{FmDev}/libs/core/include/Urho3D/ThirdParty/SDL)
include_directories($ENV{FmDev}/libs/core/include/Urho3D/ThirdParty/ImGui)
include_directories($ENV{FmDev}/libs/core/include/Urho3D/ThirdParty/ImGui/misc/cpp)

# 设置链接路径
link_directories($ENV{FmDev}/libs/core/lib)

#
add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS)
add_definitions(-DIMGUI_USE_WCHAR32)
add_definitions(-DURHO3D_SYSTEMUI)
add_definitions(-DURHO3D_RMLUI)
add_definitions(-DURHO3D_THREADING)
add_definitions(-DEASTL_DEBUG)
add_definitions(-DURHO3D_DEBUG)

#
# set(CMAKE_EXECUTABLE_SUFFIX ".html")

#
# include($ENV{FmDev}/libs/core/share/CMake/Urho3D.cmake)
file(GLOB app_src
    source/*.cpp
    source/*.cxx
    source/component/*.cpp
    source/imguiDemo/*.cpp
    source/implot/*.cpp

    source/implot3d/*.cpp
)

# 生成
add_executable(${app_name}

    ${app_src}
)

#
# Link the engine and plugins.
target_link_libraries(${app_name}
    libBox2D.a
    libBullet.a
    libdatachannel-wasm.a
    libDetour.a
    libDetourCrowd.a
    libDetourTileCache.a
    libDiligent-BasicPlatform.a
    libDiligent-Common.a
    libDiligent-EmscriptenPlatform.a
    libDiligent-GraphicsAccessories.a
    libDiligent-GraphicsEngine.a
    libDiligent-GraphicsEngineOpenGL-static.a
    libDiligent-HLSL2GLSLConverterLib.a
    libDiligent-Primitives.a
    libDiligent-ShaderTools.a
    libEASTL.a
    libenkiTS.a
    libETCPACK.a
    libfmt.a
    libFreeType.a
    libGenericCodeGen.a
    libGLEW.a
    libglslang-default-resource-limits.a
    libglslang.a
    libImGui.a
    libLZ4.a
    libMachineIndependent.a
    libnativefiledialog.a
    libOGLCompiler.a
    libOSDependent.a
    libPugiXml.a
    libRecast.a
    libRmlUi.a
    libSDL2.a
    libspirv-cross-core.a
    libspirv-cross-glsl.a
    libspirv-cross-hlsl.a
    libspirv-cross-msl.a
    libSPIRV-Tools-opt.a
    libSPIRV-Tools.a
    libSPIRV.a
    libStanHull.a
    libSTB.a
    libtinygltf.a
    libUrho3D.a
    libWebP.a
    libzlibstatic.a
)
