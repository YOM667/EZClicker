#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
namespace utility
{
	namespace gui
	{
		ImVec2 getTextSize(const char* text);
	}
}