target_sources( ${PROJECT_NAME}
    PRIVATE
        toml_parser.h
        toml_parser.cpp
        toml_writer.h
        toml_writer.cpp
        register_types.h
        register_types.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE
        "src"
)