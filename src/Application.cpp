/**
 * Application.hpp
 * Contributors:
 *      * Arthur Sonzogni (author)
 * Licence:
 *      * MIT
 */

#include "Application.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <stdexcept>

using namespace std;

Application *currentApplication = NULL;

Application &Application::getInstance() {
    if (currentApplication)
        return *currentApplication;
    else
        throw std::runtime_error("There is no current Application");
}

Application::Application(std::string title)
    : state(stateReady), width(1000), height(800), title(title) {
    currentApplication = this;

    cout << "[Info] GLFW initialisation" << endl;

    // initialize the GLFW library
    if (!glfwInit()) {
        throw std::runtime_error("Couldn't init GLFW");
    }

    // setting the opengl version
    int major = 3;
    int minor = 2;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    // create the window
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Couldn't create a window");
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        glfwTerminate();
        throw std::runtime_error(string("Could initialize GLEW, error = ") +
                                 (const char *)glewGetErrorString(err));
    }

    // get version info
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported " << version << endl;

    // opengl configuration
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(
        GL_LESS); // depth-testing interprets a smaller value as "closer"

    // mouse callbacks
    glfwSetCursorPosCallback(window, glfwCursorPosCallback);
    glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);

    // vsync
    // glfwSwapInterval(false);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

GLFWwindow *Application::getWindow() const {
    return window;
}

void Application::exit() {
    state = stateExit;
}

float Application::getFrameDeltaTime() const {
    return deltaTime;
}

float Application::getTime() const {
    return time;
}

void Application::run() {
    state = stateRun;

    // Make the window's context current
    glfwMakeContextCurrent(window);

    time = glfwGetTime();

    while (state == stateRun) {
        // compute new time and delta time
        float t = glfwGetTime();
        deltaTime = t - time;
        time = t;

        // detech window related changes
        detectWindowDimensionChange();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // execute the frame code
        loop();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap Front and Back buffers (double buffering)
        glfwSwapBuffers(window);

        // Pool and process events
        glfwPollEvents();
    }

    glfwTerminate();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::detectWindowDimensionChange() {
    int w, h;
    glfwGetWindowSize(getWindow(), &w, &h);
    dimensionChanged = (w != width || h != height);
    if (dimensionChanged) {
        width = w;
        height = h;
        glViewport(0, 0, width, height);
    }
}

void Application::loop() {
    cout << "[INFO] : loop" << endl;
}

int Application::getWidth() {
    return width;
}

int Application::getHeight() {
    return height;
}

float Application::getWindowRatio() {
    return float(width) / float(height);
}

bool Application::windowDimensionChanged() {
    return dimensionChanged;
}

void Application::glfwCursorPosCallback(GLFWwindow *window, double x,
                                        double y) {
    Application::getInstance().onMouseMove(x, y);
}

void Application::glfwMouseButtonCallback(GLFWwindow *window, int button,
                                          int action, int mods) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    Application::getInstance().onMouseButton(button, action, mods, x, y);
}
