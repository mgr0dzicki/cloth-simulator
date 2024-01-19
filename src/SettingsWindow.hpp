#ifndef OPENGL_CMAKE_SKELETON_SETTINGSWINDOW
#define OPENGL_CMAKE_SKELETON_SETTINGSWINDOW

#include <functional>
#include "Cloth.hpp"

class SettingsWindow {
public:
    SettingsWindow(std::function<void(int)> subdivisionStepsCallback);
    void draw(float dt);

private:
    int subdivisionSteps = Cloth::SUBDIVISION_STEPS_MAX;
    std::function<void(int)> subdivisionStepsCallback;
};

#endif // OPENGL_CMAKE_SKELETON_SETTINGSWINDOW
