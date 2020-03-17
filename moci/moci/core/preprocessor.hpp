#pragma once

#if defined(_WIN32) || defined(_WIN64)
#define MOCI_WIN32 1
#define MOCI_WINDOWS 1
#elif defined(MOCI_ANDROID)
#undef MOCI_ANDROID
#define MOCI_ANDROID 1
#elif defined(__FreeBSD__) || (__OpenBSD__)
#define MOCI_BSD 1
#elif defined(LINUX) || defined(__linux__)
#define MOCI_LINUX 1
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
#define CF_EXCLUDE_CSTD_HEADERS 1
#include <CoreFoundation/CoreFoundation.h>  // (needed to find out what platform we're using)

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#define MOCI_IPHONE 1
#define MOCI_IOS 1
#else
#define MOCI_MAC 1
#endif
#else
#error "Unknown platform!"
#endif

#if MOCI_WINDOWS
#ifdef _MSC_VER
#ifdef _WIN64
#define MOCI_64BIT 1
#else
#define MOCI_32BIT 1
#endif
#endif

#ifdef _DEBUG
#define MOCI_DEBUG 1
#endif

#ifdef __MINGW32__
#define MOCI_MINGW 1
#ifdef __MINGW64__
#define MOCI_64BIT 1
#else
#define MOCI_32BIT 1
#endif
#endif

/** If defined, this indicates that the processor is little-endian. */
#define MOCI_LITTLE_ENDIAN 1

#define MOCI_INTEL 1
#endif

#if MOCI_MAC || MOCI_IOS

#if defined(DEBUG) || defined(_DEBUG) || !(defined(NDEBUG) || defined(_NDEBUG))
#define MOCI_DEBUG 1
#endif

#if !(defined(DEBUG) || defined(_DEBUG) || defined(NDEBUG) || defined(_NDEBUG))
#error                                                                                                                 \
    "Neither NDEBUG or DEBUG has been defined - you should set one of these to make it clear whether this is a release build,"
#endif

#ifdef __LITTLE_ENDIAN__
#define MOCI_LITTLE_ENDIAN 1
#else
#define MOCI_BIG_ENDIAN 1
#endif

#ifdef __LP64__
#define MOCI_64BIT 1
#else
#define MOCI_32BIT 1
#endif

#if defined(__ppc__) || defined(__ppc64__)
#error "PowerPC is no longer supported by JUCE!"
#elif defined(__arm__) || defined(__arm64__)
#define MOCI_ARM 1
#else
#define MOCI_INTEL 1
#endif

#if MOCI_MAC
#if !defined(MAC_OS_X_VERSION_10_11)
#error                                                                                                                 \
    "The 10.11 SDK (Xcode 7.3.1+) is required to build JUCE apps. You can create apps that run on macOS 10.7+ by changing the deployment target."
#elif MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_7
#error "Building for OSX 10.6 is no longer supported!"
#endif
#endif
#endif

#if MOCI_LINUX || MOCI_ANDROID

#ifdef _DEBUG
#define MOCI_DEBUG 1
#endif

// Allow override for big-endian Linux platforms
#if defined(__LITTLE_ENDIAN__) || !defined(MOCI_BIG_ENDIAN)
#define MOCI_LITTLE_ENDIAN 1
#undef MOCI_BIG_ENDIAN
#else
#undef MOCI_LITTLE_ENDIAN
#define MOCI_BIG_ENDIAN 1
#endif

#if defined(__LP64__) || defined(_LP64) || defined(__arm64__)
#define MOCI_64BIT 1
#else
#define MOCI_32BIT 1
#endif

#if defined(__arm__) || defined(__arm64__) || defined(__aarch64__)
#define MOCI_ARM 1
#define MOCI_GRAPHICS_API_OPENGLES 1
#elif __MMX__ || __SSE__ || __amd64__
#define MOCI_INTEL 1
#endif
#endif

// Compiler type macros.
#if defined(__clang__)
#define MOCI_COMPILER_CLANG 1

#elif defined(__GNUC__)
#define MOCI_COMPILER_GCC 1

#elif defined(_MSC_VER)
#define MOCI_COMPILER_MSVC 1

#else
#error unknown compiler
#endif

#define MOCI_ENABLE_ASSERTS 1
#ifdef MOCI_ENABLE_ASSERTS
#define MOCI_ASSERT(x, ...)                                                                                            \
    {                                                                                                                  \
        if (!(x))                                                                                                      \
        {                                                                                                              \
            MOCI_CRITICAL("Assertion Failed: {0}:{1}: {2}", __FILE__, __LINE__, __VA_ARGS__);                          \
        }                                                                                                              \
    }
#define MOCI_CORE_ASSERT(x, ...)                                                                                       \
    {                                                                                                                  \
        if (!(x))                                                                                                      \
        {                                                                                                              \
            MOCI_CORE_CRITICAL("Assertion Failed: {0}:{1}: {2}", __FILE__, __LINE__, __VA_ARGS__);                     \
        }                                                                                                              \
    }
#else
#define MOCI_ASSERT(x, ...)
#define MOCI_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define MOCI_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)