#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Physics.hpp"
#include <glm/glm.hpp>
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
    bool frozen = false;

    Settings();

    void setRecommended();
    void setAllDisabled();

    void registerSubdivisionStepsCallback(std::function<void()> callback);
    void registerResetCallback(std::function<void()> callback);
    void registerFreezeCallback(std::function<void()> callback);
    void registerUnfreezeCallback(std::function<void()> callback);

    const std::function<void()> subdivisionStepsCallback = [this]() {
        for (auto &callback : subdivisionStepsCallbacks)
            callback();
    };

    const std::function<void()> resetCallback = [this]() {
        for (auto &callback : resetCallbacks)
            callback();
    };

    const std::function<void()> freezeCallback = [this]() {
        frozen = true;
        for (auto &callback : freezeCallbacks)
            callback();
    };

    const std::function<void()> unfreezeCallback = [this]() {
        frozen = false;
        for (auto &callback : unfreezeCallbacks)
            callback();
    };

  private:
    std::vector<std::function<void()>> subdivisionStepsCallbacks;
    std::vector<std::function<void()>> resetCallbacks;
    std::vector<std::function<void()>> freezeCallbacks;
    std::vector<std::function<void()>> unfreezeCallbacks;
};

extern Settings settings;

#endif // SETTINGS_HPP
