cmake_minimum_required(VERSION 3.12)
project(
  Game2D
  VERSION 0
)

# Options
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Enable unit testing when building ${CMAKE_PROJECT_NAME}
set(GAME_ENABLE_TESTS OFF)


# Libraries
set(REAL_BUILD_TYPE ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE Release) # for subdirectories

set(TRACY_ENABLE ON)
set(TRACY_ON_DEMAND OFF)
link_libraries(
  ws2_32
  wsock32
  dbghelp
)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/tracy)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glad)

set(GTEST_LINKED_AS_SHARED_LIBRARY ON)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/googletest)

set(CMAKE_BUILD_TYPE ${REAL_BUILD_TYPE})


# Targets
set(GAME_OBJ_LIB_NAME "${PROJECT_NAME}objlib")
add_library(${GAME_OBJ_LIB_NAME} STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Game.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Window.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/EventSystem.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/FlagParser.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/Logger.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/FileReader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/Math.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Rendering/RenderSystem.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Rendering/SpriteComponent.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Rendering/Shader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Rendering/VAO.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Rendering/ResourceManager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Rendering/Texture.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Player/PlayerComponent.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Player/InputSystem.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Physics/PhysicsSystem.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Physics/ColliderComponents.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Physics/TransformComponent.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Physics/RigidbodyComponent.cpp
)

target_compile_options(
  ${GAME_OBJ_LIB_NAME}
  PRIVATE -Wall
  PRIVATE -Wextra
  PRIVATE -Wdeprecated
  PRIVATE -Wshadow
  PRIVATE -pedantic-errors
  PRIVATE -fmax-errors=3
)

target_include_directories(
  ${GAME_OBJ_LIB_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/tracy/public
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb_image
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/entt
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/box2d/include
)

target_link_libraries(
  ${GAME_OBJ_LIB_NAME}
  PRIVATE TracyClient
)

target_precompile_headers(
  ${GAME_OBJ_LIB_NAME}
  PUBLIC [["c:\\Programming\\c++\\Game2D\\src\\Setup.hpp"]]
)


add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_compile_options(
  ${CMAKE_PROJECT_NAME}
  PRIVATE -Wall
  PRIVATE -Wextra
  PRIVATE -Wdeprecated
  PRIVATE -Wshadow
  PRIVATE -pedantic-errors
  PRIVATE -fmax-errors=3
)

target_link_directories(
  ${CMAKE_PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/box2d/lib
)

target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  PRIVATE ${GAME_OBJ_LIB_NAME}
  PRIVATE glad
  PRIVATE SDL2
  PRIVATE SDL2main
  PRIVATE TracyClient
  PRIVATE box2d
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_PROJECT_NAME})

add_custom_target(
  copy_resources ALL
  DEPENDS copy_resources.fake_file
)

add_custom_command(
  OUTPUT copy_resources.fake_file
  COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}
)

# Tests
if(${GAME_ENABLE_TESTS})
  macro(Test name sources)
    add_executable(${name} ${sources})

    target_link_libraries(${name}
      PRIVATE ${GAME_OBJ_LIB_NAME}
      PRIVATE gtest
    )

    target_include_directories(
      ${name}
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    target_link_options(${name} PRIVATE -Wno-attributes)

    set_target_properties(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/Testing)

    add_test(
      NAME ${name}
      COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build/Testing/${name}
    )
  endmacro()

  enable_testing()
  set(CTEST_OUTPUT_ON_FAILURE ON)
  Test(FlagParserTest ${CMAKE_CURRENT_SOURCE_DIR}/test/FlagParser.cpp)
  Test(StringTest ${CMAKE_CURRENT_SOURCE_DIR}/test/String.cpp)
  Test(MathTest ${CMAKE_CURRENT_SOURCE_DIR}/test/Math.cpp)
  
endif()