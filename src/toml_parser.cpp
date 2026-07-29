#include "toml_parser.h"

#include <cfloat>
#include <ranges>
#include <toml.hpp>
#include "translations.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// const int8_t OK = 0;
constexpr int8_t ERR_INCORRECT_TYPE = 1;
constexpr int8_t ERR_INCORRECT_FORMAT = 2;
constexpr int8_t ERR_KEY_EMPTY = 10;
constexpr int8_t ERR_KEY_NOT_FOUND = 11;
constexpr int8_t ERR_KEY_MISSING = 12;
constexpr int8_t ERR_NODE_EMPTY = 20;

void TomlParser::_bind_methods() {
    ClassDB::bind_method(D_METHOD("logging", "logging"), &TomlParser::logging);
    ClassDB::bind_method(D_METHOD("try_parse", "p_content"), &TomlParser::try_parse);

    ClassDB::bind_method(D_METHOD("get_string", "p_key"), &TomlParser::get_string);
    ClassDB::bind_method(D_METHOD("get_string_or", "p_key", "p_default_value"), &TomlParser::get_string_or, DEFVAL(""));
    ClassDB::bind_method(D_METHOD("get_string_at", "p_keys"), &TomlParser::get_string_at);
    ClassDB::bind_method(D_METHOD("get_string_at_or", "p_keys", "p_default_value"), &TomlParser::get_string_at_or, DEFVAL(""));
    ClassDB::bind_method(D_METHOD("get_string_arr", "p_key"), &TomlParser::get_string_arr);
    ClassDB::bind_method(D_METHOD("get_string_arr_at", "p_keys"), &TomlParser::get_string_arr_at);

    ClassDB::bind_method(D_METHOD("get_int", "p_key"), &TomlParser::get_int);
    ClassDB::bind_method(D_METHOD("get_int_or", "p_key", "p_default_value"), &TomlParser::get_int_or, DEFVAL(0));
    ClassDB::bind_method(D_METHOD("get_int_at", "p_keys"), &TomlParser::get_int_at);
    ClassDB::bind_method(D_METHOD("get_int_at_or", "p_keys", "p_default_value"), &TomlParser::get_int_at_or, DEFVAL(0));
    ClassDB::bind_method(D_METHOD("get_int_arr", "p_key"), &TomlParser::get_int_arr);
    ClassDB::bind_method(D_METHOD("get_int_arr_at", "p_keys"), &TomlParser::get_int_arr_at);

    ClassDB::bind_method(D_METHOD("get_float", "p_key"), &TomlParser::get_float);
    ClassDB::bind_method(D_METHOD("get_float_or", "p_key", "p_default_value"), &TomlParser::get_float_or, DEFVAL(0.0));
    ClassDB::bind_method(D_METHOD("get_float_at", "p_keys"), &TomlParser::get_float_at);
    ClassDB::bind_method(D_METHOD("get_float_at_or", "p_keys", "p_default_value"), &TomlParser::get_float_at_or, DEFVAL(0.0));
    ClassDB::bind_method(D_METHOD("get_float_arr", "p_keys"), &TomlParser::get_float_arr);
    ClassDB::bind_method(D_METHOD("get_float_arr_at", "p_keys"), &TomlParser::get_float_arr_at);

    ClassDB::bind_method(D_METHOD("get_bool", "p_key"), &TomlParser::get_bool);
    ClassDB::bind_method(D_METHOD("get_bool_or", "p_key", "p_default_value"), &TomlParser::get_bool_or, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("get_bool_at", "p_keys"), &TomlParser::get_bool_at);
    ClassDB::bind_method(D_METHOD("get_bool_at_or", "p_keys", "p_default_value"), &TomlParser::get_bool_at_or, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("get_bool_arr", "p_key"), &TomlParser::get_bool_arr);
    ClassDB::bind_method(D_METHOD("get_bool_arr_at", "p_keys"), &TomlParser::get_bool_arr_at);

    ClassDB::bind_method(D_METHOD("get_vec2", "p_key"), &TomlParser::get_vec2);
    ClassDB::bind_method(D_METHOD("get_vec2_or", "p_key", "p_default_value"), &TomlParser::get_vec2_or, DEFVAL(Vector2{}));
    ClassDB::bind_method(D_METHOD("get_vec2_at", "p_keys"), &TomlParser::get_vec2_at);
    ClassDB::bind_method(D_METHOD("get_vec2_at_or", "p_keys", "p_default_value"), &TomlParser::get_vec2_at_or, DEFVAL(Vector2{}));
    ClassDB::bind_method(D_METHOD("get_vec2_arr", "p_keys"), &TomlParser::get_vec2_arr);
    ClassDB::bind_method(D_METHOD("get_vec2_arr_at", "p_keys"), &TomlParser::get_vec2_arr_at);

    ClassDB::bind_method(D_METHOD("get_vec2i", "p_key"), &TomlParser::get_vec2i);
    ClassDB::bind_method(D_METHOD("get_vec2i_or", "p_key", "p_default_value"), &TomlParser::get_vec2i_or, DEFVAL(Vector2i{}));
    ClassDB::bind_method(D_METHOD("get_vec2i_at", "p_keys"), &TomlParser::get_vec2i_at);
    ClassDB::bind_method(D_METHOD("get_vec2i_at_or", "p_keys", "p_default_value"), &TomlParser::get_vec2i_at_or, DEFVAL(Vector2i{}));
    ClassDB::bind_method(D_METHOD("get_vec2i_arr", "p_key"), &TomlParser::get_vec2i_arr);
    ClassDB::bind_method(D_METHOD("get_vec2i_arr_at", "p_keys"), &TomlParser::get_vec2i_arr_at);

    ClassDB::bind_method(D_METHOD("get_vec3", "p_key"), &TomlParser::get_vec3);
    ClassDB::bind_method(D_METHOD("get_vec3_or", "p_key", "p_default_value"), &TomlParser::get_vec3_or, DEFVAL(Vector3{}));
    ClassDB::bind_method(D_METHOD("get_vec3_at", "p_keys"), &TomlParser::get_vec3_at);
    ClassDB::bind_method(D_METHOD("get_vec3_at_or", "p_keys", "p_default_value"), &TomlParser::get_vec3_at_or, DEFVAL(Vector3{}));
    ClassDB::bind_method(D_METHOD("get_vec3_arr", "p_key"), &TomlParser::get_vec3_arr);
    ClassDB::bind_method(D_METHOD("get_vec3_arr_at", "p_keys"), &TomlParser::get_vec3_arr_at);

    ClassDB::bind_method(D_METHOD("get_vec3i", "p_key"), &TomlParser::get_vec3i);
    ClassDB::bind_method(D_METHOD("get_vec3i_or", "p_key", "p_default_value"), &TomlParser::get_vec3i_or, DEFVAL(Vector3i{}));
    ClassDB::bind_method(D_METHOD("get_vec3i_at", "p_keys"), &TomlParser::get_vec3i_at);
    ClassDB::bind_method(D_METHOD("get_vec3i_at_or", "p_keys", "p_default_value"), &TomlParser::get_vec3i_at_or, DEFVAL(Vector3i{}));
    ClassDB::bind_method(D_METHOD("get_vec3i_arr", "p_key"), &TomlParser::get_vec3i_arr);
    ClassDB::bind_method(D_METHOD("get_vec3i_arr_at", "p_keys"), &TomlParser::get_vec3i_arr_at);

    ClassDB::bind_method(D_METHOD("get_color", "p_key"), &TomlParser::get_color);
    ClassDB::bind_method(D_METHOD("get_color_or", "p_key", "p_default_value"), &TomlParser::get_color_or, DEFVAL(Color{}));
    ClassDB::bind_method(D_METHOD("get_color_at", "p_keys"), &TomlParser::get_color_at);
    ClassDB::bind_method(D_METHOD("get_color_at_or", "p_keys", "p_default_value"), &TomlParser::get_color_at_or, DEFVAL(Color{}));
    ClassDB::bind_method(D_METHOD("get_color_arr", "p_key"), &TomlParser::get_color_arr);
    ClassDB::bind_method(D_METHOD("get_color_arr_at", "p_keys"), &TomlParser::get_color_arr_at);

    ClassDB::bind_method(D_METHOD("get_table", "p_key"), &TomlParser::get_table);
    ClassDB::bind_method(D_METHOD("get_table_at", "p_keys"), &TomlParser::get_table_at);
    ClassDB::bind_method(D_METHOD("get_table_keys", "p_key"), &TomlParser::get_table_keys);

    ClassDB::bind_method(D_METHOD("get_array", "p_key"), &TomlParser::get_array);
    ClassDB::bind_method(D_METHOD("get_array_at", "p_keys"), &TomlParser::get_array_at);

    ClassDB::bind_method(D_METHOD("key_exists", "p_key"), &TomlParser::key_exists);
    ClassDB::bind_method(D_METHOD("path_exists", "p_keys"), &TomlParser::path_exists);
}

TomlParser::TomlParser() {
    // disable logging in release build
    enable_logging = true;
    t = std::make_unique<toml::value>();
}

TomlParser::~TomlParser() {
    // auto v = t.release();
    // v = nullptr;
}

void TomlParser::logging(bool logging) {
    enable_logging = logging;
}

void TomlParser::log(const String &message, const Array &args = {}) const {
    if (enable_logging) {
        UtilityFunctions::print(message.format(args));
    }
}

void TomlParser::log_error_code(int8_t code) const {
    switch (code) {
        case ERR_INCORRECT_TYPE:
            log("[ERROR] TomlParser :: Incorrect type");
            break;
        case ERR_KEY_EMPTY:
            log("[ERROR] TomlParser :: Empty key");
            break;
        case ERR_KEY_NOT_FOUND:
            log("[ERROR] TomlParser :: Key not found");
            break;
        case ERR_KEY_MISSING:
            log("[ERROR] TomlParser :: Missing key");
            break;
        case ERR_NODE_EMPTY:
            log("[ERROR] TomlParser :: Node is empty");
            break;
        default:
            break;
    }
}

bool TomlParser::try_parse(const String &p_content) {
    // what happens if you try to parse a new string and t is not null?
    const auto parse_result = toml::try_parse_str(to_str(p_content));

    if (parse_result.is_ok()) {
        t = std::make_unique<toml::value>(parse_result.unwrap());
        // t = parse_result.unwrap();
        return true;
    }

    const auto err = parse_result.unwrap_err().at(0);
    log(String(format_error(err).c_str()));
    return false;
}

bool TomlParser::key_exists(const String &p_key) const {
    const auto &doc = *t;
    return doc.contains(to_str(p_key));
}

bool TomlParser::path_exists(const Array &p_keys) const {
    return has_path(p_keys);
}



// String
// #############################################################################


String TomlParser::get_string(const String &p_key) const {
    return get_string_or(p_key);
}

String TomlParser::get_string_or(const String &p_key, const String &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return default {1}.", { p_key, p_default_value });
        return p_default_value;
    }
    String str;
    if (const int8_t code = to_string(node, str); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at key '{0}' to String. Return empty string.", { p_key });
        return {};
    }
    return str;
}

String TomlParser::get_string_at(const Array &p_keys) const {
    return get_string_at_or(p_keys);
}

String TomlParser::get_string_at_or(const Array &p_keys, const String &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return default {1}.", { path_as_string(p_keys), p_default_value });
        return p_default_value;
    }
    String str;
    if (const int8_t code = to_string(node, str); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at path '{0}' to String. Return empty string.", { path_as_string(p_keys) });
        return {};
    }
    return str;
}

TypedArray<String> TomlParser::get_string_arr(const String &p_key) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return empty Array[String].", { p_key });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at key '{0}' is not an array. Return empty Array[String].", { p_key });
        return {};
    }

    TypedArray<String> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        String str;
        if (const int8_t code = to_string(value, str); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at key '{1}' to String. Element is ignored.", {elm, p_key});
            continue;
        }
        result.append(str);
        elm++;
    }
    return result;
}

TypedArray<String> TomlParser::get_string_arr_at(const Array &p_keys) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return empty Array[String].", { path_as_string(p_keys) });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at path '{0}' is not an array. Return empty Array[String].", { path_as_string(p_keys) });
        return {};
    }

    TypedArray<String> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        String str;
        if (const int8_t code = to_string(value, str); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at path '{1}' to String. Element is ignored.", {elm, path_as_string(p_keys)});
            continue;
        }
        result.append(str);
        elm++;
    }
    return result;
}


// int
// #############################################################################


int64_t TomlParser::get_int(const String &p_key) const {
    return get_int_or(p_key);
}

int64_t TomlParser::get_int_or(const String &p_key, const int64_t &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return default {1}.", { p_key, p_default_value });
        return p_default_value;
    }
    int64_t i;
    if (const int8_t code = to_int(node, i); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at key '{0}' to int. Return 0.", { p_key });
        return {};
    }
    return i;
}

int64_t TomlParser::get_int_at(const Array &p_keys) const {
    return get_int_at_or(p_keys);
}

int64_t TomlParser::get_int_at_or(const Array &p_keys, const int64_t &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return default {1}.", { path_as_string(p_keys), p_default_value });
        return p_default_value;
    }
    int64_t i;
    if (const int8_t code = to_int(node, i); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at path '{0}' to int. Return 0.", { path_as_string(p_keys) });
        return {};
    }
    return i;
}

TypedArray<int64_t> TomlParser::get_int_arr(const String &p_key) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return empty Array[int].", { p_key });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at key '{0}' is not an array. Return empty Array[int].", { p_key });
        return {};
    }

    TypedArray<int64_t> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        int64_t i;
        if (const int8_t code = to_int(value, i); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at key '{1}' to int. Element is ignored.", {elm, p_key});
            continue;
        }
        result.append(i);
        elm++;
    }
    return result;
}

TypedArray<int64_t> TomlParser::get_int_arr_at(const Array &p_keys) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return empty Array[int].", { path_as_string(p_keys) });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at path '{0}' is not an array. Return empty Array[int].", { path_as_string(p_keys) });
        return {};
    }

    TypedArray<int64_t> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        int64_t i;
        if (const int8_t code = to_int(value, i); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at path '{1}' to int. Element is ignored.", {elm, path_as_string(p_keys)});
            continue;
        }
        result.append(i);
        elm++;
    }
    return result;
}


// float
// #############################################################################


float TomlParser::get_float(const String &p_key) const {
    return get_float_or(p_key);
}

float TomlParser::get_float_or(const String &p_key, const float &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return default {1}.", { p_key, p_default_value });
        return p_default_value;
    }
    float f;
    if (const int8_t code = to_float(node, f); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at key '{0}' to float. Return false.", { p_key });
        return {};
    }
    return f;
}

float TomlParser::get_float_at(const Array &p_keys) const {
    return get_float_at_or(p_keys);
}

float TomlParser::get_float_at_or(const Array &p_keys, const float &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return default {1}.", { path_as_string(p_keys), p_default_value });
        return p_default_value;
    }
    float f;
    if (const int8_t code = to_float(node, f); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at path '{0}' to float. Return false.", { path_as_string(p_keys) });
        return {};
    }
    return f;
}

TypedArray<float> TomlParser::get_float_arr(const String &p_key) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return empty Array[float].", { p_key });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at key '{0}' is not an array. Return empty Array[float].", { p_key });
        return {};
    }

    TypedArray<float> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        float f;
        if (const int8_t code = to_float(value, f); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at key '{1}' to float. Element is ignored.", {elm, p_key});
            continue;
        }
        result.append(f);
        elm++;
    }
    return result;
}

TypedArray<float> TomlParser::get_float_arr_at(const Array &p_keys) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return empty Array[float].", { path_as_string(p_keys) });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at path '{0}' is not an array. Return empty Array[float].", { path_as_string(p_keys) });
        return {};
    }

    TypedArray<float> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        float f;
        if (const int8_t code = to_float(value, f); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at path '{1}' to float. Element is ignored.", {elm, path_as_string(p_keys)});
            continue;
        }
        result.append(f);
        elm++;
    }
    return result;
}


// bool
// #############################################################################


bool TomlParser::get_bool(const String &p_key) const {
    return get_bool_or(p_key);
}

bool TomlParser::get_bool_or(const String &p_key, const bool &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return default {1}.", { p_key, p_default_value });
        return p_default_value;
    }
    bool b;
    if (const int8_t code = to_bool(node, b); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at key '{0}' to bool. Return false.", { p_key });
        return {};
    }
    return b;
}

bool TomlParser::get_bool_at(const Array &p_keys) const {
    return get_bool_at_or(p_keys);
}

bool TomlParser::get_bool_at_or(const Array &p_keys, const bool &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return default {1}.", { path_as_string(p_keys), p_default_value });
        return p_default_value;
    }
    bool b;
    if (const int8_t code = to_bool(node, b); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at path '{0}' to bool. Return false.", { path_as_string(p_keys) });
        return {};
    }
    return b;
}

TypedArray<bool> TomlParser::get_bool_arr(const String &p_key) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return empty Array[bool].", { p_key });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at key '{0}' is not an array. Return empty Array[bool].", { p_key });
        return {};
    }

    TypedArray<bool> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        bool vec;
        if (const int8_t code = to_bool(value, vec); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at key '{1}' to bool. Element is ignored.", {elm, p_key});
            continue;
        }
        result.append(vec);
        elm++;
    }
    return result;
}

TypedArray<bool> TomlParser::get_bool_arr_at(const Array &p_keys) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return empty Array[bool].", { path_as_string(p_keys) });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at path '{0}' is not an array. Return empty Array[bool].", { path_as_string(p_keys) });
        return {};
    }

    TypedArray<bool> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        bool vec;
        if (const int8_t code = to_bool(value, vec); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at path '{1}' to bool. Element is ignored.", {elm, path_as_string(p_keys)});
            continue;
        }
        result.append(vec);
        elm++;
    }
    return result;
}


// Vector2
// #############################################################################


Vector2 TomlParser::get_vec2(const String &p_key) const {
    return get_vec2_or(p_key);
}

Vector2 TomlParser::get_vec2_or(const String &p_key, const Vector2 &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return default {1}.", { p_key, p_default_value });
        return p_default_value;
    }
    Vector2 vec;
    if (const int8_t code = to_vec2(node, vec); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at key '{0}' to Vector2. Return Vector2.ZERO.", { p_key });
        return {};
    }
    return vec;
}

Vector2 TomlParser::get_vec2_at(const Array &p_keys) const {
    return get_vec2_at_or(p_keys);
}

Vector2 TomlParser::get_vec2_at_or(const Array &p_keys, const Vector2 &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return default {1}.", { path_as_string(p_keys), p_default_value });
        return p_default_value;
    }
    Vector2 vec;
    if (const int8_t code = to_vec2(node, vec); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at path '{0}' to Vector2. Return Vector2.ZERO.", { path_as_string(p_keys) });
        return {};
    }
    return vec;
}

TypedArray<Vector2> TomlParser::get_vec2_arr(const String &p_key) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return empty Array[Vector2].", { p_key });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at key '{0}' is not an array. Return empty Array[Vector2].", { p_key });
        return {};
    }

    TypedArray<Vector2> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        Vector2 vec;
        if (const int8_t code = to_vec2(value, vec); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at key '{1}' to Vector2. Element is ignored.", {elm, p_key});
            continue;
        }
        result.append(vec);
        elm++;
    }
    return result;
}

TypedArray<Vector2> TomlParser::get_vec2_arr_at(const Array &p_keys) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return empty Array[Vector2].", { path_as_string(p_keys) });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at path '{0}' is not an array. Return empty Array[Vector2].", { path_as_string(p_keys) });
        return {};
    }

    TypedArray<Vector2> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        Vector2 vec;
        if (const int8_t code = to_vec2(value, vec); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at path '{1}' to Vector2. Element is ignored.", {elm, path_as_string(p_keys)});
            continue;
        }
        result.append(vec);
        elm++;
    }
    return result;
}



// Vector2i
// #############################################################################


Vector2i TomlParser::get_vec2i(const String &p_key) const {
    return get_vec2i_or(p_key);
}

Vector2i TomlParser::get_vec2i_or(const String &p_key, const Vector2i &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return default {1}.", { p_key, p_default_value });
        return p_default_value;
    }
    Vector2i vec;
    if (const int8_t code = to_vec2i(node, vec); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at key '{0}' to Vector2i. Return Vector2i.ZERO.", { p_key });
        return {};
    }
    return vec;
}

Vector2i TomlParser::get_vec2i_at(const Array &p_keys) const {
    return get_vec2i_at_or(p_keys);
}

Vector2i TomlParser::get_vec2i_at_or(const Array &p_keys, const Vector2i &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return default {1}.", { path_as_string(p_keys), p_default_value });
        return p_default_value;
    }
    Vector2i vec;
    if (const int8_t code = to_vec2i(node, vec); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at path '{0}' to Vector2i. Return Vector2i.ZERO.", { path_as_string(p_keys) });
        return {};
    }
    return vec;
}

TypedArray<Vector2i> TomlParser::get_vec2i_arr(const String &p_key) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return empty Array[Vector2i].", { p_key });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at key '{0}' is not an array. Return empty Array[Vector2i].", { p_key });
        return {};
    }

    TypedArray<Vector2i> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        Vector2i vec;
        if (const int8_t code = to_vec2i(value, vec); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at key '{1}' to Vector2i. Element is ignored.", {elm, p_key});
            continue;
        }
        result.append(vec);
        elm++;
    }
    return result;
}

TypedArray<Vector2i> TomlParser::get_vec2i_arr_at(const Array &p_keys) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return empty Array[Vector2i].", { path_as_string(p_keys) });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at path '{0}' is not an array. Return empty Array[Vector2i].", { path_as_string(p_keys) });
        return {};
    }

    TypedArray<Vector2i> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        Vector2i vec;
        if (const int8_t code = to_vec2i(value, vec); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at path '{1}' to Vector2i. Element is ignored.", {elm, path_as_string(p_keys)});
            continue;
        }
        result.append(vec);
        elm++;
    }
    return result;
}


// Vector3
// #############################################################################


Vector3 TomlParser::get_vec3(const String &p_key) const {
    return get_vec3_or(p_key);
}

Vector3 TomlParser::get_vec3_or(const String &p_key, const Vector3 &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return default {1}.", { p_key, p_default_value });
        return p_default_value;
    }
    Vector3 vec;
    if (const int8_t code = to_vec3(node, vec); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at key '{0}' to Vector3. Return Vector3.ZERO.", { p_key });
        return {};
    }
    return vec;
}

Vector3 TomlParser::get_vec3_at(const Array &p_keys) const {
    return get_vec3_at_or(p_keys);
}

Vector3 TomlParser::get_vec3_at_or(const Array &p_keys, const Vector3 &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return default {1}.", { path_as_string(p_keys), p_default_value });
        return p_default_value;
    }
    Vector3 vec;
    if (const int8_t code = to_vec3(node, vec); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at path '{0}' to Vector3. Return Vector3.ZERO.", { path_as_string(p_keys) });
        return {};
    }
    return vec;
}

TypedArray<Vector3> TomlParser::get_vec3_arr(const String &p_key) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return empty Array[Vector3].", { p_key });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at key '{0}' is not an array. Return empty Array[Vector3].", { p_key });
        return {};
    }

    TypedArray<Vector3> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        Vector3 vec;
        if (const int8_t code = to_vec3(value, vec); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at key '{1}' to Vector3. Element is ignored.", {elm, p_key});
            continue;
        }
        result.append(vec);
        elm++;
    }
    return result;
}

TypedArray<Vector3> TomlParser::get_vec3_arr_at(const Array &p_keys) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return empty Array[Vector3].", { path_as_string(p_keys) });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at path '{0}' is not an array. Return empty Array[Vector3].", { path_as_string(p_keys) });
        return {};
    }

    TypedArray<Vector3> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        Vector3 vec;
        if (const int8_t code = to_vec3(value, vec); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at path '{1}' to Vector3. Element is ignored.", {elm, path_as_string(p_keys)});
            continue;
        }
        result.append(vec);
        elm++;
    }
    return result;
}


// Vector3i
// #############################################################################


Vector3i TomlParser::get_vec3i(const String &p_key) const {
    return get_vec3i_or(p_key);
}

Vector3i TomlParser::get_vec3i_or(const String &p_key, const Vector3i &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return default {1}.", { p_key, p_default_value });
        return p_default_value;
    }
    Vector3i vec;
    if (const int8_t code = to_vec3i(node, vec); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at key '{0}' to Vector3i. Return Vector3i.ZERO.", { p_key });
        return {};
    }
    return vec;
}

Vector3i TomlParser::get_vec3i_at(const Array &p_keys) const {
    return get_vec3i_at_or(p_keys);
}

Vector3i TomlParser::get_vec3i_at_or(const Array &p_keys, const Vector3i &p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return default {1}.", { path_as_string(p_keys), p_default_value });
        return p_default_value;
    }
    Vector3i vec;
    if (const int8_t code = to_vec3i(node, vec); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at path '{0}' to Vector3i. Return Vector3i.ZERO.", { path_as_string(p_keys) });
        return {};
    }
    return vec;
}

TypedArray<Vector3i> TomlParser::get_vec3i_arr(const String &p_key) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return empty Array[Vector3i].", { p_key });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at key '{0}' is not an array. Return empty Array[Vector3i].", { p_key });
        return {};
    }

    TypedArray<Vector3i> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        Vector3i vec;
        if (const int8_t code = to_vec3i(value, vec); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at key '{1}' to Vector3i. Element is ignored.", {elm, p_key});
            continue;
        }
        result.append(vec);
        elm++;
    }
    return result;
}

TypedArray<Vector3i> TomlParser::get_vec3i_arr_at(const Array &p_keys) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return empty Array[Vector3i].", { path_as_string(p_keys) });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at path '{0}' is not an array. Return empty Array[Vector3i].", { path_as_string(p_keys) });
        return {};
    }

    TypedArray<Vector3i> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        Vector3i vec;
        if (const int8_t code = to_vec3i(value, vec); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at path '{1}' to Vector3i. Element is ignored.", {elm, path_as_string(p_keys)});
            continue;
        }
        result.append(vec);
        elm++;
    }
    return result;
}


// Color
// #############################################################################


Color TomlParser::get_color(const String &p_key) const {
    return get_color_or(p_key);
}

Color TomlParser::get_color_or(const String &p_key, const Color& p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return default {1}.", { p_key, p_default_value });
        return p_default_value;
    }

    Color color;
    if (const int8_t code = to_color(node, color); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at key '{0}' to Color. Return Color {0,0,0,0}.", { p_key });
        return {0,0,0,0};
    }

    return color;
}

Color TomlParser::get_color_at(const Array &p_keys) const {
    return get_color_at_or(p_keys);
}

Color TomlParser::get_color_at_or(const Array &p_keys, const Color& p_default_value) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return default {1}.", { path_as_string(p_keys), p_default_value });
        return p_default_value;
    }

    Color color;
    if (const int8_t code = to_color(node, color); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to cast node at path '{0}' to Color. Return default {1}.", { path_as_string(p_keys), p_default_value });
        return {0,0,0,0};
    }

    return color;
}

TypedArray<Color> TomlParser::get_color_arr(const String &p_key) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return empty Array[Color].", { p_key });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at key '{0}' is not an array. Return empty Array[Color].", { p_key });
        return {};
    }

    TypedArray<Color> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        Color color;
        if (const int8_t code = to_color(value, color); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at key '{1}' to Color. Element is ignored.", {elm, p_key});
            continue;
        }
        result.append(color);
        elm++;
    }
    return result;
}

TypedArray<Color> TomlParser::get_color_arr_at(const Array &p_keys) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return empty Array[Color].", { path_as_string(p_keys) });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at path '{0}' is not an array. Return empty Array[Color].", { path_as_string(p_keys) });
        return {};
    }

    TypedArray<Color> result = {};
    int elm = 0;
    // node should be an array of arrays
    for (const auto& value : node.as_array()) {
        Color color;
        if (const int8_t code = to_color(value, color); code != OK) {
            log_error_code(code);
            log("[ERROR] TomlParser : Failed to cast element {0} at path '{1}' to Color. Element is ignored.", {elm, path_as_string(p_keys)});
            continue;
        }
        result.append(color);
        elm++;
    }
    return result;
}


// Table
// #############################################################################


Dictionary TomlParser::get_table(const String &p_key) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return empty Dictionary.", { p_key });
        return {};
    }

    if (!node.is_table()) {
        log("[ERROR] TomlParser : The key '{0}' is not a table. Return empty Dictionary.", {p_key});
        return {};
    }

    Dictionary result = {};
    to_dictionary(node, result);
    return result;
}

Dictionary TomlParser::get_table_at(const Array &p_keys) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return empty Dictionary.", { path_as_string(p_keys) });
        return {};
    }

    if (!node.is_table()) {
        log("[ERROR] TomlParser : The node at path '{0}' is not a table. Return empty Dictionary.", { path_as_string(p_keys) });
        return {};
    }

    Dictionary result = {};
    to_dictionary(node, result);
    return result;
}

TypedArray<String> TomlParser::get_table_keys(const String &p_key) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return empty Dictionary.", { p_key });
        return {};
    }

    if (!node.is_table()) {
        log("[ERROR] TomlParser : The node at key '{0}' is not a table. Return empty Dictionary.", { p_key });
        return {};
    }

    TypedArray<String> result = {};
    for (const auto &k: node.as_table() | std::views::keys) {
        result.append(dec_string(k));
    }
    return result;
}


// Array
// #############################################################################


Array TomlParser::get_array(const String &p_key) const {
    toml::value node;
    if (const int8_t code = get_node(p_key, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at key '{0}'. Return empty Array.", { p_key });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at key '{0}' is not an array. Return empty Array.", { p_key });
        return {};
    }

    Array result = {};
    to_array(node, result);
    return result;
}

Array TomlParser::get_array_at(const Array &p_keys) const {
    toml::value node;
    if (const int8_t code = get_node(p_keys, node); code != OK) {
        log_error_code(code);
        log("[ERROR] TomlParser : Failed to get node at path '{0}'. Return empty Array.", { path_as_string(p_keys) });
        return {};
    }

    if (!node.is_array()) {
        log("[ERROR] TomlParser : The node at path '{0}' is not an array. Return empty Array.", { path_as_string(p_keys) });
        return {};
    }

    Array result = {};
    to_array(node, result);
    return result;
}



void TomlParser::to_dictionary(const toml::basic_value<toml::type_config> &p_table, Dictionary &p_dict) {
    for(const auto& [k, v] : p_table.as_table()) {
        auto label = Variant(k.c_str());

        if (v.is_boolean()) {
            p_dict[label] = v.as_boolean();
        }
        if (v.is_integer()) {
            p_dict[label] = dec_long(v);
        }
        if (v.is_floating()) {
            p_dict[label] = dec_float(v);
        }
        if (v.is_string()) {
            p_dict[label] = dec_string(v);
        }
        if (v.is_array()) {
            auto arr = Array();
            to_array(v, arr);
            p_dict[label] = arr;
        }
        if (v.is_table()) {
            auto d = Dictionary();
            to_dictionary(v, d);
            p_dict[label] = d;
        }
    }
}

void TomlParser::to_array(const toml::basic_value<toml::type_config> &p_value, Array &p_array) {
    for (const auto& value : p_value.as_array()) {
        if (value.is_boolean()) {
            p_array.append(value.as_boolean());
        }
        if (value.is_integer()) {
            p_array.append(dec_long(value));
        }
        if (value.is_floating()) {
            p_array.append(dec_float(value));
        }
        if (value.is_string()) {
            p_array.append(dec_string(value));
        }
        if (value.is_table()) {
            auto d = Dictionary();
            to_dictionary(value, d);
            p_array.append(d);
        }
        if (value.is_array()) {
            auto arr = Array();
            to_array(value, arr);
            p_array.append(arr);
        }
    }
}

int8_t TomlParser::to_string(const toml::basic_value<toml::type_config> &p_value, String &p_string) {
    if (!p_value.is_string()) {
        return ERR_INCORRECT_TYPE;
    }
    p_string = dec_string(p_value.as_string());
    return OK;
}

int8_t TomlParser::to_int(const toml::basic_value<toml::type_config> &p_value, int64_t &p_int) {
    if (!p_value.is_integer()) {
        return ERR_INCORRECT_TYPE;
    }
    p_int = p_value.as_integer();
    return OK;
}

int8_t TomlParser::to_float(const toml::basic_value<toml::type_config> &p_value, float &p_float) {
    if (!p_value.is_floating()) {
        return ERR_INCORRECT_TYPE;
    }
    p_float = p_value.as_floating();
    return OK;
}

int8_t TomlParser::to_bool(const toml::basic_value<toml::type_config> &p_value, bool &p_bool) {
    if (!p_value.is_boolean()) {
        return ERR_INCORRECT_TYPE;
    }
    p_bool = p_value.as_boolean();
    return OK;
}

int8_t TomlParser::to_color(const toml::basic_value<toml::type_config> &p_value, Color &p_color) {
    if (!p_value.is_array()) {
        return ERR_INCORRECT_TYPE;
    }
    const auto& arr = p_value.as_array();
    if (arr.size() < 3 || arr.size() > 4) {
        return ERR_INCORRECT_FORMAT;
    }
    float alpha = 1.0f;
    if (arr.size() == 4) {
        alpha = dec_float(arr[3]);
    }
    p_color = {dec_float(arr[0]), dec_float(arr[1]), dec_float(arr[2]), alpha};
    return OK;
}

int8_t TomlParser::to_vec2(const toml::basic_value<toml::type_config> &p_value, Vector2 &p_vec2) {
    if (!p_value.is_array()) {
        return ERR_INCORRECT_TYPE;
    }
    if (p_value.size() != 2) {
        return ERR_INCORRECT_FORMAT;
    }
    p_vec2 = {dec_float(p_value[0]), dec_float(p_value[1])};
    return OK;
}

int8_t TomlParser::to_vec2i(const toml::basic_value<toml::type_config> &p_value, Vector2i &p_vec2i) {
    if (!p_value.is_array()) {
        return ERR_INCORRECT_TYPE;
    }
    if (p_value.size() != 3) {
        return ERR_INCORRECT_FORMAT;
    }
    p_vec2i = {dec_int(p_value[0]), dec_int(p_value[1])};
    return OK;
}

int8_t TomlParser::to_vec3(const toml::basic_value<toml::type_config> &p_value, Vector3 &p_vec3) {
    if (!p_value.is_array()) {
        return ERR_INCORRECT_TYPE;
    }
    if (p_value.size() != 3) {
        return ERR_INCORRECT_FORMAT;
    }
    p_vec3 = {dec_float(p_value[0]), dec_float(p_value[1]), dec_float(p_value[2])};
    return OK;
}

int8_t TomlParser::to_vec3i(const toml::basic_value<toml::type_config> &p_value, Vector3i &p_vec3i) {
    if (!p_value.is_array()) {
        return ERR_INCORRECT_TYPE;
    }
    if (p_value.size() != 3) {
        return ERR_INCORRECT_FORMAT;
    }
    p_vec3i = {dec_int(p_value[0]), dec_int(p_value[1]), dec_int(p_value[2])};
    return OK;
}

int8_t TomlParser::get_node(const String &p_key, toml::value &node) const {
    if (p_key.is_empty()) {
        return ERR_KEY_EMPTY;
    }
    if (!t->contains(to_str(p_key))) {
        return ERR_KEY_NOT_FOUND;
    }
    node = t->at(to_str(p_key));
    return OK;
}

int8_t TomlParser::get_node(const Array &p_keys, toml::value &node) const {
    if (p_keys.size() == 0) {
        return ERR_KEY_MISSING;
    }
    const toml::value n = find_recursive(p_keys);
    if (n.is_empty()) {
        return ERR_NODE_EMPTY;
    }
    node = n;
    return OK;
}

bool TomlParser::has_path(const Array &p_keys) const {
    std::vector<toml::value> nodes = {};
    const auto &doc = *t;

    if (p_keys.size() == 0) {
        log("[ERROR] TomlParser::find_recursive : No keys provided. Return.");
        return false;
    }

    const std::string first = to_str(p_keys[0]);
    if (!doc.contains(first)) {
        log("[ERROR] TomlParser::find_recursive : Could not find key {0} in TOML doc. Return.", {first.c_str()});
        return false;
    }

    nodes.emplace_back(doc.at(first));

    for (int i = 1, j = 0; i < p_keys.size(); i++, j++) {
        if (!nodes[j].contains(to_str(p_keys[i]))) {
            log("[ERROR] TomlParser::find_recursive : Could not find key {0}. Return.", {p_keys[i]});
            return false;
        }
        nodes.emplace_back(nodes[j].at(to_str(p_keys[i])));
    }

    return true;
}

toml::value TomlParser::find_recursive(const Array &p_keys) const {
    std::vector<toml::value> nodes = {};

    if (p_keys.size() == 0) {
        log("[ERROR] TomlParser::find_recursive : No keys provided. Return.");
        return {};
    }

    const std::string first = to_str(p_keys[0]);
    if (!t->contains(first)) {
        log("[ERROR] TomlParser::find_recursive : Could not find key {0} in TOML doc. Return.", {first.c_str()});
        return {};
    }

    nodes.emplace_back(t->at(first));

    for (int i = 1, j = 0; i < p_keys.size(); i++, j++) {
        if (!nodes[j].contains(to_str(p_keys[i]))) {
            log("[ERROR] TomlParser::find_recursive : Could not find key {0}. Return.", {p_keys[i]});
            return {};
        }
        nodes.emplace_back(nodes[j].at(to_str(p_keys[i])));
    }

    // Should return a reference instead?
    return nodes[nodes.size() - 1];
}

String TomlParser::path_as_string(const Array &p_keys) {
    std::ostringstream oss;
    int i = 0;
    const auto num_keys = p_keys.size();
    for (const auto& k : p_keys) {
        oss << to_str(k);
        if (i < num_keys - 1) {
            oss << "/";
        }
        i++;
    }
    return dec_string(oss.str());
}

template<typename T>
TypedArray<T> TomlParser::FIND_TYPED_ARR(const Array &p_keys) const {
    const toml::value node = find_recursive(p_keys);
    if (node.is_empty() or !node.is_array()) {
        return {};
    }

    TypedArray<T> result = {};
    for (const auto arr = toml::get<std::vector<T>>(node); const auto &val : arr) {
        result.append(val);
    }
    return result;
}
