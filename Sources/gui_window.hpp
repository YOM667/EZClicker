#pragma once

#include <iostream>
#include <Windows.h>
#include "imgui_utility.h";
#pragma comment(lib, "d3d11.lib")

#include "controller.hpp"
#include "pthread.hpp"
namespace gui
{
	void CheckBox(const char* str_id, bool* v)
	{
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		float height = (ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2) - 7;
		float width = height * 1.55f;
		float radius = height * 0.50f;

		if (ImGui::InvisibleButton(str_id, ImVec2(width, height)))
			*v = !*v;
		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), 1, height * 0.5f);
		draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius, IM_COL32(255, 255, 255, 255));
		draw_list->AddText(ImVec2((p.x + width - radius) + 20, (p.y + radius) - ImGui::CalcTextSize(str_id).y * .5 - 1.5), ImColor(255, 255, 255, 255), str_id);
	}
	class Window
	{
	public:

		Window(const char* title, int width, int height, ImGuiWindowFlags configs) : configs(configs), title(title) , isRunning(true)
		{
			int sceneWidth = GetSystemMetrics(SM_CXSCREEN);
			int sceneHeight = GetSystemMetrics(SM_CYSCREEN);
			this->position = { (float)(sceneWidth - width) / 2, (float)(sceneHeight - height) / 2 };
			this->size = { (float)width, (float)height };
			this->clicker = std::make_unique<controller::Clicker>(10,true,controller::Mouse::LEFT);
		}
		void render()
		{
			//set window senter once
			if (!hasSeted)
			{
				ImGui::SetNextWindowPos(position, ImGuiCond_Always);
				hasSeted = true;
			}
			//set window size
			ImGui::SetNextWindowSize(size,ImGuiCond_Always);
			ImGui::Begin(title, &isRunning, configs);
			{
				auto draw = ImGui::GetWindowDrawList();
				auto& style = ImGui::GetStyle();
				auto& io = ImGui::GetIO(); (void)io;
				this->position = ImGui::GetWindowPos();
#pragma region Render
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding,8.0f);
				if (ImGui::BeginChild("Constrained", ImVec2(-FLT_MIN, 0.0f), ImGuiChildFlags_Border))
				{
					//window size width boost readable;
					float windowWidth = size.x;
					//the width get by GetItemRectSize() function
					float itemWidth = 41;
					//take the group begin render at the window's 1/4
					float xCheckboxCenterPosision = (windowWidth - (itemWidth * 2.0f)) / 4.0f;
					float xTextCenterPosision = (windowWidth - (utility::gui::getTextSize("Left").x * 2.0f)) / 4.0f;
					
					ImGui::SetCursorPos({ xCheckboxCenterPosision ,45 });
					ImGui::BeginGroup();{
#pragma region Left CheckBox
						draw->AddTextSize({ position.x + xTextCenterPosision, position.y + 43 - utility::gui::getTextSize("Left").y }, 0xFFFFFFFF, 24.0f, "Left");
						//Render CheckBox
						ImGui::Checkbox("##left", &this->left);
#pragma endregion
						//add padding between left checkbox and right checkbos
						//padding: take the right checkbos begin render at the window's 3/4
						float checkboxPadding = windowWidth - (xCheckboxCenterPosision * 2.0f) - itemWidth - style.WindowPadding.x;
						float textPadding = windowWidth - (xTextCenterPosision * 2.0f) - utility::gui::getTextSize("Right").x + 2 - style.WindowPadding.x;
						ImGui::SameLine(checkboxPadding);
#pragma region Right CheckBox

						draw->AddTextSize({ position.x + xTextCenterPosision + textPadding , position.y + 43 - utility::gui::getTextSize("Right").y }, 0xFFFFFFFF, 24.0f, "Right");
						ImGui::Checkbox("##right", &this->right);
#pragma endregion
					}
					ImGui::EndGroup();

					ImGui::SetCursorPos({ xCheckboxCenterPosision,120 });
					ImGui::BeginGroup();{
						ImGui::SliderFloat(u8"cps", &cps, 0.1f, 50.0f, "%.1f");
					}
					draw->AddLine({ position.x + 20,position.y + 170 }, {position.x + size.x - 20,position.y + 170}, ImColor(1.0f, 1.0f, 1.0f, 0.2f));
					ImGui::EndGroup();
					ImGui::SetCursorPos({ 20,size.y - 80 });
					ImGui::BeginGroup();{
						ImGui::Checkbox(u8"Auto Hide", &autoHide);
					}
					ImGui::EndGroup();
#pragma endregion
#pragma region Logic

					ImGui::PushStyleColor(ImGuiCol_Button, { 0.43,0.21,0.85,1});
					ImGui::SetCursorPos({ 170,310 });
					if (ImGui::Button(u8"Start", {100,50}))
					{
						std::cout << "the clicker have enabled cps: " << cps << std::endl;
						clicker->cps = cps;
						if (left) clicker->mouse = controller::Mouse::LEFT;
						else if (right) clicker->mouse = controller::Mouse::RIGHT;
						clicker->start();
					}
					ImGui::PopStyleColor();
#pragma endregion
				}
				ImGui::PopStyleVar();
				ImGui::EndChild();
			}

			ImGui::End();
		}
		bool windowRunning() const
		{
			return this->isRunning;
		}
	public:
		float cps =     { 1.0f  };
		bool left =     { false };
		bool right =    { false };
		bool autoHide = { false };
		bool enable =   { true  };
	    char keybind[100];
	private:
		std::unique_ptr<controller::Clicker> clicker;
		bool               hasSeted = { false };
		ImVec2             position;
		ImVec2             size;
		bool               isRunning;
	    const char*        title;
		ImGuiWindowFlags   configs;
	};
}