target_sources( ${PROJECT_NAME}
    PRIVATE
        translations.h
        toml_parser.h
        toml_parser.cpp
        toml_creator.h
        toml_creator.cpp
        register_types.h
        register_types.cpp
)

#target_include_directories( ${PROJECT_NAME}
#    PRIVATE
#        "src"
#)