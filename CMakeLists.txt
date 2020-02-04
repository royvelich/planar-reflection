cmake_minimum_required(VERSION 3.10)
project(planar-reflection)

set(CMAKE_CXX_STANDARD 20)

# sources
file(GLOB SOURCES
	src/*.cpp
	include/*.h
	shaders/*.glsl)

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

# imgui
set(imgui_INCLUDE_DIRS "libs/imgui/imgui")
add_subdirectory(libs/imgui EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)
target_include_directories(${PROJECT_NAME} PUBLIC ${imgui_INCLUDE_DIRS})

# project
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

# source tree
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES})