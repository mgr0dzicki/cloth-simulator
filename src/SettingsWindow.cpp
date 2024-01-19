#include "SettingsWindow.hpp"
#include <imgui.h>

SettingsWindow::SettingsWindow(
    std::function<void(int)> subdivisionStepsCallback)
    : subdivisionStepsCallback(subdivisionStepsCallback){};

void SettingsWindow::draw(float dt) {
    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(
        ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y),
        ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_FirstUseEver);

    ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("%.3f ms / frame", 1000.f * dt);
    ImGui::Separator();
    if (ImGui::SliderInt("Subdivision steps", &subdivisionSteps, 0, 3))
        subdivisionStepsCallback(subdivisionSteps);
    ImGui::End();
}
