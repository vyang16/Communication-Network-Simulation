cmake_minimum_required( VERSION 3.0 )

project( simulation )

# flags

# add project files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SOURCES
     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME}_lib SHARED ${SOURCES})

# build executables
add_executable(simulation ${CMAKE_CURRENT_SOURCE_DIR}/simulation.cpp)

target_link_libraries(  simulation
                        ${PROJECT_NAME}_lib
                      )
