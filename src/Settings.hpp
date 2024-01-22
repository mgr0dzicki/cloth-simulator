#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Cloth.hpp"
#include <functional>
#include <vector>

class Settings {
  public:
    enum class RenderMode { Points, Lines, Triangles };
    RenderMode renderMode;

    bool regularLinks;
    bool diagonalLinks;
    bool farLinks;
    bool clothClothCollision;
    int subdivisionSteps;

    Settings();

    void setRecommended();
    void setAllDisabled();

    void registerSubdivisionStepsCallback(std::function<void()> callback);
    void registerResetCallback(std::function<void()> callback);

    const std::function<void()> subdivisionStepsCallback = [this]() {
        for (auto &callback : subdivisionStepsCallbacks)
            callback();
    };

    const std::function<void()> resetCallback = [this]() {
        for (auto &callback : resetCallbacks)
            callback();
    };

  private:
    std::vector<std::function<void()>> subdivisionStepsCallbacks;
    std::vector<std::function<void()>> resetCallbacks;
};

extern Settings settings;

#endif // SETTINGS_HPP
