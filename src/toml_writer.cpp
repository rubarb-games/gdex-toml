#include "toml_writer.h"
#include "../include/toml.hpp"
#include "translations.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

TomlWriter::TomlWriter() {
    t = toml::ordered_table();
}

TomlWriter::~TomlWriter() {
    t = nullptr;
}

void TomlWriter::_bind_methods() {
    ClassDB::bind_integer_constant("TomlWriter", "ArrayFormat", "AF_DEFAULT_FORMAT", AF_DEFAULT_FORMAT);
    ClassDB::bind_integer_constant("TomlWriter", "ArrayFormat", "AF_ONELINE", AF_ONELINE);
    ClassDB::bind_integer_constant("TomlWriter", "ArrayFormat", "AF_MULTILINE", AF_MULTILINE);
    ClassDB::bind_integer_constant("TomlWriter", "ArrayFormat", "AF_ARRAY_OF_TABLES", AF_ARRAY_OF_TABLES);

    ClassDB::bind_integer_constant("TomlWriter", "TableFormat", "TF_MULTILINE", TF_MULTILINE);
    ClassDB::bind_integer_constant("TomlWriter", "TableFormat", "TF_MULTILINE_ONELINE", TF_MULTILINE_ONELINE);
    ClassDB::bind_integer_constant("TomlWriter", "TableFormat", "TF_DOTTED", TF_DOTTED);
    ClassDB::bind_integer_constant("TomlWriter", "TableFormat", "TF_IMPLICIT", TF_IMPLICIT);
    ClassDB::bind_integer_constant("TomlWriter", "TableFormat", "TF_ONELINE", TF_ONELINE);

    ClassDB::bind_integer_constant("TomlWriter", "IndentChar", "IC_NONE", IC_NONE);
    ClassDB::bind_integer_constant("TomlWriter", "IndentChar", "IC_SPACE", IC_SPACE);
    ClassDB::bind_integer_constant("TomlWriter", "IndentChar", "IC_TAB", IC_TAB);

    ClassDB::bind_method(D_METHOD("add_int", "p_label", "p_value"), &TomlWriter::add_int);
    ClassDB::bind_method(D_METHOD("add_float", "p_label", "p_value"), &TomlWriter::add_float);
    ClassDB::bind_method(D_METHOD("add_string", "p_label", "p_value"), &TomlWriter::add_string);
    ClassDB::bind_method(D_METHOD("add_bool", "p_label", "p_value"), &TomlWriter::add_bool);
    ClassDB::bind_method(D_METHOD("add_color", "p_label", "p_value"), &TomlWriter::add_color);
    ClassDB::bind_method(D_METHOD("add_vector2", "p_label", "p_value"), &TomlWriter::add_vector2);
    ClassDB::bind_method(D_METHOD("add_vector2i", "p_label", "p_value"), &TomlWriter::add_vector2i);
    ClassDB::bind_method(D_METHOD("add_vector3", "p_label", "p_value"), &TomlWriter::add_vector3);
    ClassDB::bind_method(D_METHOD("add_vector3i", "p_label", "p_value"), &TomlWriter::add_vector3i);
    ClassDB::bind_method(D_METHOD("add_vector4", "p_label", "p_value"), &TomlWriter::add_vector4);
    ClassDB::bind_method(D_METHOD("add_vector4i", "p_label", "p_value"), &TomlWriter::add_vector4i);
    ClassDB::bind_method(D_METHOD("add_variant", "p_label", "p_value"), &TomlWriter::add_variant);
    ClassDB::bind_method(D_METHOD("add_array", "p_label", "p_value"), &TomlWriter::add_array);
    ClassDB::bind_method(D_METHOD("add_dictionary", "p_label", "p_value"), &TomlWriter::add_dictionary);

    ClassDB::bind_method(D_METHOD("add_int_to_section", "p_section", "p_label", "p_value"), &TomlWriter::add_int_to_section);
    ClassDB::bind_method(D_METHOD("add_float_to_section", "p_section", "p_label", "p_value"), &TomlWriter::add_float_to_section);
    ClassDB::bind_method(D_METHOD("add_string_to_section", "p_section", "p_label", "p_value"), &TomlWriter::add_string_to_section);
    ClassDB::bind_method(D_METHOD("add_bool_to_section", "p_section", "p_label", "p_value"), &TomlWriter::add_bool_to_section);
    ClassDB::bind_method(D_METHOD("add_color_to_section", "p_section", "p_label", "p_value"), &TomlWriter::add_color_to_section);
    ClassDB::bind_method(D_METHOD("add_vector2_to_section", "p_section", "p_label", "p_value"), &TomlWriter::add_vector2_to_section);
    ClassDB::bind_method(D_METHOD("add_vector2i_to_section", "p_section", "p_label", "p_value"), &TomlWriter::add_vector2i_to_section);
    ClassDB::bind_method(D_METHOD("add_vector3_to_section", "p_section", "p_label", "p_value"), &TomlWriter::add_vector3_to_section);
    ClassDB::bind_method(D_METHOD("add_vector3i_to_section", "p_section", "p_label", "p_value"), &TomlWriter::add_vector3i_to_section);
    ClassDB::bind_method(D_METHOD("add_vector4_to_section", "p_section", "p_label", "p_value"), &TomlWriter::add_vector4_to_section);
    ClassDB::bind_method(D_METHOD("add_vector4i_to_section", "p_section", "p_label", "p_value"), &TomlWriter::add_vector4i_to_section);
    ClassDB::bind_method(D_METHOD("add_variant_to_section", "p_section", "p_label", "p_value"), &TomlWriter::add_variant_to_section);

    ClassDB::bind_method(D_METHOD("format_array", "p_label", "p_arrfmt", "p_body_indent", "p_closing_indent"), &TomlWriter::format_array, DEFVAL(4), DEFVAL(2));
    ClassDB::bind_method(D_METHOD("format_table", "p_label", "p_tblfmt", "p_body_indent", "p_closing_indent", "p_name_indent", "p_indent_char"), &TomlWriter::format_table, DEFVAL(4), DEFVAL(2), DEFVAL(0), DEFVAL(0));
    ClassDB::bind_method(D_METHOD("serialize"), &TomlWriter::serialize);
}

void TomlWriter::add_int(const String &p_label, const int p_value) { add_value(p_label, p_value); }
void TomlWriter::add_float(const String &p_label, const float p_value) { add_value(p_label, p_value); }
void TomlWriter::add_string(const String &p_label, const String &p_value) { add_value(p_label, p_value); }
void TomlWriter::add_bool(const String &p_label, const bool p_value) { add_value(p_label, p_value); }
void TomlWriter::add_color(const String &p_label, const Color &p_value) { add_value(p_label, p_value); }
void TomlWriter::add_vector2(const String &p_label, const Vector2 &p_value) { add_value(p_label, p_value); }
void TomlWriter::add_vector2i(const String &p_label, const Vector2i &p_value) { add_value(p_label, p_value); }
void TomlWriter::add_vector3(const String &p_label, const Vector3 &p_value) { add_value(p_label, p_value); }
void TomlWriter::add_vector3i(const String &p_label, const Vector3i &p_value) { add_value(p_label, p_value); }
void TomlWriter::add_vector4(const String &p_label, const Vector4 &p_value) { add_value(p_label, p_value); }
void TomlWriter::add_vector4i(const String &p_label, const Vector4i &p_value) { add_value(p_label, p_value); }
void TomlWriter::add_variant(const String &p_label, const Variant &p_value) { add_value(p_label, p_value); }

void TomlWriter::add_array(const String &p_label, const Array &p_value) {
    toml::value arr(toml::array{});
    parse_array(arr, p_value);
    t[to_str(p_label)] = arr;
}

void TomlWriter::add_dictionary(const String &p_label, const Dictionary &p_value) {
    toml::value tbl(toml::table{});
    parse_dict(tbl, p_value);
    t[to_str(p_label)] = tbl;
}

void TomlWriter::add_int_to_section(const String &p_section, const String &p_label, const int p_value) { add_to_section(p_section, p_label, p_value); }
void TomlWriter::add_float_to_section(const String &p_section, const String &p_label, const float p_value) { add_to_section(p_section, p_label, p_value); }
void TomlWriter::add_bool_to_section(const String &p_section, const String &p_label, const bool p_value) { add_to_section(p_section, p_label, p_value); }
void TomlWriter::add_string_to_section(const String &p_section, const String &p_label, const String &p_value) { add_to_section(p_section, p_label, p_value); }
void TomlWriter::add_color_to_section(const String &p_section, const String &p_label, const Color &p_value) { add_to_section(p_section, p_label, p_value); }
void TomlWriter::add_variant_to_section(const String &p_section, const String &p_label, const Variant &p_value) { add_to_section(p_section, p_label, p_value); }
void TomlWriter::add_vector2_to_section(const String &p_section, const String &p_label, const Vector2 &p_value) { add_to_section(p_section, p_label, p_value); }
void TomlWriter::add_vector2i_to_section(const String &p_section, const String &p_label, const Vector2i &p_value) { add_to_section(p_section, p_label, p_value); }
void TomlWriter::add_vector3_to_section(const String &p_section, const String &p_label, const Vector3 &p_value) { add_to_section(p_section, p_label, p_value); }
void TomlWriter::add_vector3i_to_section(const String &p_section, const String &p_label, const Vector3i &p_value) { add_to_section(p_section, p_label, p_value); }
void TomlWriter::add_vector4_to_section(const String &p_section, const String &p_label, const Vector4 &p_value) { add_to_section(p_section, p_label, p_value); }
void TomlWriter::add_vector4i_to_section(const String &p_section, const String &p_label, const Vector4i &p_value) { add_to_section(p_section, p_label, p_value); }

void TomlWriter::add_to_section(const String &p_section, const String &p_label, const Variant &p_value) {
    const std::string sect_str = to_str(p_section);
    const std::string label_str = to_str(p_label);
    if (!t.contains(sect_str)) {
        t[to_str(p_section)] = toml::table{{to_str(p_label), enc(p_value)}};
        return;
    }
    toml::value &sect = t.at(sect_str);
    if (!sect.is_table()) {
        const String warning = String("[WARNING] TomlWriter::add_to_section - Section '{0}' is not a table").format(Array::make(p_section));
        UtilityFunctions::print(warning);
        return;
    }
    if (sect.contains(label_str)) {
        const String warning = String("[WARNING] TomlWriter::add_to_section - Section '{0}' contains a value named '{1}'").format(Array::make(p_section, p_label));
        UtilityFunctions::print(warning);
        return;
    }
    sect[to_str(p_label)] = enc(p_value);
}

void TomlWriter::add_value(const String &p_label, const Variant &p_value) {
    const std::string label_str = to_str(p_label);
    if (t.contains(label_str)) {
        const String warning = String("[WARNING] TomlWriter::add_value - TOML document already contains a value '{0}'").format(Array::make(p_label));
        UtilityFunctions::print(warning);
        return;
    }
    t[label_str] = enc(p_value);
}

void TomlWriter::parse_dict(toml::value &p_toml, const Dictionary &p_dict) {
    for (const auto &key : p_dict.keys()) {
        auto &variant = p_dict[key];
        std::string key_str = to_str(key);

        if (variant.get_type() == Variant::ARRAY) {
            toml::value arr(toml::array{});
            parse_array(arr, variant);
            p_toml[key_str] = arr;
            continue;
        }
        if (variant.get_type() == Variant::DICTIONARY) {
            toml::value tbl(toml::table{});
            parse_dict(tbl, variant);
            p_toml[key_str] = tbl;
            continue;
        }

        p_toml[key_str] = enc(variant);
    }
}

void TomlWriter::parse_array(toml::value &p_toml, const Array &p_arr) {
    for (const auto &variant : p_arr) {
        if (variant.get_type() == Variant::ARRAY) {
            toml::value arr(toml::array{});
            parse_array(arr, variant);
            p_toml.push_back(arr);
            continue;
        }
        if (variant.get_type() == Variant::DICTIONARY) {
            toml::value tbl(toml::table{});
            parse_dict(tbl, variant);
            p_toml.push_back(tbl);
            continue;
        }
        p_toml.push_back(enc(variant));
    }

}

void TomlWriter::format_array(const String &p_label, ArrayFormat p_arrfmt, int p_body_indent = 4, int p_closing_indent = 2) {
    const std::string key = to_str(p_label);

    if (!t.contains(key)) {
        const String warning = String("[WARNING] TomlWriter::format_array - Cannot find array at label '{0}'").format(Array::make(p_label));
        UtilityFunctions::print(warning);
        return;
    }

    if (!t.at(key).is_array()) {
        const String warning = String("[WARNING] TomlWriter::format_array - Entry at label '{0}' is not an array").format(Array::make(p_label));
        UtilityFunctions::print(warning);
        return;
    }

    auto&[table_format, indent_type, body_indent, closing_indent] = t.at(key).as_array_fmt();
    table_format   = static_cast<toml::array_format>(p_arrfmt);
    body_indent    = p_body_indent;
    closing_indent = p_closing_indent;
}

void TomlWriter::format_table(const String &p_label, TableFormat p_tblfmt, const int p_body_indent = 4, const int p_closing_indent = 2, const int p_name_indent = 0, IndentChar p_indent_char = IC_SPACE) {
    const std::string key = to_str(p_label);

    if (!t.contains(key)) {
        const String warning = String("[WARNING] TomlWriter::format_table - Cannot find table at label '{0}'").format(Array::make(p_label));
        UtilityFunctions::print(warning);
        return;
    }

    if (!t.at(key).is_table()) {
        const String warning = String("[WARNING] TomlWriter::format_table - Entry at label '{0}' is not a table").format(Array::make(p_label));
        UtilityFunctions::print(warning);
        return;
    }

    auto&[table_format, indent_type, body_indent, name_indent, closing_indent] = t.at(key).as_table_fmt();
    table_format   = static_cast<toml::table_format>(p_tblfmt);
    body_indent    = p_body_indent;
    closing_indent = p_closing_indent;
    indent_type    = static_cast<toml::indent_char>(p_indent_char);
    name_indent    = p_name_indent;
}

String TomlWriter::serialize() const {
    return {toml::format(t).c_str()};
}
