#ifndef GDEX_TOML_TOML_WRITER_H
#define GDEX_TOML_TOML_WRITER_H

#include <godot_cpp/classes/object.hpp>
#include "../include/toml.hpp"

using namespace godot;

class TomlWriter final : public Object {
    GDCLASS(TomlWriter, Object);

private:
    toml::value t;

    static std::string to_str(const String &p_string);
    // static Dictionary to_dictionary(const toml::value &val);

protected:
    static void _bind_methods();

public:
    TomlWriter();
    ~TomlWriter() override;

    // bool try_parse(const String &p_content);
    // String get_str(const String &p_key);
    // Array get_arr(const String &p_key);
    // Dictionary get_section(const String &p_section_name);
    // Array get_tables(const String &p_key);
    //
    // static String format(const String &p_label, const Variant &p_value);
    // static String format_table(const String &p_label, const Dictionary &p_dict);


    // add overrides for when you want to append a value to a section
    // no need to fiddly daddly with objects and whatnot. at least not for now
    void add_int(const String &p_label, int p_value);
    void add_float(const String &p_label, float p_value);
    void add_string(const String &p_label, const String &p_value);
    void add_color(const String &p_label, const Color &p_value);
    void add_vector2(const String &p_label, const Vector2 &p_value);
    //v2i
    //v3
    //v3i
    // variant
    // array
    // dict

    String write_to_string();
};
#endif //GDEX_TOML_TOML_WRITER_H