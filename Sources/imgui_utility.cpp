#include "imgui_utility.h"

ImVec2 utility::gui::getTextSize(const char* text)
{
	return ImGui::CalcTextSize(text);
}
