#ifndef TOML11_REGISTER_TYPES_H
#define TOML11_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void initialize_toml_parser(ModuleInitializationLevel p_level);
void uninitialize_toml_parser(ModuleInitializationLevel p_level);

#endif