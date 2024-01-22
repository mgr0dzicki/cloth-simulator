# Projects Settings
cmake_minimum_required (VERSION 3.1)
project (cloth-simulator)

set(IMGUI_DIR lib/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

# The main executable
add_executable(cloth-simulator
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp

  src/Application.cpp
  src/MyApplication.cpp
  src/glError.cpp
  src/main.cpp
  src/Shader.cpp
  src/Cloth.cpp
  src/Renderer.cpp
  src/Settings.cpp
  src/SettingsWindow.cpp
)

set_property(TARGET cloth-simulator PROPERTY CXX_STANDARD 11)
target_compile_options(cloth-simulator PRIVATE -Wall -O2)

add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)

target_link_libraries(cloth-simulator
  PRIVATE glfw
  PRIVATE libglew_static
  PRIVATE glm
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/asset.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/src/asset.hpp
)
target_include_directories(cloth-simulator
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)
