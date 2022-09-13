add_library(WireEngine
        src/Core/wire_window.cpp
        src/Core/Internal/error_handling.cpp
        src/Core/wire_renderer.cpp
        src/Core/wire_shader_manager.cpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# clear everything in include directory
file(GLOB_RECURSE INCLUDE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*")

foreach(INCLUDE_FILE ${INCLUDE_FILES})
    file(REMOVE ${INCLUDE_FILE})
endforeach()

# copy header files from src to include
file(GLOB_RECURSE HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

foreach(HEADER_FILE ${HEADER_FILES})
    file(RELATIVE_PATH RELATIVE_HEADER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src ${HEADER_FILE})
    get_filename_component(RELATIVE_HEADER_DIR ${RELATIVE_HEADER_FILE} DIRECTORY)
    file(COPY ${HEADER_FILE} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/${RELATIVE_HEADER_DIR})
endforeach()

# add dependencies to the library

add_subdirectory(dependencies/glfw-3.3.8)
add_subdirectory(dependencies/glew-2.2.0/build/cmake)

# link dependencies to the library

target_link_libraries(WireEngine PUBLIC glfw glew_s)
target_include_directories(WireEngine
        PUBLIC dependencies/glfw-3.3.8/include
        dependencies/glew-2.2.0/include)

# if release build, static link libraries

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(WireEngine PUBLIC -static)
endif()