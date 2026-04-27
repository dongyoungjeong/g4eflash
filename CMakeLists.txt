cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(eflash)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(eflash eflash.cc ${sources} ${headers})
target_include_directories(eflash PRIVATE include)
target_link_libraries(eflash PRIVATE ${Geant4_LIBRARIES})

set(EFLASH_SCRIPTS
#    eflash.in
#    eflash.out
    init_vis.mac
    run.mac
    vis.mac
    )
foreach(_script ${EFLASH_SCRIPTS})
    configure_file(
        ${PROJECT_SOURCE_DIR}/${_script}
        ${PROJECT_BINARY_DIR}/${_script}
        COPYONLY
        )
endforeach()
