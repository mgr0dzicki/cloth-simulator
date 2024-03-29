#include "SettingsWindow.hpp"
#include "Settings.hpp"
#include <imgui.h>

void drawSettingsWindow(float dt) {
    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(
        ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y),
        ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_FirstUseEver);

    ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("%.3f ms / frame", 1000.f * dt);
    if (ImGui::Button("Reset"))
        settings.resetCallback();
    ImGui::SameLine();
    if (settings.frozen) {
        if (ImGui::Button("Unfreeze"))
            settings.unfreezeCallback();
    } else {
        if (ImGui::Button("Freeze"))
            settings.freezeCallback();
    }

    ImGui::Separator();

    if (ImGui::SliderInt("Mesh size", &settings.meshSize,
                         Settings::MIN_MESH_SIZE, Settings::MAX_MESH_SIZE))
        settings.resetCallback();

    ImGui::Text("Render mode");
    ImGui::RadioButton("Points", (int *)&settings.renderMode,
                       (int)Settings::RenderMode::Points);
    ImGui::SameLine();
    ImGui::RadioButton("Lines", (int *)&settings.renderMode,
                       (int)Settings::RenderMode::Lines);
    ImGui::SameLine();
    ImGui::RadioButton("Triangles", (int *)&settings.renderMode,
                       (int)Settings::RenderMode::Triangles);
    ImGui::Spacing();

    if (settings.renderMode == Settings::RenderMode::Triangles) {
        if (ImGui::SliderInt("Subdivision steps", &settings.subdivisionSteps,
                             Settings::MIN_SUBDIVISION_STEPS,
                             Settings::MAX_SUBDIVISION_STEPS))
            settings.subdivisionStepsCallback();
        ImGui::Spacing();
    }

    ImGui::Text("Links");
    ImGui::Checkbox("Regular", &settings.regularLinks);
    ImGui::SameLine();
    ImGui::Checkbox("Diagonal", &settings.diagonalLinks);
    ImGui::SameLine();
    ImGui::Checkbox("Far", &settings.farLinks);
    ImGui::Spacing();

    ImGui::Checkbox("Cloth-cloth collision", &settings.clothClothCollision);
    ImGui::Spacing();

    if (ImGui::Button("Recommended"))
        settings.setRecommended();
    ImGui::SameLine();
    if (ImGui::Button("All disabled"))
        settings.setAllDisabled();

    ImGui::End();
}
