#pragma once
//
#include <emscripten/websocket.h>
#include <stdio.h>
#include <stdlib.h>
//
static eastl::string websocket_staus = "Close";
//
static EM_BOOL WebSocketOpen( int eventType, const EmscriptenWebSocketOpenEvent* e, void* userData )
{
    printf( "open(eventType=%d, userData=%ld)\n", eventType, ( long )userData );

    emscripten_websocket_send_utf8_text( e->socket, "hello on the other side" );

    char data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    emscripten_websocket_send_binary( e->socket, data, sizeof( data ) );

    emscripten_websocket_close( e->socket, 0, 0 );
    return 0;
}
//
static EM_BOOL WebSocketClose( int eventType, const EmscriptenWebSocketCloseEvent* e, void* userData )
{
    printf( "close(eventType=%d, wasClean=%d, code=%d, reason=%s, userData=%ld)\n", eventType, e->wasClean, e->code, e->reason, ( long )userData );
    return 0;
}
//
static EM_BOOL WebSocketError( int eventType, const EmscriptenWebSocketErrorEvent* e, void* userData )
{
    printf( "error(eventType=%d, userData=%ld)\n", eventType, ( long )userData );
    return 0;
}
//
static EM_BOOL WebSocketMessage( int eventType, const EmscriptenWebSocketMessageEvent* e, void* userData )
{
    printf( "message(eventType=%d, userData=%ld, data=%p, numBytes=%d, isText=%d)\n", eventType, ( long )userData, e->data, e->numBytes, e->isText );
    if ( e->isText )
        printf( "text data: \"%s\"\n", e->data );
    else
    {
        printf( "binary data:" );
        for ( int i = 0; i < e->numBytes; ++i )
            printf( " %02X", e->data[ i ] );
        printf( "\n" );

        emscripten_websocket_delete( e->socket );
        exit( 0 );
    }
    return 0;
}
