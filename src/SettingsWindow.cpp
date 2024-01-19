#include "SettingsWindow.hpp"
#include <imgui.h>

namespace Settings {
bool diagonalLinks = true;
bool farLinks = true;
} // namespace Settings

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
    ImGui::Checkbox("Diagonal links", &Settings::diagonalLinks);
    ImGui::Checkbox("Far links", &Settings::farLinks);
    if (ImGui::SliderInt("Subdivision steps", &subdivisionSteps, 0, 3))
        subdivisionStepsCallback(subdivisionSteps);
    ImGui::End();
}
