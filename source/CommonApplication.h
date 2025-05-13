#pragma once
//

#include <Urho3D/Engine/Application.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/SystemUI/Console.h>
#include <Urho3D/UI/Cursor.h>
#if URHO3D_SYSTEMUI
    #include <Urho3D/SystemUI/DebugHud.h>
#endif

#include "websocket/wasmsocket.h"
#include <Urho3D/Core/Profiler.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Engine/StateManager.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/UI.h>
#include <emscripten/websocket.h>
//
using namespace Urho3D;
//
class CommonApplication : public Application
{
    URHO3D_OBJECT( CommonApplication, Application );
public:
    /// Construct.
    explicit CommonApplication( Context* context );
    //
    void Setup() override;
    void Start() override;
    void Stop() override;
    void Update( StringHash eventType, VariantMap& eventData );
    void FmRegisterOjbj();
private:
    float GyrMisalignment_1[ 3 ]    = { 1.0f, 0.0f, 0.0f };
    float GyrMisalignment_2[ 3 ]    = { 0.0f, 1.0f, 0.0f };
    float GyrMisalignment_3[ 3 ]    = { 0.0f, 0.0f, 1.0f };
    float GyroscopeSensitivity[ 3 ] = { 1.0f, 1.0f, 1.0f };
    float GyroscopeOffset[ 3 ]      = { 0.0f, 0.0f, 0.0f };
    //
    float AccelerometerMisalignment_1[ 3 ] = { 1.0f, 0.0f, 0.0f };
    float AccelerometerMisalignment_2[ 3 ] = { 0.0f, 1.0f, 0.0f };
    float AccelerometerMisalignment_3[ 3 ] = { 0.0f, 0.0f, 1.0f };
    float AccelerometerSensitivity[ 3 ]    = { 1.0f, 1.0f, 1.0f };
    float AccelerometerOffset[ 3 ]         = { 0.0f, 0.0f, 0.025f };
    //
    float SoftIronMatrix_1[ 3 ] = { 1.0f, 0.0f, 0.0f };
    float SoftIronMatrix_2[ 3 ] = { 0.0f, 1.0f, 0.0f };
    float SoftIronMatrix_3[ 3 ] = { 0.0f, 0.0f, 1.0f };
    float HardIronOffset[ 3 ]   = { 0.0f, 0.0f, 0.0f };
    //
    int   ahrs_convention            = 0;
    float ahrs_gain                  = 0.5f;
    float ahrs_gyroscopeRange        = 2000.0f;
    float ahrs_accelerationRejection = 10.0f;
    float ahrs_magneticRejection     = 10.0f;
    int   ahrs_recoveryTriggerPeriod = 500;
public:
    /// Scene.
    SharedPtr< Scene > scene_;
    /// Camera scene node.
    SharedPtr< Node > mainCameraNode_;
    //
    Node*                  axes_node_;
    int                    winSizeX_;
    int                    winSizeY_;
    EMSCRIPTEN_WEBSOCKET_T socket;
public:
    void        CreateScene();
    void        SetupViewport();
    void        CreateLog();
    void        CreateSocket( eastl::string url );
    void        setup_style_of_imgui();
    void        RenderUi();
    void        WebsocketUi();
    void        AxesNodeAttributeUi();
    void        ChartUi();
    std::string GetConfigString();
    void        interpretConfig( std::string content_str );
    //
    void ToCtrlAxesNode();
    void DrawPoints();
public:
    void HandleMouseDown( StringHash eventType, VariantMap& eventData );
    void HandleKeyDown( StringHash /*eventType*/, VariantMap& eventData );
    void HandlePostRenderUpdate( StringHash eventType, VariantMap& eventData );
};
