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
    //
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
    void CreateScene();
    void SetupViewport();
    void CreateLog();
    void CreateSocket( eastl::string url );
    void setup_style_of_imgui();
    void RenderUi();
    void WebsocketUi();
    void AxesNodeAttributeUi();
    void ChartUi();
    void BigCharUi( std::string str_title, float* befor_x, float* befor_y, float* befor_z, int befor_count, float* after_x, float* after_y, float* after_z, int after_count );
    void BigCharUiChange();
    //
    void ToCtrlAxesNode();
    void DrawPoints();
public:
    void HandleMouseDown( StringHash eventType, VariantMap& eventData );
    void HandleKeyDown( StringHash /*eventType*/, VariantMap& eventData );
    void HandlePostRenderUpdate( StringHash eventType, VariantMap& eventData );
private:
    void v2a();
    //
    float roll[ 1024 ], pitch[ 1024 ], yaw[ 1024 ], magx[ 1024 ], magy[ 1024 ], magz[ 1024 ], gyrx[ 1024 ], gyry[ 1024 ], gyrz[ 1024 ], accx[ 1024 ], accy[ 1024 ], accz[ 1024 ], eax[ 1024 ], eay[ 1024 ], eaz[ 1024 ], evx[ 1024 ], evy[ 1024 ], evz[ 1024 ], px[ 1024 ], py[ 1024 ], pz[ 1024 ];
    float original_eax[ 1024 ], original_eay[ 1024 ], original_eaz[ 1024 ], original_evx[ 1024 ], original_evy[ 1024 ], original_evz[ 1024 ], original_px[ 1024 ], original_py[ 1024 ], original_pz[ 1024 ];
};
