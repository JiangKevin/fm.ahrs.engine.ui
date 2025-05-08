//
// Copyright (c) 2008-2017 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#ifdef _WIN32

#ifdef _MSC_VER
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#endif

#define URHO3D_EXPORT_API __declspec(dllexport)
#define URHO3D_IMPORT_API __declspec(dllimport)

#ifdef URHO3D_STATIC
#  define URHO3D_API
#  define URHO3D_NO_EXPORT
#else
#  ifndef URHO3D_API
#    ifdef URHO3D_EXPORTS
        /* We are building this library */
#      define URHO3D_API URHO3D_EXPORT_API
#    else
        /* We are using this library */
#      define URHO3D_API URHO3D_IMPORT_API
#    endif
#  endif

#  ifndef URHO3D_NO_EXPORT
#    define URHO3D_NO_EXPORT
#  endif
#endif

#ifndef URHO3D_DEPRECATED
#  define URHO3D_DEPRECATED __declspec(deprecated)
#endif

#ifndef URHO3D_DEPRECATED_EXPORT
#  define URHO3D_DEPRECATED_EXPORT URHO3D_API URHO3D_DEPRECATED
#endif

#ifndef URHO3D_DEPRECATED_NO_EXPORT
#  define URHO3D_DEPRECATED_NO_EXPORT URHO3D_NO_EXPORT URHO3D_DEPRECATED
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define URHO3D_NO_DEPRECATED
#endif

#else

#define URHO3D_EXPORT_API __attribute__((visibility("default")))
#define URHO3D_IMPORT_API __attribute__((visibility("default")))

#ifdef URHO3D_STATIC
#ifndef URHO3D_API
#  define URHO3D_API
#endif
#  define URHO3D_NO_EXPORT
#else
#  define URHO3D_API URHO3D_EXPORT_API
#  ifndef URHO3D_NO_EXPORT
#    define URHO3D_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef URHO3D_DEPRECATED
#  define URHO3D_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef URHO3D_DEPRECATED_EXPORT
#  define URHO3D_DEPRECATED_EXPORT URHO3D_API URHO3D_DEPRECATED
#endif

#ifndef URHO3D_DEPRECATED_NO_EXPORT
#  define URHO3D_DEPRECATED_NO_EXPORT URHO3D_NO_EXPORT URHO3D_DEPRECATED
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define URHO3D_NO_DEPRECATED
#endif

#endif

// Engine configuration
#ifndef URHO3D_ACTIONS
#   define URHO3D_ACTIONS 1
#endif
#ifndef URHO3D_CXX_STANDARD
#   define URHO3D_CXX_STANDARD 1
#endif
#ifndef URHO3D_DEBUG_ASSERT
#   define URHO3D_DEBUG_ASSERT 1
#endif
#ifndef URHO3D_ENABLE_ALL
#   define URHO3D_ENABLE_ALL 1
#endif
#ifndef URHO3D_FILEWATCHER
#   define URHO3D_FILEWATCHER 1
#endif
#ifndef URHO3D_HASH_DEBUG
#   define URHO3D_HASH_DEBUG 1
#endif
#ifndef URHO3D_IK
#   define URHO3D_IK 1
#endif
#ifndef URHO3D_LOGGING
#   define URHO3D_LOGGING 1
#endif
#ifndef URHO3D_NAVIGATION
#   define URHO3D_NAVIGATION 1
#endif
#ifndef URHO3D_NETWORK
#   define URHO3D_NETWORK 1
#endif
#ifndef URHO3D_PACKAGING
#   define URHO3D_PACKAGING 1
#endif
#ifndef URHO3D_PARALLEL_BUILD
#   define URHO3D_PARALLEL_BUILD 1
#endif
#ifndef URHO3D_PARTICLE_GRAPH
#   define URHO3D_PARTICLE_GRAPH 1
#endif
#ifndef URHO3D_PCH
#   define URHO3D_PCH 1
#endif
#ifndef URHO3D_PHYSICS
#   define URHO3D_PHYSICS 1
#endif
#ifndef URHO3D_PHYSICS2D
#   define URHO3D_PHYSICS2D 1
#endif
#ifndef URHO3D_PROFILING_FALLBACK
#   define URHO3D_PROFILING_FALLBACK 1
#endif
#ifndef URHO3D_PROFILING_SYSTRACE
#   define URHO3D_PROFILING_SYSTRACE 1
#endif
#ifndef URHO3D_RMLUI
#   define URHO3D_RMLUI 1
#endif
#ifndef URHO3D_SHADER_OPTIMIZER
#   define URHO3D_SHADER_OPTIMIZER 1
#endif
#ifndef URHO3D_SHADER_TRANSLATOR
#   define URHO3D_SHADER_TRANSLATOR 1
#endif
#ifndef URHO3D_SYSTEMUI
#   define URHO3D_SYSTEMUI 1
#endif
#ifndef URHO3D_SYSTEMUI_VIEWPORTS
#   define URHO3D_SYSTEMUI_VIEWPORTS 1
#endif
#ifndef URHO3D_THREADING
#   define URHO3D_THREADING 1
#endif
#ifndef URHO3D_URHO2D
#   define URHO3D_URHO2D 1
#endif
#ifndef URHO3D_WEBP
#   define URHO3D_WEBP 1
#endif
#ifndef NOMINMAX
#   define NOMINMAX 1
#endif
#ifndef URHO3D_DEBUG
#   define URHO3D_DEBUG 1=1
#endif
#ifndef STB_VORBIS_HEADER_ONLY
#   define STB_VORBIS_HEADER_ONLY 1
#endif
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#   define IMGUI_DISABLE_OBSOLETE_FUNCTIONS 1
#endif
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS 1
#endif
#ifndef USE_IMGUI_API
#   define USE_IMGUI_API 1
#endif
#ifndef GLEW_NO_GLU
#   define GLEW_NO_GLU 1
#endif
#ifndef GLEW_STATIC
#   define GLEW_STATIC 1
#endif
#ifndef FMT_USE_RVALUE_REFERENCES
#   define FMT_USE_RVALUE_REFERENCES 1
#endif
#ifndef FMT_USE_VARIADIC_TEMPLATES
#   define FMT_USE_VARIADIC_TEMPLATES 1
#endif
#ifndef SPDLOG_FMT_EXTERNAL
#   define SPDLOG_FMT_EXTERNAL 1
#endif
#ifndef SPDLOG_DISABLE_DEFAULT_LOGGER
#   define SPDLOG_DISABLE_DEFAULT_LOGGER 1
#endif
#ifndef EASTL_OPENSOURCE
#   define EASTL_OPENSOURCE 1
#endif
#ifndef EASTL_RTTI_ENABLED
#   define EASTL_RTTI_ENABLED 1
#endif
#ifndef EASTL_URHO3D_EXTENSIONS
#   define EASTL_URHO3D_EXTENSIONS 1
#endif
#ifndef EASTL_SIZE_T_32BIT
#   define EASTL_SIZE_T_32BIT 1
#endif
#ifndef EASTDC_GLOBALPTR_SUPPORT_ENABLED
#   define EASTDC_GLOBALPTR_SUPPORT_ENABLED 1
#endif
#ifndef EASTDC_THREADING_SUPPORTED
#   define EASTDC_THREADING_SUPPORTED 1
#endif
#ifndef EASTL_STD_ITERATOR_CATEGORY_ENABLED
#   define EASTL_STD_ITERATOR_CATEGORY_ENABLED 1
#endif
#ifndef EASTL_DEBUG
#   define EASTL_DEBUG 1=1
#endif
#ifndef RMLUI_NO_THIRDPARTY_CONTAINERS
#   define RMLUI_NO_THIRDPARTY_CONTAINERS 1
#endif
#ifndef RMLUI_CUSTOM_CONFIGURATION_FILE
#   define RMLUI_CUSTOM_CONFIGURATION_FILE "RmlUi/Config/RbfxConfig.h"=1
#endif
#ifndef RMLUI_DEBUG_BUILD
#   define RMLUI_DEBUG_BUILD 1=1
#endif
#ifndef RMLUI_STATIC_LIB
#   define RMLUI_STATIC_LIB 1
#endif
#ifndef ENKITS_TASK_PRIORITIES_NUM
#   define ENKITS_TASK_PRIORITIES_NUM 1
#endif
#ifndef URHO3D_STATIC
#   define URHO3D_STATIC 1
#endif
#ifndef HAVE_MALLOC_USABLE_SIZE
#   define HAVE_MALLOC_USABLE_SIZE 1
#endif
#ifndef HAVE_STDINT_H
#   define HAVE_STDINT_H 1
#endif
#ifndef HAVE_INTTYPES_H
#   define HAVE_INTTYPES_H 1
#endif
#ifndef HAVE_MALLOC_H
#   define HAVE_MALLOC_H 1
#endif


// Disable SSE if compiler does not support it.
#if defined(URHO3D_SSE) && !defined(__SSE2__) && (!defined(_M_IX86_FP) || _M_IX86_FP < 2)
#   undef URHO3D_SSE
#endif

// Platform identification macros.
#if defined(__ANDROID__)
    #define URHO3D_PLATFORM_ANDROID 1
#elif defined(IOS)
    #define URHO3D_PLATFORM_IOS 1
#elif defined(TVOS)
    #define URHO3D_PLATFORM_TVOS 1
#elif defined(__APPLE__)
    #define URHO3D_PLATFORM_MACOS 1
#elif UWP
    #define URHO3D_PLATFORM_UNIVERSAL_WINDOWS 1
#elif defined(_WIN32)
    #define URHO3D_PLATFORM_WINDOWS 1
#elif defined(RPI)
    #define URHO3D_PLATFORM_RASPBERRY_PI 1
#elif defined(__EMSCRIPTEN__)
    #define URHO3D_PLATFORM_WEB 1
#elif defined(__linux__)
    #define URHO3D_PLATFORM_LINUX 1
#else
    #error Unsupported platform
#endif

// Diligent platform defines.
// This is needed so the user can include Diligent-using headers without linking to Diligent-PublicBuildSettings.
// TODO: Can we resolve this at CMake level?
#if URHO3D_PLATFORM_WINDOWS
    #define PLATFORM_WIN32 1
#endif
#if URHO3D_PLATFORM_UNIVERSAL_WINDOWS
    #define PLATFORM_UNIVERSAL_WINDOWS 1
#endif

#if URHO3D_PLATFORM_LINUX
    #define PLATFORM_LINUX 1
#endif
#if URHO3D_PLATFORM_ANDROID
    #define PLATFORM_ANDROID 1
#endif
#if URHO3D_PLATFORM_RASPBERRY_PI
    // As workaround, we use PLATFORM_LINUX for raspberry pi
    #define PLATFORM_LINUX 1
#endif

#if URHO3D_PLATFORM_MACOS
    #define PLATFORM_MACOS 1
#endif
#if URHO3D_PLATFORM_IOS
    #define PLATFORM_IOS 1
#endif
#if URHO3D_PLATFORM_TVOS
    #define PLATFORM_TVOS 1
#endif

#if URHO3D_PLATFORM_WEB
    #define PLATFORM_EMSCRIPTEN 1
#endif
