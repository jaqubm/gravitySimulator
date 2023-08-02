cmake_minimum_required(VERSION 3.26)
project(gravitySimulator)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(gravitySimulator main.cpp)

target_link_libraries(gravitySimulator PRIVATE sfml-graphics)
target_compile_features(gravitySimulator PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET gravitySimulator POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:gravitySimulator> $<TARGET_FILE_DIR:gravitySimulator> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS gravitySimulator)