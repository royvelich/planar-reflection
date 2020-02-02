cmake_minimum_required(VERSION 3.10)
project(planar-reflection)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# glfw
add_subdirectory(libs/glfw EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# glew
add_definitions(-DGLEW_STATIC)
add_subdirectory(libs/glew-cmake EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static)

# glm
add_subdirectory(libs/glm EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)