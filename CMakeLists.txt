add_library(WireEngine
        src/Core/wire_window.cpp
        src/Core/Internal/error_handling.cpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(include/glfw-3.3.8)
add_subdirectory(include/glew-2.2.0/build/cmake)

target_link_libraries(WireEngine PUBLIC glfw glew_s)
target_include_directories(WireEngine PUBLIC include/glfw-3.3.8/include)
target_include_directories(WireEngine PUBLIC include/glew-2.2.0/include)