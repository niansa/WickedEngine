#pragma once
// This file includes platform, os specific libraries and supplies common platform specific resources

#include <vector>
#include <mutex>
#include <string>
#include <iostream>

#ifdef _WIN32

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <SDKDDKVer.h>
#include <windows.h>
#include <tchar.h>

#if WINAPI_FAMILY == WINAPI_FAMILY_APP
#define PLATFORM_UWP
#define wiLoadLibrary(name) LoadPackagedLibrary(_T(name),0)
#define wiGetProcAddress(handle,name) GetProcAddress(handle, name)
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.Graphics.Display.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#else
#define PLATFORM_WINDOWS_DESKTOP
#define wiLoadLibrary(name) LoadLibrary(_T(name))
#define wiGetProcAddress(handle,name) GetProcAddress(handle, name)
#endif // UWP

#else

#define PLATFORM_LINUX
#define wiLoadLibrary(name) dlopen(name, RTLD_LAZY)
#define wiGetProcAddress(handle,name) dlsym(handle, name)
typedef void* HMODULE;

#endif // _WIN32

#ifdef SDL2
#include <SDL2/SDL.h>
#include <SDL_vulkan.h>
#include "sdl2.h"
#endif


namespace wiPlatform
{
#ifdef _WIN32
#ifdef PLATFORM_UWP
	using window_type = const winrt::Windows::UI::Core::CoreWindow*;
#else
	using window_type = HWND;
#endif // PLATFORM_UWP
#elif SDL2
	using window_type = SDL_Window*;
#else
	using window_type = int;
#endif // _WIN32

	inline void Exit()
	{
#ifdef _WIN32
#ifndef PLATFORM_UWP
		PostQuitMessage(0);
#else
		winrt::Windows::ApplicationModel::Core::CoreApplication::Exit();
#endif // PLATFORM_UWP
#endif // _WIN32
#ifdef SDL2
		SDL_Quit();
#endif
	}

	struct WindowProperties
	{
		int width = 0;
		int height = 0;
		float dpi = 96;
	};
	inline void GetWindowProperties(window_type window, WindowProperties* dest)
	{
#ifdef PLATFORM_WINDOWS_DESKTOP
		dest->dpi = (float)GetDpiForWindow(window);
		RECT rect;
		GetClientRect(window, &rect);
		dest->width = uint32_t(rect.right - rect.left);
		dest->height = uint32_t(rect.bottom - rect.top);
#endif // WINDOWS_DESKTOP

#ifdef PLATFORM_UWP
		dest->dpi = winrt::Windows::Graphics::Display::DisplayInformation::GetForCurrentView().LogicalDpi();
		float dpiscale = dest->dpi / 96.f;
		dest->width = uint32_t(window->Bounds().Width * dpiscale);
		dest->height = uint32_t(window->Bounds().Height * dpiscale);
#endif // PLATFORM_UWP

#ifdef PLATFORM_LINUX
		int window_width, window_height;
		SDL_GetWindowSize(window, &window_width, &window_height);
		SDL_Vulkan_GetDrawableSize(window, &dest->width, &dest->height);
		dest->dpi = ((float) dest->width / (float) window_width) * 96.0;
#endif // PLATFORM_LINUX
	}
}
