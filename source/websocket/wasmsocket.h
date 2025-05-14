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
static std::vector< SENSOR_DB > original_sensor_data_vector;
static std::mutex               queue_mutex;
static int64_t                  start_time;
static int                      Microsecond = 1000000;
static int                      item_count  = 1024;
//
static float GyrMisalignment_1[ 3 ]    = { 1.0f, 0.0f, 0.0f };
static float GyrMisalignment_2[ 3 ]    = { 0.0f, 1.0f, 0.0f };
static float GyrMisalignment_3[ 3 ]    = { 0.0f, 0.0f, 1.0f };
static float GyroscopeSensitivity[ 3 ] = { 1.0f, 1.0f, 1.0f };
static float GyroscopeOffset[ 3 ]      = { 0.0f, 0.0f, 0.0f };
//
static float AccelerometerMisalignment_1[ 3 ] = { 1.0f, 0.0f, 0.0f };
static float AccelerometerMisalignment_2[ 3 ] = { 0.0f, 1.0f, 0.0f };
static float AccelerometerMisalignment_3[ 3 ] = { 0.0f, 0.0f, 1.0f };
static float AccelerometerSensitivity[ 3 ]    = { 1.0f, 1.0f, 1.0f };
static float AccelerometerOffset[ 3 ]         = { 0.0f, 0.0f, 0.025f };
//
static float SoftIronMatrix_1[ 3 ] = { 1.0f, 0.0f, 0.0f };
static float SoftIronMatrix_2[ 3 ] = { 0.0f, 1.0f, 0.0f };
static float SoftIronMatrix_3[ 3 ] = { 0.0f, 0.0f, 1.0f };
static float HardIronOffset[ 3 ]   = { 0.0f, 0.0f, 0.0f };
//
static int   ahrs_convention            = 0;
static float ahrs_gain                  = 0.5f;
static float ahrs_gyroscopeRange        = 2000.0f;
static float ahrs_accelerationRejection = 10.0f;
static float ahrs_magneticRejection     = 10.0f;
static int   ahrs_recoveryTriggerPeriod = 500;
//
//
static float roll[ 1024 ], pitch[ 1024 ], yaw[ 1024 ], magx[ 1024 ], magy[ 1024 ], magz[ 1024 ], gyrx[ 1024 ], gyry[ 1024 ], gyrz[ 1024 ], accx[ 1024 ], accy[ 1024 ], accz[ 1024 ], eax[ 1024 ], eay[ 1024 ], eaz[ 1024 ], evx[ 1024 ], evy[ 1024 ], evz[ 1024 ], px[ 1024 ], py[ 1024 ], pz[ 1024 ];
static float original_eax[ 1024 ], original_eay[ 1024 ], original_eaz[ 1024 ], original_evx[ 1024 ], original_evy[ 1024 ], original_evz[ 1024 ], original_px[ 1024 ], original_py[ 1024 ], original_pz[ 1024 ];
//
//
static std::string GetConfigString()
{
    std::string content_str = "Setup";
    content_str += "," + transaction_to_string( GyrMisalignment_1[ 0 ] );
    content_str += "," + transaction_to_string( GyrMisalignment_1[ 1 ] );
    content_str += "," + transaction_to_string( GyrMisalignment_1[ 2 ] );
    content_str += "," + transaction_to_string( GyrMisalignment_2[ 0 ] );
    content_str += "," + transaction_to_string( GyrMisalignment_2[ 1 ] );
    content_str += "," + transaction_to_string( GyrMisalignment_2[ 2 ] );
    content_str += "," + transaction_to_string( GyrMisalignment_3[ 0 ] );
    content_str += "," + transaction_to_string( GyrMisalignment_3[ 1 ] );
    content_str += "," + transaction_to_string( GyrMisalignment_3[ 2 ] );
    content_str += "," + transaction_to_string( GyroscopeSensitivity[ 0 ] );
    content_str += "," + transaction_to_string( GyroscopeSensitivity[ 1 ] );
    content_str += "," + transaction_to_string( GyroscopeSensitivity[ 2 ] );
    content_str += "," + transaction_to_string( GyroscopeOffset[ 0 ] );
    content_str += "," + transaction_to_string( GyroscopeOffset[ 1 ] );
    content_str += "," + transaction_to_string( GyroscopeOffset[ 2 ] );
    //
    content_str += "," + transaction_to_string( AccelerometerMisalignment_1[ 0 ] );
    content_str += "," + transaction_to_string( AccelerometerMisalignment_1[ 1 ] );
    content_str += "," + transaction_to_string( AccelerometerMisalignment_1[ 2 ] );
    content_str += "," + transaction_to_string( AccelerometerMisalignment_2[ 0 ] );
    content_str += "," + transaction_to_string( AccelerometerMisalignment_2[ 1 ] );
    content_str += "," + transaction_to_string( AccelerometerMisalignment_2[ 2 ] );
    content_str += "," + transaction_to_string( AccelerometerMisalignment_3[ 0 ] );
    content_str += "," + transaction_to_string( AccelerometerMisalignment_3[ 1 ] );
    content_str += "," + transaction_to_string( AccelerometerMisalignment_3[ 2 ] );
    content_str += "," + transaction_to_string( AccelerometerSensitivity[ 0 ] );
    content_str += "," + transaction_to_string( AccelerometerSensitivity[ 1 ] );
    content_str += "," + transaction_to_string( AccelerometerSensitivity[ 2 ] );
    content_str += "," + transaction_to_string( AccelerometerOffset[ 0 ] );
    content_str += "," + transaction_to_string( AccelerometerOffset[ 1 ] );
    content_str += "," + transaction_to_string( AccelerometerOffset[ 2 ] );
    //
    content_str += "," + transaction_to_string( SoftIronMatrix_1[ 0 ] );
    content_str += "," + transaction_to_string( SoftIronMatrix_1[ 1 ] );
    content_str += "," + transaction_to_string( SoftIronMatrix_1[ 2 ] );
    content_str += "," + transaction_to_string( SoftIronMatrix_2[ 0 ] );
    content_str += "," + transaction_to_string( SoftIronMatrix_2[ 1 ] );
    content_str += "," + transaction_to_string( SoftIronMatrix_2[ 2 ] );
    content_str += "," + transaction_to_string( SoftIronMatrix_3[ 0 ] );
    content_str += "," + transaction_to_string( SoftIronMatrix_3[ 1 ] );
    content_str += "," + transaction_to_string( SoftIronMatrix_3[ 2 ] );
    content_str += "," + transaction_to_string( HardIronOffset[ 0 ] );
    content_str += "," + transaction_to_string( HardIronOffset[ 1 ] );
    content_str += "," + transaction_to_string( HardIronOffset[ 2 ] );
    //
    content_str += "," + int_transaction_to_string( ahrs_convention );
    content_str += "," + transaction_to_string( ahrs_gain );
    content_str += "," + transaction_to_string( ahrs_gyroscopeRange );
    content_str += "," + transaction_to_string( ahrs_accelerationRejection );
    content_str += "," + transaction_to_string( ahrs_magneticRejection );
    content_str += "," + int_transaction_to_string( ahrs_recoveryTriggerPeriod );
    //
    return content_str;
}
//
static void interpretConfig( std::string content_str )
{
    //
    char delimiter = ',';
    auto values    = splitString( content_str, delimiter );
    //
    if ( values.size() == 49 )
    {
        //
        GyrMisalignment_1[ 0 ]    = std::stof( values[ 1 ] );
        GyrMisalignment_1[ 1 ]    = std::stof( values[ 2 ] );
        GyrMisalignment_1[ 2 ]    = std::stof( values[ 3 ] );
        GyrMisalignment_2[ 0 ]    = std::stof( values[ 4 ] );
        GyrMisalignment_2[ 1 ]    = std::stof( values[ 5 ] );
        GyrMisalignment_2[ 2 ]    = std::stof( values[ 6 ] );
        GyrMisalignment_3[ 0 ]    = std::stof( values[ 7 ] );
        GyrMisalignment_3[ 1 ]    = std::stof( values[ 8 ] );
        GyrMisalignment_3[ 2 ]    = std::stof( values[ 9 ] );
        GyroscopeSensitivity[ 0 ] = std::stof( values[ 10 ] );
        GyroscopeSensitivity[ 1 ] = std::stof( values[ 11 ] );
        GyroscopeSensitivity[ 2 ] = std::stof( values[ 12 ] );
        GyroscopeOffset[ 0 ]      = std::stof( values[ 13 ] );
        GyroscopeOffset[ 1 ]      = std::stof( values[ 14 ] );
        GyroscopeOffset[ 2 ]      = std::stof( values[ 15 ] );
        //
        AccelerometerMisalignment_1[ 0 ] = std::stof( values[ 16 ] );
        AccelerometerMisalignment_1[ 1 ] = std::stof( values[ 17 ] );
        AccelerometerMisalignment_1[ 2 ] = std::stof( values[ 18 ] );
        AccelerometerMisalignment_2[ 0 ] = std::stof( values[ 19 ] );
        AccelerometerMisalignment_2[ 1 ] = std::stof( values[ 20 ] );
        AccelerometerMisalignment_2[ 2 ] = std::stof( values[ 21 ] );
        AccelerometerMisalignment_3[ 0 ] = std::stof( values[ 22 ] );
        AccelerometerMisalignment_3[ 1 ] = std::stof( values[ 23 ] );
        AccelerometerMisalignment_3[ 2 ] = std::stof( values[ 24 ] );
        AccelerometerSensitivity[ 0 ]    = std::stof( values[ 25 ] );
        AccelerometerSensitivity[ 1 ]    = std::stof( values[ 26 ] );
        AccelerometerSensitivity[ 2 ]    = std::stof( values[ 27 ] );
        AccelerometerOffset[ 0 ]         = std::stof( values[ 28 ] );
        AccelerometerOffset[ 1 ]         = std::stof( values[ 29 ] );
        AccelerometerOffset[ 2 ]         = std::stof( values[ 30 ] );
        //
        SoftIronMatrix_1[ 0 ] = std::stof( values[ 31 ] );
        SoftIronMatrix_1[ 1 ] = std::stof( values[ 32 ] );
        SoftIronMatrix_1[ 2 ] = std::stof( values[ 33 ] );
        SoftIronMatrix_2[ 0 ] = std::stof( values[ 34 ] );
        SoftIronMatrix_2[ 1 ] = std::stof( values[ 35 ] );
        SoftIronMatrix_2[ 2 ] = std::stof( values[ 36 ] );
        SoftIronMatrix_3[ 0 ] = std::stof( values[ 37 ] );
        SoftIronMatrix_3[ 1 ] = std::stof( values[ 38 ] );
        SoftIronMatrix_3[ 2 ] = std::stof( values[ 39 ] );
        HardIronOffset[ 0 ]   = std::stof( values[ 40 ] );
        HardIronOffset[ 1 ]   = std::stof( values[ 41 ] );
        HardIronOffset[ 2 ]   = std::stof( values[ 42 ] );
        //
        ahrs_convention            = std::stoi( values[ 43 ] );
        ahrs_gain                  = std::stof( values[ 44 ] );
        ahrs_gyroscopeRange        = std::stof( values[ 45 ] );
        ahrs_accelerationRejection = std::stof( values[ 46 ] );
        ahrs_magneticRejection     = std::stof( values[ 47 ] );
        ahrs_recoveryTriggerPeriod = std::stoul( values[ 48 ] );
    }
};
//
static void v2a()
{
    int count = sensor_data_vector.size();
    for ( int i = 0; i < count; i++ )
    {
        roll[ i ]  = sensor_data_vector[ i ].roll;
        pitch[ i ] = sensor_data_vector[ i ].pitch;
        yaw[ i ]   = sensor_data_vector[ i ].yaw;
        //
        magx[ i ] = sensor_data_vector[ i ].mag_x;
        magy[ i ] = sensor_data_vector[ i ].mag_y;
        magz[ i ] = sensor_data_vector[ i ].mag_z;
        //
        gyrx[ i ] = sensor_data_vector[ i ].gyro_x;
        gyry[ i ] = sensor_data_vector[ i ].gyro_y;
        gyrz[ i ] = sensor_data_vector[ i ].gyro_z;
        //
        accx[ i ] = sensor_data_vector[ i ].acc_x;
        accy[ i ] = sensor_data_vector[ i ].acc_y;
        accz[ i ] = sensor_data_vector[ i ].acc_z;
        //
        eax[ i ] = sensor_data_vector[ i ].eacc_x;
        eay[ i ] = sensor_data_vector[ i ].eacc_y;
        eaz[ i ] = sensor_data_vector[ i ].eacc_z;
        //
        evx[ i ] = sensor_data_vector[ i ].vel_x;
        evy[ i ] = sensor_data_vector[ i ].vel_y;
        evz[ i ] = sensor_data_vector[ i ].vel_z;
        //
        px[ i ] = sensor_data_vector[ i ].pos_x;
        py[ i ] = sensor_data_vector[ i ].pos_y;
        pz[ i ] = sensor_data_vector[ i ].pos_z;
    }
    //
    int original_count = original_sensor_data_vector.size();
    //
    for ( int i = 0; i < original_count; i++ )
    {
        original_eax[ i ] = original_sensor_data_vector[ i ].eacc_x;
        original_eay[ i ] = original_sensor_data_vector[ i ].eacc_y;
        original_eaz[ i ] = original_sensor_data_vector[ i ].eacc_z;
        //
        original_evx[ i ] = original_sensor_data_vector[ i ].vel_x;
        original_evy[ i ] = original_sensor_data_vector[ i ].vel_y;
        original_evz[ i ] = original_sensor_data_vector[ i ].vel_z;
        //
        original_px[ i ] = original_sensor_data_vector[ i ].pos_x;
        original_py[ i ] = original_sensor_data_vector[ i ].pos_y;
        original_pz[ i ] = original_sensor_data_vector[ i ].pos_z;
    }
}
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

        if ( startsWith( websocket_receive_message_original.c_str(), "Setup" ) )
        {
            websocket_receive_message = websocket_receive_message_original;
            //
            interpretConfig( websocket_receive_message_original.c_str() );
        }
        else if ( startsWith( websocket_receive_message_original.c_str(), "Stoped" ) )
        {
            websocket_receive_message = websocket_receive_message_original;
        }
        else if ( startsWith( websocket_receive_message_original.c_str(), "Connected" ) )
        {
            websocket_receive_message = websocket_receive_message_original;
        }
        else if ( startsWith( websocket_receive_message_original.c_str(), "AfterCalculation:" ) )
        {
            // printf( "%s\n", websocket_receive_message_original.c_str() );
            std::string receive_message = websocket_receive_message_original.c_str();
            receive_message             = removePrefix( receive_message, "AfterCalculation:" );
            // printf( "%s\n", receive_message.c_str() );
            //
            std::lock_guard< std::mutex > lock( queue_mutex );
            //
            SENSOR_DB new_sensor_db;
            new_sensor_db.getValueFromString( receive_message );
            //
            sensor_data_queue.push( new_sensor_db );
            //
            if ( sensor_data_vector.size() < item_count )
            {
                sensor_data_vector.push_back( new_sensor_db );
            }
            else
            {
                sensor_data_vector.erase( sensor_data_vector.begin() );
                sensor_data_vector.push_back( new_sensor_db );
            }
            //
            v2a();
            //
            websocket_receive_message = ( "After Calculation:" + new_sensor_db.to_info() ).c_str();
        }
        else if ( startsWith( websocket_receive_message_original.c_str(), "BeforCalculation:" ) )
        {
            // printf( "%s\n", websocket_receive_message_original.c_str() );

            std::string receive_message = websocket_receive_message_original.c_str();
            receive_message             = removePrefix( receive_message, "BeforCalculation:" );
            //
            std::lock_guard< std::mutex > lock( queue_mutex );
            //
            SENSOR_DB new_sensor_db;
            new_sensor_db.getValueFromString( receive_message );
            //
            if ( original_sensor_data_vector.size() < item_count )
            {
                original_sensor_data_vector.push_back( new_sensor_db );
            }
            else
            {
                original_sensor_data_vector.erase( original_sensor_data_vector.begin() );
                original_sensor_data_vector.push_back( new_sensor_db );
            }
            //
            v2a();
            //
            // websocket_receive_message = ( "Befor Calculation:" + new_sensor_db.to_info() ).c_str();
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
