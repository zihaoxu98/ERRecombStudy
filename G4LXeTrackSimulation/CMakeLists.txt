cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(test)

option(VIS "Build example with Geant4 UI and Vis drivers" OFF)
if(VIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(test main.cpp ${sources} ${headers})
target_link_libraries(test ${Geant4_LIBRARIES})



