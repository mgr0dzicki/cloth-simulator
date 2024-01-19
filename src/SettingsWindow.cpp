#include "SettingsWindow.hpp"
#include <imgui.h>

SettingsWindow::SettingsWindow(float dt) {
    ImGui::Begin("Settings");
    ImGui::Text("dt: %.3fms", 1000.f * dt);
    ImGui::End();
}
