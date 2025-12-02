#include "toml_writer.h"
#include "../include/toml.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

TomlWriter::TomlWriter() = default;
TomlWriter::~TomlWriter() = default;

void TomlWriter::_bind_methods() {
    ClassDB::bind_method(D_METHOD("add_int", "p_label", "p_value"), &TomlWriter::add_int);
    ClassDB::bind_method(D_METHOD("write_to_string"), &TomlWriter::write_to_string);
}

std::string TomlWriter::to_str(const String &p_string) {
    return p_string.utf8().get_data();
}

void TomlWriter::add_int(const String &p_label, int p_value) {
    t[to_str(p_label)] = toml::value{p_value};
}

void TomlWriter::add_float(const String &p_label, float p_value) {
}

void TomlWriter::add_string(const String &p_label, const String &p_value) {
}

void TomlWriter::add_color(const String &p_label, const Color &p_value) {
}

void TomlWriter::add_vector2(const String &p_label, const Vector2 &p_value) {
}

String TomlWriter::write_to_string() {
    return {toml::format(t).c_str()};
}
