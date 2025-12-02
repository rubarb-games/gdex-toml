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
        src/toml_writer.cpp
        src/toml_writer.h)
add_subdirectory(src)
include_directories(include)

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
        src
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
        VERSION 0.1.0
        SOVERSION 1
        RUNTIME_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/${LIB_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/${LIB_DIR}"
)

# install
if(WIN32)
    set(LIB_FILE_NAME "${PROJECT_NAME}.windows.x86_64.$<IF:$<CONFIG:Debug>,debug,release>.dll")
    set_property(TARGET ${PROJECT_NAME}
        PROPERTY OUTPUT_NAME "${PROJECT_NAME}.windows.x86_64.$<IF:$<CONFIG:Debug>,debug,release>")
elseif(APPLE)
    # don't know the extension
    set(LIB_FILE_NAME "${PROJECT_NAME}.macos.$<IF:$<CONFIG:Debug>,debug,release>")
    set_property(TARGET ${PROJECT_NAME}
        PROPERTY OUTPUT_NAME "${PROJECT_NAME}.macos.$<IF:$<CONFIG:Debug>,debug,release>")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX "")
endif()

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/godot-project/addons/${PROJECT_NAME}")

if(WIN32)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include
        )
elseif(APPLE)
    install(TARGETS ${PROJECT_NAME}
        DESTINATION bin/lib${PROJECT_NAME}.macos.$<IF:$<CONFIG:Debug>,debug,release>.framework
        PUBLIC_HEADER DESTINATION include
    )
endif()

install(FILES gdex_toml.gdextension DESTINATION .)

# work-temp is for debugging the dll while the game is running. Build the game to work-temp first.
#install(
#FILES
#    "${CMAKE_SOURCE_DIR}/godot-project/addons/${PROJECT_NAME}/bin/${LIB_FILE_NAME}"
#DESTINATION
#    "${CMAKE_SOURCE_DIR}/work-temp"
#)

# dummy to trick CLion into building
add_executable(dummy dummy.cpp)
install(TARGETS dummy DESTINATION "${CMAKE_SOURCE_DIR}/dummy")
