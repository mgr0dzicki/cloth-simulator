#include "Settings.hpp"

Settings settings;

Settings::Settings() {
    setRecommended();
}

int Settings::subdividedMeshSize() {
    return meshSize * (1 << subdivisionSteps) + 1;
}

void Settings::setRecommended() {
    regularLinks = true;
    diagonalLinks = true;
    farLinks = false;
    clothClothCollision = true;
    renderMode = RenderMode::Triangles;

    if (subdivisionSteps != MAX_SUBDIVISION_STEPS) {
        subdivisionSteps = MAX_SUBDIVISION_STEPS;
        subdivisionStepsCallback();
    } else {
        subdivisionSteps = MAX_SUBDIVISION_STEPS;
    }

    if (meshSize != RECOMMENDED_MESH_SIZE) {
        meshSize = RECOMMENDED_MESH_SIZE;
        resetCallback();
    } else {
        meshSize = RECOMMENDED_MESH_SIZE;
    }
}

void Settings::setAllDisabled() {
    regularLinks = false;
    diagonalLinks = false;
    farLinks = false;
    clothClothCollision = false;
    renderMode = RenderMode::Points;

    if (subdivisionSteps != MIN_SUBDIVISION_STEPS) {
        subdivisionSteps = MIN_SUBDIVISION_STEPS;
        subdivisionStepsCallback();
    } else {
        subdivisionSteps = MIN_SUBDIVISION_STEPS;
    }

    if (meshSize != MIN_MESH_SIZE) {
        meshSize = MIN_MESH_SIZE;
        resetCallback();
    } else {
        meshSize = MIN_MESH_SIZE;
    }
}

void Settings::registerSubdivisionStepsCallback(
    std::function<void()> callback) {
    subdivisionStepsCallback = callback;
}

void Settings::registerResetCallback(std::function<void()> callback) {
    resetCallback = callback;
}

void Settings::registerFreezeCallback(std::function<void()> callback) {
    freezeCallback = [this, callback] {
        frozen = true;
        callback();
    };
}

void Settings::registerUnfreezeCallback(std::function<void()> callback) {
    unfreezeCallback = [this, callback] {
        frozen = false;
        callback();
    };
}
