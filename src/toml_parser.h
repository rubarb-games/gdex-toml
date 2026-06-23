#ifndef GD_TOML_PARSER
#define GD_TOML_PARSER

#include <godot_cpp/classes/ref_counted.hpp>
#include <toml.hpp>

using namespace godot;

class TomlParser : public RefCounted {
    GDCLASS(TomlParser, RefCounted);

private:
    std::unique_ptr<toml::value> t;
    bool enable_logging;

    static void to_dictionary(const toml::basic_value<toml::type_config> &p_table, Dictionary &p_dict);
    static void to_array(const toml::basic_value<toml::type_config> &p_value, Array &p_array);

    bool has_path(const Array &p_keys) const;
    toml::value find_recursive(const Array &p_keys) const;

    template<class T>
    TypedArray<T> FIND_TYPED_ARR(const Array &p_keys) const;

protected:
    static void _bind_methods();

    void log(const String &message, const Array &args) const;

public:
    TomlParser();
    ~TomlParser();

    void logging(bool logging);

    bool try_parse(const String &p_content);

    bool key_exists(const String &p_key) const;
    bool path_exist(const Array &p_keys) const;

    String get_string(const String &p_key) const;
    String get_string_or(const String &p_key, const String &p_default_value) const;
    String get_string_at(const Array &p_keys) const;
    TypedArray<String> get_string_arr_at(const Array &p_keys) const;

    int64_t get_int(const String &p_key) const;
    int64_t get_int_or(const String &p_key, int64_t p_default_value) const;
    int64_t get_int_at(const Array &p_keys) const;
    TypedArray<int64_t> get_int_arr_at(const Array &p_keys) const;

    // template<class T>
    // T FIND_RECURSIVE(const Array &p_keys, T p_return_val);

    float get_float(const String &p_key) const;
    float get_float_or(const String &p_key, float p_default_value) const;
    float get_float_at(const Array &p_keys) const;
    TypedArray<float> get_float_arr_at(const Array &p_keys) const;

    bool get_bool(const String &p_key) const;
    bool get_bool_or(const String &p_key, int p_default_value) const;
    bool get_bool_at(const Array &p_keys) const;
    TypedArray<bool> get_bool_arr_at(const Array &p_keys) const;

    Vector2 get_vec2(const String &p_key) const;
    Vector2 get_vec2_or(const String &p_key, Vector2 p_default_value) const;
    Vector2 get_vec2_at(const Array &p_keys) const;
    TypedArray<Vector2> get_vec2_arr_at(const Array &p_keys) const;

    Vector2i get_vec2i(const String &p_key) const;
    Vector2i get_vec2i_or(const String &p_key, Vector2i p_default_value) const;
    Vector2i get_vec2i_at(const Array &p_keys) const;
    TypedArray<Vector2i> get_vec2i_arr_at(const Array &p_keys) const;

    Vector3 get_vec3(const String &p_key) const;
    Vector3 get_vec3_or(const String &p_key, Vector3 p_default_value) const;
    Vector3 get_vec3_at(const Array &p_keys) const;
    TypedArray<Vector3> get_vec3_arr_at(const Array &p_keys) const;

    Vector3i get_vec3i(const String &p_key) const;
    Vector3i get_vec3i_or(const String &p_key, Vector3i p_default_value) const;
    Vector3i get_vec3i_at(const Array &p_keys) const;
    TypedArray<Vector3i> get_vec3i_arr_at(const Array &p_keys) const;

    Color get_color(const String &p_key) const;
    Color get_color_or(const String &p_key, Color p_default_value) const;
    Color get_color_at(const Array &p_keys) const;
    TypedArray<Color> get_color_arr_at(const Array &p_keys) const;

    Dictionary get_table(const String &p_key) const;
    Dictionary get_table_at(const Array &p_keys) const;
    TypedArray<String> get_table_keys(const String &p_key) const;

    Array get_array(const String &p_key) const;
    Array get_array_at(const Array &p_keys) const;
};

#endif