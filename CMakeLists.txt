cmake_minimum_required(VERSION 2.8)

project(Lect7)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(Prog task2.cpp)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${SDL_INCLUDE_DIR})
target_link_libraries(Prog ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

set(EXECUTABLE_OUTPUT_PATH ../bin)
