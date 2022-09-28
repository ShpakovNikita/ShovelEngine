cmake_minimum_required(VERSION 3.12.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(MINECRAFT_SOURCE_PATH "${CMAKE_SOURCE_DIR}/src")

set(ENGINE_SOURCE_PATH "${MINECRAFT_SOURCE_PATH}/Engine")
set(METAL_RENDER_SOURCE_PATH "${ENGINE_SOURCE_PATH}/Render/Metal")
set(OPEN_GL_RENDER_SOURCE_PATH "${ENGINE_SOURCE_PATH}/Render/OpenGl")


# set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")
# set(SHADERS_LIB "MetalShaders")
# set(COMMON_LIB "CommonLib")

set(TARGET_NAME Minecraft)
project(${TARGET_NAME})

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE METAL_SHADERS LIST_DIRECTORIES false
        "${SOURCE_DIR}/*.metal"
        )
file(GLOB_RECURSE SOURCE_FILES LIST_DIRECTORIES false
        "${SOURCE_DIR}/*.cpp"
        "${SOURCE_DIR}/*.mm"
        )
file(GLOB_RECURSE HEADER_FILES LIST_DIRECTORIES false
        "${SOURCE_DIR}/*.hpp"
        )
set(SOURCE_FILES ${SOURCE_FILES} ${HEADER_FILES} ${METAL_SHADERS})

include_directories(SYSTEM external/glm)
include_directories(SYSTEM external/stb)
include_directories(SYSTEM external/entt)
include_directories(SYSTEM external/tinygltf)
include_directories(SYSTEM external/metal-cpp)
include_directories(SYSTEM external/glew/build/cmake)
include_directories(${MINECRAFT_SOURCE_PATH})
# include_directories(${ENGINE_SOURCE_PATH})

# add_definitions(-DASSETS_DIR=\"${ASSETS_DIR}/\")
add_definitions(-DOPEN_GL_SHADERS_DIR=\"${OPEN_GL_RENDER_SOURCE_PATH}/Shaders/\")

if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    add_definitions(-DGAME_DEBUG)
    add_definitions(-DSHV_DEBUG)
endif (CMAKE_BUILD_TYPE MATCHES DEBUG)

set_source_files_properties(${METAL_SHADERS} ${HEADER_FILES} PROPERTIES LANGUAGE METAL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(GLEW 2.0 REQUIRED)

add_executable(${TARGET_NAME} ${SOURCE_FILES} ${METAL_SHADERS}) # ${ASSETS_DIR}

source_group(TREE ${SOURCE_DIR} FILES ${SOURCE_FILES})

target_link_libraries(${TARGET_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${TARGET_NAME} GLEW::GLEW)
target_link_libraries(${TARGET_NAME} "-framework Metal" "-framework QuartzCore")

target_compile_definitions(${TARGET_NAME}
        PUBLIC
        $<$<CONFIG:Debug>:SHV_DEBUG>
        )
target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
