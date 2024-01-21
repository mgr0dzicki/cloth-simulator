/**
 * Application.cpp
 * Contributors:
 *      * Arthur Sonzogni (author)
 * Licence:
 *      * MIT
 */

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>

struct GLFWwindow;

/// Application class:
/// * init OpenGL
/// * provide:
///   * getWidth()
///   * getHeight()
///   * getFrameDeltaTime()
///   * getWindowRatio()
///   * windowDimensionChanged()
/// * let the user define the "loop" function.
class Application {
  public:
    Application(std::string title);

    static Application &getInstance();

    // get the window id
    GLFWwindow *getWindow() const;

    // window control
    void exit();

    // delta time between frame and time from beginning
    float getFrameDeltaTime() const;
    float getTime() const;

    // application run
    void run();

    // Application informations
    //
    int getWidth();
    int getHeight();
    float getWindowRatio();
    bool windowDimensionChanged();

  private:
    enum State { stateReady, stateRun, stateExit };

    State state;

    Application &operator=(const Application &) {
        return *this;
    }

    GLFWwindow *window;

    // Time:
    float time;
    float deltaTime;

    // Dimensions:
    int width;
    int height;
    bool dimensionChanged;
    void detectWindowDimensionChange();

    // Callbacks:
    static void glfwCursorPosCallback(GLFWwindow *window, double x, double y);
    static void glfwMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

  protected:
    Application(const Application &){};

    std::string title;

    virtual void loop();
    virtual void onMouseButton(int button, int action, int mods, double x, double y) {};
    virtual void onMouseMove(double x, double y) {};
};

#endif // APPLICATION_HPP
