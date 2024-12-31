cmake_minimum_required(VERSION 3.21)
project(CMakeSFMLProject LANGUAGES CXX)

# Enforce C++17 standard for the entire project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# B2D - Box2D physics library
add_subdirectory("lib/b2d/Box2D")
set(B2D_INCS "lib/b2d/Box2D/")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

message("sfml lib: ${CMAKE_BINARY_DIR}/lib/sfml/lib")
set(SFML_INCS "${CMAKE_BINARY_DIR}/lib/sfml/include")
set(SFML_LIBS "${CMAKE_BINARY_DIR}/lib/sfml/lib")

file(GLOB_RECURSE RESOURCE_FILES "res/*.*")
## Engine ##
file(GLOB_RECURSE SOURCE_FILES engine/*.cpp engine/*.h)
add_library(lib_engine STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_engine INTERFACE "${CMAKE_SOURCE_DIR}/engine/" PUBLIC SYSTEM ${B2D_INCS} )
target_link_libraries(lib_engine lib_tile_level_loader lib_maths lib_ecm Box2D sfml-graphics sfml-audio)

## Tile loader lib
file(GLOB_RECURSE SOURCE_FILES lib_tile_level_loader/*.cpp lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib_tile_level_loader/" )
target_link_libraries(lib_tile_level_loader lib_maths sfml-graphics)

# Maths library
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths")
target_link_libraries(lib_maths INTERFACE sfml-system)

## ECM lib
file(GLOB_RECURSE SOURCE_FILES lib_ecm/*.cpp lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib_ecm" )
target_link_libraries(lib_ecm PRIVATE lib_maths)

add_executable(CMakeSFMLProject
        src/platformer.cpp
        src/scenes/scene_level1.cpp
        src/scenes/scene_level2.cpp
        src/scenes/scene_level3.cpp
        src/scenes/scene_menu.cpp
        src/components/cmp_player_physics.cpp
        src/components/cmp_sprite.cpp
        src/components/cmp_physics.cpp
        src/components/cmp_enemy_turret.cpp
        src/components/cmp_bullet.cpp
        src/components/cmp_hut_player.cpp
        src/components/cmp_enemy_ai.cpp
        src/components/cmp_actor_movement.cpp
        src/components/cmp_button.cpp
        src/components/cmp_shooting.cpp
        src/components/cmp_enitity_health.cpp
        src/components/cmp_enitity_health.h
        src/components/cmp_collision.cpp
        src/components/cmp_collision.h

)
target_link_libraries(CMakeSFMLProject PRIVATE sfml-graphics sfml-window sfml-system lib_engine)
target_include_directories(CMakeSFMLProject PRIVATE ${SFML_INCLUDE_DIR})

# Copy the res folder to the binary directory so resources are accessible
file(COPY res DESTINATION ${CMAKE_BINARY_DIR}/)

file(COPY res DESTINATION ${CMAKE_BINARY_DIR})
target_compile_features(CMakeSFMLProject PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET CMakeSFMLProject POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:CMakeSFMLProject> $<TARGET_FILE_DIR:CMakeSFMLProject> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS CMakeSFMLProject)