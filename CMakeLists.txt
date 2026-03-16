cmake_minimum_required(VERSION 4.0)

project(gdex_toml)
set(CMAKE_CXX_STANDARD 20)

# Turn on link time optimization for everything
set( CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON )

# Godot
if (NOT TARGET godot-cpp)
    include(FetchContent)
    FetchContent_Declare(
        godot-cpp
        GIT_REPOSITORY https://github.com/godotengine/godot-cpp
        GIT_TAG 4.4
    )
    FetchContent_MakeAvailable(godot-cpp)
endif()
set( GODOT_CPP_SYSTEM_HEADERS ON CACHE BOOL "" FORCE )

add_library(${PROJECT_NAME} SHARED
#        src/toml_creator.cpp
#        src/toml_creator.h
#        src/summator.cpp
#        src/summator.h
)
add_subdirectory(src)
#include_directories(include)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    $<$<CONFIG:Debug>:DEBUG_TOML>
)

if (MSVC)
    target_compile_options(godot-cpp PRIVATE "/MP")
    target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
    set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD ON)
endif()

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        godot::cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
#        src
        include
)

set( LIB_ARCH ${CMAKE_SYSTEM_PROCESSOR} )
set( LIB_DIR "${CMAKE_SYSTEM_NAME}-${LIB_ARCH}" )
set( BUILD_OUTPUT_DIR "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/" )
message( STATUS "Building ${PROJECT_NAME} for ${LIB_ARCH} on ${CMAKE_SYSTEM_NAME}")
message( STATUS "Build output dir ${BUILD_OUTPUT_DIR}")
#set_target_properties( godot-cpp
#    PROPERTIES
#        CXX_VISIBILITY_PRESET hidden    # visibility needs to be the same as the main library
#)
set_target_properties( ${PROJECT_NAME}
    PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN true
        RUNTIME_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/${LIB_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/${LIB_DIR}"
)

# install
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(GDEXTOML_REL_TYPE "debug")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(GDEXTOML_REL_TYPE "release")
endif()

message( STATUS "Rel type: ${GDEXTOML_REL_TYPE}")

if(WIN32)
    set(LIB_FILE_NAME "${PROJECT_NAME}.windows.x86_64.${GDEXTOML_REL_TYPE}.dll")
elseif(APPLE)
    set(LIB_FILE_NAME "${PROJECT_NAME}.macos.${GDEXTOML_REL_TYPE}.framework")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LIB_FILE_NAME "${PROJECT_NAME}.linux.x86_64.${GDEXTOML_REL_TYPE}")
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME ${LIB_FILE_NAME})
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/godot-project/addons/${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

message( STATUS "Lib file name: ${LIB_FILE_NAME}")
message( STATUS "Install prefix: ${CMAKE_SOURCE_DIR}/godot-project/addons/${PROJECT_NAME}")

if(APPLE)
    set(GDEXTOML_INSTALL_DEST "bin/${PROJECT_NAME}.macos.${GDEXTOML_REL_TYPE}.framework")
else()
    set(GDEXTOML_INSTALL_DEST "bin")
endif()

message( STATUS "Install dest: ${GDEXTOML_INSTALL_DEST}")

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${GDEXTOML_INSTALL_DEST}
        LIBRARY DESTINATION ${GDEXTOML_INSTALL_DEST}
        PUBLIC_HEADER DESTINATION include
)

install(FILES gdex_toml.gdextension DESTINATION .)

# dummy to trick CLion into building
add_executable(dummy dummy.cpp)
install(TARGETS dummy DESTINATION "${CMAKE_SOURCE_DIR}/dummy")
