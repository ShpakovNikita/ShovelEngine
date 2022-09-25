cmake_minimum_required(VERSION 3.12.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(MINECRAFT_SOURCE_PATH "${CMAKE_SOURCE_DIR}/src")
# set(ENGINE_SOURCE_PATH "${CMAKE_SOURCE_DIR}/src")


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

include_directories(external)
include_directories(external/glm)
include_directories(external/stb)
include_directories(external/entt)
include_directories(external/tinygltf)
include_directories(external/metal-cpp)
include_directories(${MINECRAFT_SOURCE_PATH})
# include_directories(${ENGINE_SOURCE_PATH})

# add_definitions(-DASSETS_DIR=\"${ASSETS_DIR}/\")

if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    add_definitions(-DGAME_DEBUG)
    add_definitions(-DSHV_DEBUG)
endif (CMAKE_BUILD_TYPE MATCHES DEBUG)

set_source_files_properties(${METAL_SHADERS} ${HEADER_FILES} PROPERTIES LANGUAGE METAL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${TARGET_NAME} ${SOURCE_FILES} ${METAL_SHADERS}) # ${ASSETS_DIR}

source_group(TREE ${SOURCE_DIR} FILES ${SOURCE_FILES})

target_link_libraries(${TARGET_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${TARGET_NAME} "-framework Metal" "-framework QuartzCore")

target_compile_definitions(${TARGET_NAME}
        PUBLIC
        $<$<CONFIG:Debug>:SHV_DEBUG>
        )
target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic) # -Werror -Wgnu-anonymous-struct
