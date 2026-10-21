cmake_minimum_required(VERSION 3.18)
project(BlueHedgehogGameEngine VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLUT REQUIRED)
find_path(SOIL_INCLUDE_DIR SOIL/SOIL.h REQUIRED)
find_library(SOIL_LIBRARY NAMES SOIL soil REQUIRED)

add_library(engine_core
  src/engine/GLParallax.cpp src/engine/GLTexture.cpp
  src/engine/GLTimer.cpp src/engine/GLCheckCollision.cpp)
target_include_directories(engine_core PUBLIC include/engine ${SOIL_INCLUDE_DIR})
target_link_libraries(engine_core PUBLIC OpenGL::GL OpenGL::GLU GLUT::GLUT ${SOIL_LIBRARY})

add_library(platformer_demo
  src/demo/GLScene.cpp src/demo/GLPlayer.cpp src/demo/GLEnms.cpp
  src/demo/GLInputs.cpp src/demo/GLLight.cpp src/demo/GLModel.cpp)
target_include_directories(platformer_demo PUBLIC include/demo)
target_link_libraries(platformer_demo PUBLIC engine_core)
foreach(target engine_core platformer_demo)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endforeach()

add_executable(blue_hedgehog src/main.cpp src/app/Options.cpp src/app/Capture.cpp)
target_include_directories(blue_hedgehog PRIVATE src)
target_link_libraries(blue_hedgehog PRIVATE platformer_demo glfw)
add_custom_command(TARGET blue_hedgehog POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:blue_hedgehog>/assets)

# Optional Linux capture runner: the same engine and demo in a surfaceless EGL context.
# This runs OpenGL directly and does not require an X server or a browser.
if(UNIX AND NOT APPLE)
  find_library(EGL_LIBRARY NAMES EGL)
  if(EGL_LIBRARY)
    add_executable(blue_hedgehog_capture src/capture_main.cpp
      src/app/HeadlessContext.cpp src/app/Options.cpp src/app/Capture.cpp src/app/InputScript.cpp)
    target_include_directories(blue_hedgehog_capture PRIVATE src)
    target_link_libraries(blue_hedgehog_capture PRIVATE platformer_demo ${EGL_LIBRARY})
  endif()
endif()

include(CTest)
if(BUILD_TESTING)
  add_executable(core_tests tests/core_tests.cpp)
  target_link_libraries(core_tests PRIVATE platformer_demo)
  add_test(NAME core_behaviour COMMAND core_tests)
  add_test(NAME cli_help COMMAND blue_hedgehog --help)
  add_executable(input_script_tests tests/input_script_tests.cpp src/app/InputScript.cpp)
  target_include_directories(input_script_tests PRIVATE src)
  add_test(NAME capture_input_script COMMAND input_script_tests)
endif()
