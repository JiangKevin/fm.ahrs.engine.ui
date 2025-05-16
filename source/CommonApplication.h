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
    bool                   is_axes_node_enble_ = true;
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
    void BigCharUi( float* befor_x, float* befor_y, float* befor_z, int befor_count, float* after_x, float* after_y, float* after_z, int after_count );
    void BigCharUiChange();
    //
    void ToCtrlAxesNode();
    void DrawPoints();
public:
    void HandleMouseDown( StringHash eventType, VariantMap& eventData );
    void HandleKeyDown( StringHash /*eventType*/, VariantMap& eventData );
    void HandlePostRenderUpdate( StringHash eventType, VariantMap& eventData );
private:
    bool is_show_big_char  = true;
    bool is_show_char      = false;
    bool is_show_websocket = true;
    bool is_show_3d_char   = true;
};
