#ifndef GDEX_TOML_TOML_CREATOR_H
#define GDEX_TOML_TOML_CREATOR_H

#include <deque>
#include <godot_cpp/classes/ref_counted.hpp>
#include <toml.hpp>
#include "godot_cpp/core/binder_common.hpp"

using namespace godot;

class TomlCreator final : public RefCounted {
    GDCLASS(TomlCreator, RefCounted);

private:
    std::unique_ptr<toml::value> t;
    bool enable_logging;

    static void parse_dict(toml::value &p_toml, const Dictionary &p_dict);
    static void parse_array(toml::value &p_toml, const Array &p_arr);

    template<class T>
    void set_value_at(const Array &p_keys, const toml::value &p_value);

    void set_value_at(const Array &      p_keys,
                      const String &     p_label,
                      const toml::value &p_value) const;

    void set_value(const String &p_label, const Variant &p_value) const;

    bool ensure(bool condition, const std::string &message, const Array &args) const;
    void warn_if(bool condition, const std::string &message, const Array &args) const;

    template<typename T>
    T find_recursive(const toml::value& v, std::deque<std::string> keys);

protected:
    static void _bind_methods();

    void log(const String &message, const Array &args) const;

public:
    TomlCreator();
    ~TomlCreator() override;

    void logging(bool logging);

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

    // add support for comments

    void set_int(const String &p_label, int p_value) const;
    void set_float(const String &p_label, float p_value) const;
    void set_bool(const String &p_label, bool p_value) const;
    void set_string(const String &p_label, const String &p_value) const;
    void set_color(const String &p_label, const Color &p_value) const;
    void set_vec2(const String &p_label, const Vector2 &p_value) const;
    void set_vec2i(const String &p_label, const Vector2i &p_value) const;
    void set_vec3(const String &p_label, const Vector3 &p_value) const;
    void set_vec3i(const String &p_label, const Vector3i &p_value) const;
    void set_vec4(const String &p_label, const Vector4 &p_value) const;
    void set_vec4i(const String &p_label, const Vector4i &p_value) const;
    void set_variant(const String &p_label, const Variant &p_value) const;

    void set_int_at(const Array &p_keys, const String &p_label, int p_value) const;
    void set_float_at(const Array &p_keys, float p_value);
    void set_string_at(const Array &p_keys, const String &p_value);
    void set_bool_at(const Array &p_keys, bool p_value);
    void set_color_at(const Array &p_keys, const Color &p_value);
    void set_vec2_at(const Array &p_keys, const Vector2 &p_value);
    void set_vec2i_at(const Array &p_keys, const Vector2i &p_value);
    void set_vec3_at(const Array &p_keys, const Vector3 &p_value);
    void set_vec3i_at(const Array &p_keys, const Vector3i &p_value);
    void set_vec4_at(const Array &p_keys, const Vector4 &p_value);
    void set_vec4i_at(const Array &p_keys, const Vector4i &p_value);
    void set_variant_at(const Array &p_keys, const Variant &p_value);

    void set_array(const String &p_label, const Array &p_value) const;
    void set_array_at(const Array &p_keys, const Array &p_value);

    void set_table(const String &p_label, const Dictionary &p_value) const;
    void set_table_at(const Array &p_keys, const Dictionary &p_value);

    void format_array(const String &p_key, ArrayFormat p_arrfmt, int p_body_indent, int p_closing_indent) const;
    void format_table(const String &p_key, TableFormat p_tblfmt, int p_body_indent, int p_closing_indent, int p_name_indent, IndentChar p_indent_char) const;

    [[nodiscard]] String serialize() const;
};

VARIANT_ENUM_CAST(TomlCreator::ArrayFormat);
VARIANT_ENUM_CAST(TomlCreator::TableFormat);
VARIANT_ENUM_CAST(TomlCreator::IndentChar);

#endif //GDEX_TOML_TOML_CREATOR_H