#ifndef GDEX_TOML_TOML_CREATOR_H
#define GDEX_TOML_TOML_CREATOR_H

#include <godot_cpp/classes/object.hpp>
#include "../include/toml.hpp"
#include "godot_cpp/core/binder_common.hpp"

using namespace godot;

class TomlCreator final : public Object {
    GDCLASS(TomlCreator, Object);

private:
    toml::value t = toml::ordered_table{};

    static void parse_dict(toml::value &p_toml, const Dictionary &p_dict);
    static void parse_array(toml::value &p_toml, const Array &p_arr);
    void set_to_section(const String &p_section, const String &p_label, const Variant &p_value);
    void set_value(const String &p_label, const Variant &p_value);

protected:
    static void _bind_methods();

public:
    TomlCreator();
    ~TomlCreator() override;

    enum ArrayFormat {
        AF_DEFAULT_FORMAT = 0,
        AF_ONELINE = 1,
        AF_MULTILINE = 2,
        AF_ARRAY_OF_TABLES = 3,
    };

    enum TableFormat {
        TF_MULTILINE = 0,
        TF_ONELINE = 1,
        TF_DOTTED = 2,
        TF_MULTILINE_ONELINE = 3,
        TF_IMPLICIT = 4,
    };

    enum IndentChar {
        IC_SPACE = 0,
        IC_TAB = 1,
        IC_NONE = 2,
    };

    void set_int(const String &p_label, int p_value);
    void set_float(const String &p_label, float p_value);
    void set_bool(const String &p_label, bool p_value);
    void set_string(const String &p_label, const String &p_value);
    void set_color(const String &p_label, const Color &p_value);
    void set_vector2(const String &p_label, const Vector2 &p_value);
    void set_vector2i(const String &p_label, const Vector2i &p_value);
    void set_vector3(const String &p_label, const Vector3 &p_value);
    void set_vector3i(const String &p_label, const Vector3i &p_value);
    void set_vector4(const String &p_label, const Vector4 &p_value);
    void set_vector4i(const String &p_label, const Vector4i &p_value);
    void set_variant(const String &p_label, const Variant &p_value);
    void set_array(const String &p_label, const Array &p_value);

    void set_array_to_section(const String &p_section, const String &p_label, const Array &p_value);

    void set_dictionary(const String &p_label, const Dictionary &p_value);

    void set_int_to_section(const String &p_section, const String &p_label, int p_value);
    void set_float_to_section(const String &p_section, const String &p_label, float p_value);
    void set_bool_to_section(const String &p_section, const String &p_label, bool p_value);
    void set_string_to_section(const String &p_section, const String &p_label, const String &p_value);
    void set_color_to_section(const String &p_section, const String &p_label, const Color &p_value);
    void set_variant_to_section(const String &p_section, const String &p_label, const Variant &p_value);
    void set_vector2_to_section(const String &p_section, const String &p_label, const Vector2 &p_value);
    void set_vector2i_to_section(const String &p_section, const String &p_label, const Vector2i &p_value);
    void set_vector3_to_section(const String &p_section, const String &p_label, const Vector3 &p_value);
    void set_vector3i_to_section(const String &p_section, const String &p_label, const Vector3i &p_value);
    void set_vector4_to_section(const String &p_section, const String &p_label, const Vector4 &p_value);
    void set_vector4i_to_section(const String &p_section, const String &p_label, const Vector4i &p_value);

    void format_array(const String &p_label, ArrayFormat p_arrfmt, int p_body_indent, int p_closing_indent);
    void format_table(const String &p_label, TableFormat p_tblfmt, int p_body_indent, int p_closing_indent, int p_name_indent, IndentChar p_indent_char);

    String serialize() const;
};

VARIANT_ENUM_CAST(TomlCreator::ArrayFormat);
VARIANT_ENUM_CAST(TomlCreator::TableFormat);
VARIANT_ENUM_CAST(TomlCreator::IndentChar);

#endif //GDEX_TOML_TOML_CREATOR_H