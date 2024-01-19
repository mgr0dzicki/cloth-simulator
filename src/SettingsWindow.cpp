#include "SettingsWindow.hpp"
#include <imgui.h>

namespace Settings {
enum class RenderMode { Points, Lines, Triangles };

bool diagonalLinks = true;
bool farLinks = true;
bool clothClothCollision = true;
RenderMode renderMode = RenderMode::Triangles;

void recommended() {
    diagonalLinks = true;
    farLinks = true;
    clothClothCollision = true;
    renderMode = RenderMode::Triangles;
}

void allDisabled() {
    diagonalLinks = false;
    farLinks = false;
    clothClothCollision = false;
    renderMode = RenderMode::Points;
}
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
    ImGui::Button("Reset animation");

    ImGui::Separator();

    ImGui::Text("Render mode");
    ImGui::RadioButton("Points", (int *)&Settings::renderMode,
                       (int)Settings::RenderMode::Points);
    ImGui::SameLine();
    ImGui::RadioButton("Lines", (int *)&Settings::renderMode,
                       (int)Settings::RenderMode::Lines);
    ImGui::SameLine();
    ImGui::RadioButton("Triangles", (int *)&Settings::renderMode,
                       (int)Settings::RenderMode::Triangles);
    ImGui::Spacing();

    ImGui::Text("Links");
    ImGui::Checkbox("Diagonal", &Settings::diagonalLinks);
    ImGui::SameLine();
    ImGui::Checkbox("Far", &Settings::farLinks);
    ImGui::Spacing();

    ImGui::Checkbox("Cloth-cloth collision", &Settings::clothClothCollision);
    ImGui::Spacing();

    if (ImGui::SliderInt("Subdivision steps", &subdivisionSteps, 0, 3))
        subdivisionStepsCallback(subdivisionSteps);

    ImGui::Spacing();

    if (ImGui::Button("Recommended"))
        Settings::recommended();
    ImGui::SameLine();
    if (ImGui::Button("All disabled"))
        Settings::allDisabled();

    ImGui::End();
}
