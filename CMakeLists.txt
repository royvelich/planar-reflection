cmake_minimum_required(VERSION 3.10)
project(planar-reflection)

set(CMAKE_CXX_STANDARD 20)

# Sources
file(GLOB SOURCES
	src/main.cpp)

set(SOURCES src/main.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# glfw
add_subdirectory(libs/glfw EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# glew
add_definitions(-DGLEW_STATIC)
add_subdirectory(libs/glew EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static)

# glm
add_subdirectory(libs/glm EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)

# native file dialog
add_subdirectory(libs/nativefiledialog EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE nativefiledialog)

# Source Tree
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${imgui_sources})