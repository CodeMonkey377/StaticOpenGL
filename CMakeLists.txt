cmake_minimum_required(VERSION 3.26)
project(StaticOpenGL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # This requires c++ 23, could cause issues
set(CMAKE_EXE_LINKER_FLAGS "-static")

include_directories(glad/include)
include_directories(glfw-3.4/include)

add_library(glad STATIC glad/src/glad.c)

set(GLFW_BUILD_WAYLAND OFF)
add_subdirectory(glfw-3.4)

add_executable(StaticOpenGL main.cpp glad/src/glad.c)

target_link_libraries(StaticOpenGL glad glfw ${GLFW_LIBRARIES})
# If I decide to use windows or unix specific static libraries
#if(WIN32)
#  set(CMAKE_GENERATOR "Ninja")
#else()
#  set(CMAKE_GENERATOR "Unix Makefiles")
#endif()

#if(WIN32)
#    target_sources(YourExecutableName PRIVATE win_specific.cpp)
#elseif(UNIX)
#    target_sources(YourExecutableName PRIVATE linux_specific.cpp)
#endif()