cmake_minimum_required(VERSION 3.20)
project(NetworkPractice)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/bin)

if(WIN32)
    link_libraries(ws2_32)
endif()

file(GLOB_RECURSE SOURCES "examples/*.cpp")

foreach(source_file ${SOURCES})
    get_filename_component(name ${source_file} NAME_WE)
    add_executable(${name} ${source_file})
    if(WIN32)
        target_link_libraries(${name} PRIVATE ${PLATFORM_LIBS})
    endif()
endforeach()