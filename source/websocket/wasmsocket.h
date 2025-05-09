#pragma once
//
#include "queue/sensor_db.h"
#include <boost/lockfree/queue.hpp>
#include <emscripten/websocket.h>
#include <iostream>
#include <mutex>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <vector>
//
// struct WASM_SOCKET_DATA
// {
//     eastl::string                     websocket_staus           = "\xf3\xb1\x98\x96";
//     eastl::string                     websocket_receive_message = "";
//     LockFreeMessageQueue< SENSOR_DB > sensor_data_queue;
// };
// 全局的变量
static eastl::string websocket_staus                    = "\xf3\xb1\x98\x96";
static eastl::string websocket_receive_message          = "";
static eastl::string websocket_receive_message_original = "";

//
static std::queue< SENSOR_DB >  sensor_data_queue;
static std::vector< SENSOR_DB > sensor_data_vector;
static std::mutex               queue_mutex;
static int64_t                  start_time;
static int                      Microsecond = 1000000;
//

static EM_BOOL WebSocketOpen( int eventType, const EmscriptenWebSocketOpenEvent* e, void* userData )
{
    // printf( "open(eventType=%d, userData=%ld)\n", eventType, ( long )userData );
    // struct WASM_SOCKET_DATA* p_userData = ( struct WASM_SOCKET_DATA* )userData;
    websocket_staus = "\xf3\xb0\x8c\x98";

    // printf( "open(eventType=%d, userData=%s)\n", eventType, p_userData->websocket_staus.c_str() );

    // emscripten_websocket_send_utf8_text( e->socket, "hello on the other side" );

    // char data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    // emscripten_websocket_send_binary( e->socket, data, sizeof( data ) );

    // emscripten_websocket_close( e->socket, 0, 0 );
    return 0;
}
//
static EM_BOOL WebSocketClose( int eventType, const EmscriptenWebSocketCloseEvent* e, void* userData )
{
    // printf( "close(eventType=%d, wasClean=%d, code=%d, reason=%s, userData=%ld)\n", eventType, e->wasClean, e->code, e->reason, ( long )userData );
    websocket_staus = "\xf3\xb1\x98\x96";
    return 0;
}
//
static EM_BOOL WebSocketError( int eventType, const EmscriptenWebSocketErrorEvent* e, void* userData )
{
    // printf( "error(eventType=%d, userData=%ld)\n", eventType, ( long )userData );
    websocket_staus = "\xf3\xb1\x98\x96";
    return 0;
}
//
static EM_BOOL WebSocketMessage( int eventType, const EmscriptenWebSocketMessageEvent* e, void* userData )
{
    // struct WASM_SOCKET_DATA* p_userData = ( struct WASM_SOCKET_DATA* )userData;

    // printf( "message(eventType=%d, userData=%ld, data=%p, numBytes=%d, isText=%d)\n", eventType, ( long )userData, e->data, e->numBytes, e->isText );
    //
    if ( e->isText )
    {
        // printf( "text data: \"%s\"\n", e->data );
        websocket_receive_message_original = ( char* )e->data;
        //
        if ( ( websocket_receive_message_original != "Stoped" ) && ( websocket_receive_message_original != "Connected" ) )
        {
            std::lock_guard< std::mutex > lock( queue_mutex );
            //
            SENSOR_DB new_sensor_db;
            new_sensor_db.getValueFromString( websocket_receive_message_original.c_str() );
            //
            sensor_data_queue.push( new_sensor_db );
            // 1s存一个
            int64_t cur_time = getMicrosecondTimestamp();
            if ( ( cur_time - start_time ) > Microsecond * 5 )
            {
                if ( sensor_data_vector.size() < 1024 )
                {
                    sensor_data_vector.push_back( new_sensor_db );
                }
                else
                {
                    sensor_data_vector.erase( sensor_data_vector.begin() );
                    sensor_data_vector.push_back( new_sensor_db );
                }
            }
            //
            websocket_receive_message = new_sensor_db.to_info().c_str();
        }
        else
        {
            websocket_receive_message = websocket_receive_message_original;
        }
    }
    else
    {
        printf( "binary data:" );
        for ( int i = 0; i < e->numBytes; ++i )
        {
            printf( " %02X", e->data[ i ] );
        }
        printf( "\n" );
        //
        emscripten_websocket_delete( e->socket );
        exit( 0 );
    }
    return 0;
}
