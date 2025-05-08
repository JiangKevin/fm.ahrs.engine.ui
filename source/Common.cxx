#include "CommonApplication.h"
#include <Urho3D/Engine/EngineDefs.h>
#include <cstring>
//

int RunApplication()
{
    Urho3D::SharedPtr< Urho3D::Context >   context( new Urho3D::Context() );
    Urho3D::SharedPtr< CommonApplication > application( new CommonApplication( context ) );
    //
    return application->Run();
}
//
int main( int argc, char** argv )
{
    Urho3D::ParseArguments( argc, argv );
    return RunApplication();
}