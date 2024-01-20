#include "Settings.hpp"

Settings settings;

Settings::Settings() {
    setRecommended();
}

void Settings::setRecommended() {
    regularLinks = true;
    diagonalLinks = true;
    farLinks = true;
    clothClothCollision = true;
    renderMode = RenderMode::Triangles;

    if (subdivisionSteps != Cloth::SUBDIVISION_STEPS_MAX) {
        subdivisionSteps = Cloth::SUBDIVISION_STEPS_MAX;
        subdivisionStepsCallback();
    } else {
        subdivisionSteps = Cloth::SUBDIVISION_STEPS_MAX;
    }
}

void Settings::setAllDisabled() {
    regularLinks = false;
    diagonalLinks = false;
    farLinks = false;
    clothClothCollision = false;
    renderMode = RenderMode::Points;

    if (subdivisionSteps != 0) {
        subdivisionSteps = 0;
        subdivisionStepsCallback();
    } else {
        subdivisionSteps = 0;
    }
}

void Settings::registerSubdivisionStepsCallback(
    std::function<void()> callback) {
    subdivisionStepsCallbacks.push_back(callback);
}
