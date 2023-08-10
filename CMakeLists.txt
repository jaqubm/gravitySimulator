cmake_minimum_required(VERSION 3.0)
project(gravitySimulator)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

include_directories(include)

add_executable(gravitySimulator src/main.cpp
        include/include.h
        include/config.h
        include/Object.h
        src/GravitySource/GravitySource.h
        src/GravitySource/GravitySource.cpp
        src/Particle/Particle.h
        src/Particle/Particle.cpp
        src/Scene/Scene.cpp
        src/Scene/Scene.h
)

target_link_libraries(gravitySimulator PRIVATE sfml-graphics)
target_compile_features(gravitySimulator PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET gravitySimulator POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_RUNTIME_DLLS:gravitySimulator> $<TARGET_FILE_DIR:gravitySimulator> COMMAND_EXPAND_LISTS)
endif()

add_custom_command(TARGET gravitySimulator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:gravitySimulator>/res COMMAND_EXPAND_LISTS)

install(TARGETS gravitySimulator)
