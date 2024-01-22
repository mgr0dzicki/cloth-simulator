#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Physics.hpp"
#include <functional>
#include <glm/glm.hpp>
#include <vector>

class Settings {
  public:
    enum class RenderMode { Points, Lines, Triangles };
    RenderMode renderMode;

    bool regularLinks;
    bool diagonalLinks;
    bool farLinks;
    bool clothClothCollision;
    bool frozen = false;

    int subdivisionSteps;
    static constexpr int MIN_SUBDIVISION_STEPS = 0;
    static constexpr int MAX_SUBDIVISION_STEPS = 3;

    int meshSize;
    static constexpr int MIN_MESH_SIZE = 20;
    static constexpr int RECOMMENDED_MESH_SIZE = 40;
    static constexpr int MAX_MESH_SIZE = 50;

    int subdividedMeshSize();

    Settings();

    void setRecommended();
    void setAllDisabled();

    void registerSubdivisionStepsCallback(std::function<void()> callback);
    void registerResetCallback(std::function<void()> callback);
    void registerFreezeCallback(std::function<void()> callback);
    void registerUnfreezeCallback(std::function<void()> callback);

    std::function<void()> subdivisionStepsCallback = [] {
    };
    std::function<void()> resetCallback = [] {
    };
    std::function<void()> freezeCallback = [this] {
        frozen = true;
    };
    std::function<void()> unfreezeCallback = [this] {
        frozen = false;
    };
};

extern Settings settings;

#endif // SETTINGS_HPP
