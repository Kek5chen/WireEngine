add_library(WireEngine
        src/Core/wire_window.cpp
        src/Core/Internal/error_handling.cpp)

add_subdirectory(include/glfw-3.3.8)

target_link_libraries(WireEngine PUBLIC glfw)
target_include_directories(WireEngine PUBLIC include/glfw-3.3.8/include)