#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.


// The procedure entry point GetOverlappedResultEx
// could not be located in  the dynamic link library KERNEL32.dll.
// Therefore, for Windows 7 support:
#include <WinSDKVer.h>
#define WINVER _WIN32_WINNT_WIN7
#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <SDKDDKVer.h>
