#ifndef GD_TOML_PARSER
#define GD_TOML_PARSER

#include <godot_cpp/classes/object.hpp>
#include "../include/toml.hpp"

using namespace godot;

class TomlParser final : public Object {
    GDCLASS(TomlParser, Object);

private:
    toml::value t;

    static std::string to_str(const String &p_string);
    static Dictionary to_dictionary(const toml::value &val);

protected:
    static void _bind_methods();

public:
    TomlParser();
    ~TomlParser();

    bool try_parse(const String &p_content);
    String get_str(const String &p_key);
    Array get_arr(const String &p_key);
    Dictionary get_section(const String &p_section_name);
    Array get_tables(const String &p_key);
};

#endif