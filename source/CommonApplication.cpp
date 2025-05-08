
#include "CommonApplication.h"
#include "component/FmFreeFlyController.h"
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/DebugNew.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Navigation/CrowdAgent.h>
#include <Urho3D/Navigation/DynamicNavigationMesh.h>
#include <Urho3D/Navigation/Navigable.h>
#include <Urho3D/Navigation/NavigationEvents.h>
#include <Urho3D/Navigation/Obstacle.h>
#include <Urho3D/Navigation/OffMeshConnection.h>
#include <Urho3D/RenderPipeline/ShaderConsts.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>
#include <emscripten.h>
//
//
EM_JS( int, get_canvas_w, (), { return document.getElementById( "canvas" ).offsetWidth; } );
EM_JS( int, get_canvas_h, (), { return document.getElementById( "canvas" ).offsetHeight; } );
//
CommonApplication::CommonApplication( Context* context ) : Application( context )
{
    //
}
//
void CommonApplication::Setup()
{
    engineParameters_[ EP_WINDOW_TITLE ]          = "Basic";
    engineParameters_[ EP_APPLICATION_NAME ]      = "Built-in Basic";
    engineParameters_[ EP_LOG_NAME ]              = "conf://basic.log";
    engineParameters_[ EP_BORDERLESS ]            = false;
    engineParameters_[ EP_HEADLESS ]              = false;
    engineParameters_[ EP_SOUND ]                 = true;
    engineParameters_[ EP_RESOURCE_PATHS ]        = "CoreData;Data;EditorData;ProjectData;UserData;IndexedDB;tmp;";
    engineParameters_[ EP_ORIENTATIONS ]          = "LandscapeLeft LandscapeRight Portrait";
    engineParameters_[ EP_WINDOW_RESIZABLE ]      = true;
    engineParameters_[ EP_RESOURCE_PREFIX_PATHS ] = ";..;../..;/;IndexedDB";
    engineParameters_[ EP_AUTOLOAD_PATHS ]        = "Autoload;";
    //
    scene_          = new Scene( context_ );
    mainCameraNode_ = new Node( context_ );
}
//
void CommonApplication::Start()
{
    // setup view
    auto  graphics = GetSubsystem< Graphics >();
    auto* renderer = GetSubsystem< Renderer >();
    //
    int winSizeX_ = get_canvas_w();
    int winSizeY_ = get_canvas_h();
    graphics->SetMode( winSizeX_, winSizeY_ );
    //
    FmRegisterOjbj();
    //
    CreateScene();
    //
    SetupViewport();
    CreateLog();
    //
    CreateSocket();
    //
    auto* input = context_->GetSubsystem< Input >();
    // Subscribe key down event
    SubscribeToEvent( input, E_KEYDOWN, URHO3D_HANDLER( CommonApplication, HandleKeyDown ) );
}
void CommonApplication::Stop()
{
    //
}

void CommonApplication::Update( float timeStep )
{
    // Do nothing for now, could be extended to eg. animate the display
}
void CommonApplication::HandleMouseDown( StringHash eventType, VariantMap& eventData ){
    //
};
void CommonApplication::SetupViewport()
{
    auto* renderer = GetSubsystem< Renderer >();

    // Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    SharedPtr< Viewport > viewport( new Viewport( context_, scene_, mainCameraNode_->GetComponent< Camera >() ) );
    renderer->SetViewport( 0, viewport );
}
//
void CommonApplication::CreateScene()
{
    auto* cache = GetSubsystem< ResourceCache >();
    //
    scene_->CreateComponent< Octree >();
    scene_->CreateComponent< DebugRenderer >();

    // Create scene node & StaticModel component for showing a static plane
    Node* planeNode = scene_->CreateChild( "Plane" );
    planeNode->SetScale( Vector3( 1000.0f, 1.0f, 1000.0f ) );
    auto* planeObject = planeNode->CreateComponent< StaticModel >();
    planeObject->SetModel( cache->GetResource< Model >( "Models/Plane.mdl" ) );
    planeObject->SetMaterial( cache->GetResource< Material >( "Materials/StoneTiled.xml" ) );

    // Create a Zone component for ambient lighting & fog control
    Node* zoneNode = scene_->CreateChild( "Zone" );
    auto* zone     = zoneNode->CreateComponent< Zone >();
    zone->SetBoundingBox( BoundingBox( -10000.0f, 10000.0f ) );
    zone->SetAmbientColor( Color( 0.15f, 0.15f, 0.15f ) );
    zone->SetFogColor( Color( 0.5f, 0.5f, 0.7f ) );
    zone->SetFogStart( 100.0f );
    zone->SetFogEnd( 300.0f );

    // Create a directional light to the world. Enable cascaded shadows on it
    Node* lightNode = scene_->CreateChild( "DirectionalLight" );
    lightNode->SetDirection( Vector3( 0.6f, -1.0f, 0.8f ) );
    auto* light = lightNode->CreateComponent< Light >();
    light->SetLightType( LIGHT_DIRECTIONAL );
    light->SetCastShadows( true );
    light->SetShadowBias( BiasParameters( 0.00025f, 0.5f ) );
    // Set cascade splits at 10, 50 and 200 world units, fade shadows out at 80% of maximum shadow distance
    light->SetShadowCascade( CascadeParameters( 10.0f, 50.0f, 200.0f, 0.0f, 0.8f ) );

    // Create randomly sized boxes. If boxes are big enough, make them occluders
    Node* boxGroup = scene_->CreateChild( "Boxes" );
    for ( unsigned i = 0; i < 20; ++i )
    {
        Node* boxNode = boxGroup->CreateChild( "Box" );
        float size    = 1.0f + Random( 10.0f );
        boxNode->SetPosition( Vector3( Random( 80.0f ) - 40.0f, size * 0.5f, Random( 80.0f ) - 40.0f ) );
        boxNode->SetScale( size );
        auto* boxObject = boxNode->CreateComponent< StaticModel >();
        boxObject->SetModel( cache->GetResource< Model >( "Models/Box.mdl" ) );
        boxObject->SetMaterial( cache->GetResource< Material >( "Materials/Stone.xml" ) );
        boxObject->SetCastShadows( true );
        if ( size >= 3.0f )
            boxObject->SetOccluder( true );
    }
    //
    axes_node_ = scene_->CreateChild( "Axes" );
    axes_node_->SetPosition( Vector3( 0.0f, 10.0f, 0.0f ) );
    axes_node_->SetScale( Vector3( 0.1f, 0.1f, 0.1f ) );
    auto* axes_obj_ = axes_node_->CreateComponent< StaticModel >();
    axes_obj_->SetModel( cache->GetResource< Model >( "Models/axes.mdl" ) );
    axes_obj_->SetMaterial( 0, cache->GetResource< Material >( "Materials/white.xml" ) );
    axes_obj_->SetMaterial( 1, cache->GetResource< Material >( "Materials/red.xml" ) );
    axes_obj_->SetMaterial( 2, cache->GetResource< Material >( "Materials/green.xml" ) );
    axes_obj_->SetMaterial( 3, cache->GetResource< Material >( "Materials/blue.xml" ) );
    axes_obj_->SetMaterial( 4, cache->GetResource< Material >( "Materials/red.xml" ) );
    axes_obj_->SetMaterial( 5, cache->GetResource< Material >( "Materials/red.xml" ) );
    axes_obj_->SetMaterial( 6, cache->GetResource< Material >( "Materials/blue.xml" ) );
    axes_obj_->SetMaterial( 7, cache->GetResource< Material >( "Materials/blue.xml" ) );
    axes_obj_->SetMaterial( 8, cache->GetResource< Material >( "Materials/green.xml" ) );
    axes_obj_->SetMaterial( 9, cache->GetResource< Material >( "Materials/green.xml" ) );
    // axes_obj_->ApplyMaterialList();

    axes_obj_->SetCastShadows( true );
    // Create a DynamicNavigationMesh component to the scene root
    auto* navMesh = scene_->CreateComponent< DynamicNavigationMesh >();
    // Set small tiles to show navigation mesh streaming
    navMesh->SetTileSize( 32 );
    // Enable drawing debug geometry for obstacles and off-mesh connections
    navMesh->SetDrawObstacles( true );
    navMesh->SetDrawOffMeshConnections( true );
    // Set the agent height large enough to exclude the layers under boxes
    navMesh->SetAgentHeight( 10.0f );
    // Set nav mesh cell height to minimum (allows agents to be grounded)
    navMesh->SetCellHeight( 0.05f );
    // Create a Navigable component to the scene root. This tags all of the geometry in the scene as being part of the
    // navigation mesh. By default this is recursive, but the recursion could be turned off from Navigable
    scene_->CreateComponent< Navigable >();
    // Add padding to the navigation mesh in Y-direction so that we can add objects on top of the tallest boxes
    // in the scene and still update the mesh correctly
    navMesh->SetPadding( Vector3( 0.0f, 10.0f, 0.0f ) );
    // Now build the navigation geometry. This will take some time. Note that the navigation mesh will prefer to use
    // physics geometry from the scene nodes, as it often is simpler, but if it can not find any (like in this example)
    // it will use renderable geometry instead
    navMesh->Rebuild();

    // Create a CrowdManager component to the scene root
    auto*                        crowdManager = scene_->CreateComponent< CrowdManager >();
    CrowdObstacleAvoidanceParams params       = crowdManager->GetObstacleAvoidanceParams( 0 );
    // Set the params to "High (66)" setting
    params.velBias       = 0.5f;
    params.adaptiveDivs  = 7;
    params.adaptiveRings = 3;
    params.adaptiveDepth = 3;
    crowdManager->SetObstacleAvoidanceParams( 0, params );

    // Create some movable barrels. We create them as crowd agents, as for moving entities it is less expensive and more convenient than using obstacles

    // Create the camera. Set far clip to match the fog. Note: now we actually create the camera node outside the scene, because
    // we want it to be unaffected by scene load / save

    auto* camera = mainCameraNode_->CreateComponent< Camera >();
    camera->SetFarClip( 300.0f );

    // Set an initial position for the camera scene node above the plane and looking down
    mainCameraNode_->SetPosition( Vector3( 0.0f, 50.0f, 0.0f ) );
    mainCameraNode_->SetRotation( Quaternion( 0.0f, 0.0f, 0.0f ) );
    mainCameraNode_->CreateComponent< FmFreeFlyController >();
};
//
void CommonApplication::CreateLog()
{
    UI*            ui          = GetSubsystem< UI >();  // Get logo texture
    ResourceCache* cache       = GetSubsystem< ResourceCache >();
    Texture2D*     logoTexture = cache->GetResource< Texture2D >( "Textures/logo_outlined.png" );
    if ( ! logoTexture )
    {
        return;
    }
    /// Logo sprite.
    auto logoSprite = ui->GetRoot()->CreateChild< Sprite >();
    // Set logo sprite texture
    logoSprite->SetTexture( logoTexture );
    int textureWidth  = logoTexture->GetWidth();
    int textureHeight = logoTexture->GetHeight();
    // Set logo sprite scale
    logoSprite->SetScale( 170.0f / textureWidth );
    // Set logo sprite size
    logoSprite->SetSize( textureWidth, textureHeight );
    // Set logo sprite hot spot
    logoSprite->SetHotSpot( textureWidth, textureHeight );
    // Set logo sprite alignment
    logoSprite->SetAlignment( HA_RIGHT, VA_BOTTOM );
    logoSprite->SetPosition( -10, -10 );
    // Make logo not fully opaque to show the scene underneath
    logoSprite->SetOpacity( 0.4f );
    // Set a low priority for the logo so that other UI elements can be drawn on top
    logoSprite->SetPriority( -100 );
}
///
void CommonApplication::FmRegisterOjbj()
{
    FmFreeFlyController::RegisterObject( context_ );
}
//
void CommonApplication::HandleKeyDown( StringHash /*eventType*/, VariantMap& eventData )
{
    using namespace KeyDown;
    int key = eventData[ P_KEY ].GetInt();
    //
    if ( key == KEY_F2 )
    {
        context_->GetSubsystem< Engine >()->CreateDebugHud()->ToggleAll();
        return;
    }
}
//
void CommonApplication::CreateSocket()
{
    if ( ! emscripten_websocket_is_supported() )
    {
        printf( "WebSockets are not supported, cannot continue!\n" );
    }

    EmscriptenWebSocketCreateAttributes attr;
    emscripten_websocket_init_create_attributes( &attr );

    const char* url = "ws://192.168.254.116:18080/";
    attr.url        = url;
    attr.protocols  = "binary,base64";  // We don't really use a special protocol on the server backend in this test, but check that it can be passed.

    EMSCRIPTEN_WEBSOCKET_T socket = emscripten_websocket_new( &attr );
    if ( socket <= 0 )
    {
        printf( "WebSocket creation failed, error code %d!\n", ( EMSCRIPTEN_RESULT )socket );
    }
    //
    emscripten_websocket_set_onopen_callback( socket, ( void* )42, WebSocketOpen );
    emscripten_websocket_set_onclose_callback( socket, ( void* )43, WebSocketClose );
    emscripten_websocket_set_onerror_callback( socket, ( void* )44, WebSocketError );
    emscripten_websocket_set_onmessage_callback( socket, ( void* )45, WebSocketMessage );
};