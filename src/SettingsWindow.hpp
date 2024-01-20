#ifndef SETTINGSWINDOW_HPP
#define SETTINGSWINDOW_HPP

#include "Cloth.hpp"
#include <functional>

namespace Settings {
extern bool regularLinks;
extern bool diagonalLinks;
extern bool farLinks;
} // namespace Settings

class SettingsWindow {
  public:
    SettingsWindow(std::function<void(int)> subdivisionStepsCallback);
    void draw(float dt);

  private:
    int subdivisionSteps = Cloth::SUBDIVISION_STEPS_MAX;
    std::function<void(int)> subdivisionStepsCallback;
};

#endif // SETTINGSWINDOW_HPP
