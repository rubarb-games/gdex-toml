cmake_minimum_required(VERSION 4.2)

project(gdex_toml)

set(CMAKE_CXX_STANDARD 20)

# Turn on link time optimization for everything
set( CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON )

if (NOT TARGET godot-cpp)
    include(FetchContent)
    FetchContent_Declare(
        godot-cpp
        GIT_REPOSITORY https://github.com/godotengine/godot-cpp
        GIT_TAG 4.4
    )
    FetchContent_MakeAvailable(godot-cpp)
endif()

add_library(${PROJECT_NAME} SHARED)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

add_subdirectory(src)
include_directories(include)

if (MSVC)
    target_compile_options(godot-cpp PRIVATE "/MP")
    target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
    set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD ON)
endif()

set( LIB_ARCH ${CMAKE_SYSTEM_PROCESSOR} )
set( LIB_DIR "${CMAKE_SYSTEM_NAME}-${LIB_ARCH}" )
message( STATUS "Building ${PROJECT_NAME} for ${LIB_ARCH} on ${CMAKE_SYSTEM_NAME}")

set( BUILD_OUTPUT_DIR "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/" )
message( STATUS "Build output dir ${BUILD_OUTPUT_DIR}")

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        godot::cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
        include
)

set_target_properties( ${PROJECT_NAME}
    PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN true
        VERSION 0.1.0
        SOVERSION 1
        RUNTIME_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/${LIB_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/${LIB_DIR}"
)

if( NOT DEFINED CMAKE_DEBUG_POSTFIX )
    set_target_properties( ${PROJECT_NAME}
        PROPERTIES
            DEBUG_POSTFIX ".debug"
    )
endif()

# install
set(GODOT_TARGET "godot-toml/addons/gdex_toml")
set(INSTALL_DIR "${PROJECT_SOURCE_DIR}/${GODOT_TARGET}")
message( STATUS "Install directory: ${INSTALL_DIR}/${LIB_DIR}")

install( TARGETS ${PROJECT_NAME}
    LIBRARY
        DESTINATION "${INSTALL_DIR}/${LIB_DIR}"
    RUNTIME
        DESTINATION "${INSTALL_DIR}/${LIB_DIR}"
)

install(FILES gdex_toml.gdextension DESTINATION ${INSTALL_DIR})


# godot-cpp
# From here: https://github.com/godotengine/godot-cpp
#if ( NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/godot-cpp/Makefile" )
#    message(
#        FATAL_ERROR
#            "[${PROJECT_NAME}] The godot-cpp submodule was not downloaded. Please update submodules: git submodule update --init --recursive."
#    )
#endif()
#
#set( GODOT_CPP_SYSTEM_HEADERS ON CACHE BOOL "" FORCE )
#add_subdirectory( godot-cpp )
#set_target_properties( godot-cpp
#    PROPERTIES
#        CXX_VISIBILITY_PRESET hidden    # visibility needs to be the same as the main library
#)
#target_link_libraries(${PROJECT_NAME} PRIVATE godot-cpp)

# new take:



# dummy to trick CLion into building
add_executable(dummy dummy.cpp)