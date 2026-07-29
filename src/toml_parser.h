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

    static int8_t to_string(const toml::basic_value<toml::type_config> &p_value,
                            String &                                    p_string);

    static int8_t to_int(const toml::basic_value<toml::type_config> &p_value,
                         int64_t &                                   p_int);

    static int8_t to_float(const toml::basic_value<toml::type_config> &p_value,
                           float &                                     p_float);

    static int8_t to_bool(const toml::basic_value<toml::type_config> &p_value,
                          bool &                                      p_bool);

    static int8_t to_color(const toml::basic_value<toml::type_config> &p_value,
                           Color &                                     p_color);

    static int8_t to_vec2(const toml::basic_value<toml::type_config> &p_value,
                          Vector2 &                                   p_vec2);

    static int8_t to_vec2i(const toml::basic_value<toml::type_config> &p_value,
                           Vector2i &                                  p_vec2i);

    static int8_t to_vec3(const toml::basic_value<toml::type_config> &p_value,
                          Vector3 &                                   p_vec3);

    static int8_t to_vec3i(const toml::basic_value<toml::type_config> &p_value, Vector3i &p_vec3i);

    int8_t get_node(const String &p_key, toml::value &node) const;
    int8_t get_node(const Array &p_keys, toml::value &node) const;

    [[nodiscard]] bool has_path(const Array &p_keys) const;
    static String path_as_string(const Array &p_keys);
    [[nodiscard]] toml::value find_recursive(const Array &p_keys) const;

    template<class T>
    TypedArray<T> FIND_TYPED_ARR(const Array &p_keys) const;

protected:
    static void _bind_methods();

    void log(const String &message, const Array &args) const;
    void log_error_code(int8_t code) const;

public:
    TomlParser();
    ~TomlParser();

    void logging(bool logging);

    bool try_parse(const String &p_content);

    [[nodiscard]] bool key_exists(const String &p_key) const;
    [[nodiscard]] bool path_exists(const Array &p_keys) const;

    [[nodiscard]] String get_string(const String &p_key) const;
    [[nodiscard]] String get_string_or(const String &p_key, const String &p_default_value = String{}) const;
    [[nodiscard]] String get_string_at(const Array &p_keys) const;
    [[nodiscard]] String get_string_at_or(const Array &p_keys, const String &p_default_value = String{}) const;
    [[nodiscard]] TypedArray<String> get_string_arr(const String &p_key) const;
    [[nodiscard]] TypedArray<String> get_string_arr_at(const Array &p_keys) const;

    [[nodiscard]] int64_t get_int(const String &p_key) const;
    [[nodiscard]] int64_t get_int_or(const String &p_key, const int64_t &p_default_value = 0) const;
    [[nodiscard]] int64_t get_int_at(const Array &p_keys) const;
    [[nodiscard]] int64_t get_int_at_or(const Array &p_keys, const int64_t &p_default_value = 0) const;
    [[nodiscard]] TypedArray<int64_t> get_int_arr(const String &p_key) const;
    [[nodiscard]] TypedArray<int64_t> get_int_arr_at(const Array &p_keys) const;

    [[nodiscard]] float get_float(const String &p_key) const;
    [[nodiscard]] float get_float_or(const String &p_key, const float &p_default_value = 0.0) const;
    [[nodiscard]] float get_float_at(const Array &p_keys) const;
    [[nodiscard]] float get_float_at_or(const Array &p_keys, const float &p_default_value = 0.0) const;
    [[nodiscard]] TypedArray<float> get_float_arr(const String &p_key) const;
    [[nodiscard]] TypedArray<float> get_float_arr_at(const Array &p_keys) const;

    [[nodiscard]] bool get_bool(const String &p_key) const;
    [[nodiscard]] bool get_bool_or(const String &p_key, const bool &p_default_value = false) const;
    [[nodiscard]] bool get_bool_at(const Array &p_keys) const;
    [[nodiscard]] bool get_bool_at_or(const Array &p_keys, const bool &p_default_value = false) const;
    [[nodiscard]] TypedArray<bool> get_bool_arr(const String &p_key) const;
    [[nodiscard]] TypedArray<bool> get_bool_arr_at(const Array &p_keys) const;

    Vector2 get_vec2(const String &p_key) const;
    Vector2 get_vec2_or(const String &p_key, const Vector2& p_default_value = Vector2{0.0,0.0}) const;
    Vector2 get_vec2_at(const Array &p_keys) const;
    Vector2 get_vec2_at_or(const Array &p_keys, const Vector2& p_default_value = Vector2{0.0,0.0}) const;
    [[nodiscard]] TypedArray<Vector2> get_vec2_arr(const String &p_key) const;
    [[nodiscard]] TypedArray<Vector2> get_vec2_arr_at(const Array &p_keys) const;

    Vector2i get_vec2i(const String &p_key) const;
    Vector2i get_vec2i_or(const String &p_key, const Vector2i &p_default_value = Vector2{0,0}) const;
    Vector2i get_vec2i_at(const Array &p_keys) const;
    Vector2i get_vec2i_at_or(const Array &p_keys, const Vector2i &p_default_value = Vector2{0,0}) const;
    [[nodiscard]] TypedArray<Vector2i> get_vec2i_arr(const String &p_key) const;
    [[nodiscard]] TypedArray<Vector2i> get_vec2i_arr_at(const Array &p_keys) const;

    Vector3 get_vec3(const String &p_key) const;
    Vector3 get_vec3_or(const String &p_key,
                        const Vector3 &p_default_value = Vector3(0.0, 0.0, 0.0)) const;
    Vector3 get_vec3_at(const Array &p_keys) const;
    Vector3 get_vec3_at_or(const Array &p_keys,
                           const Vector3 &p_default_value = Vector3(0.0, 0.0, 0.0)) const;
    [[nodiscard]] TypedArray<Vector3> get_vec3_arr(const String &p_key) const;
    [[nodiscard]] TypedArray<Vector3> get_vec3_arr_at(const Array &p_keys) const;

    Vector3i get_vec3i(const String &p_key) const;
    Vector3i get_vec3i_or(const String &p_key, const Vector3i& p_default_value = Vector3i{}) const;
    Vector3i get_vec3i_at(const Array &p_keys) const;
    Vector3i get_vec3i_at_or(const Array &p_keys, const Vector3i &p_default_value = Vector3i{}) const;
    [[nodiscard]] TypedArray<Vector3i> get_vec3i_arr(const String &p_key) const;
    [[nodiscard]] TypedArray<Vector3i> get_vec3i_arr_at(const Array &p_keys) const;

    Color get_color(const String &p_key) const;
    Color get_color_or(const String &p_key, const Color& p_default_value = Color{0,0,0,0}) const;
    Color get_color_at(const Array &p_keys) const;
    Color get_color_at_or(const Array &p_keys, const Color& p_default_value = Color{0,0,0,0}) const;
    [[nodiscard]] TypedArray<Color> get_color_arr(const String &p_key) const;
    [[nodiscard]] TypedArray<Color> get_color_arr_at(const Array &p_keys) const;

    [[nodiscard]] Dictionary get_table(const String &p_key) const;
    [[nodiscard]] Dictionary get_table_at(const Array &p_keys) const;
    [[nodiscard]] TypedArray<String> get_table_keys(const String &p_key) const;

    [[nodiscard]] Array get_array(const String &p_key) const;
    [[nodiscard]] Array get_array_at(const Array &p_keys) const;
};

#endif