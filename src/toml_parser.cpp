#include "toml_parser.h"

#include <cfloat>
#include <toml.hpp>
#include "translations.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void TomlParser::_bind_methods() {
    ClassDB::bind_method(D_METHOD("logging", "logging"), &TomlParser::logging);

    ClassDB::bind_method(D_METHOD("try_parse", "p_content"), &TomlParser::try_parse);
    ClassDB::bind_method(D_METHOD("get_string", "p_key"), &TomlParser::get_string);
    ClassDB::bind_method(D_METHOD("get_string_or", "p_key", "p_default_value"), &TomlParser::get_string_or, DEFVAL(""));
    ClassDB::bind_method(D_METHOD("get_string_at", "p_keys"), &TomlParser::get_string_at);
    ClassDB::bind_method(D_METHOD("get_string_arr_at", "p_keys"), &TomlParser::get_string_arr_at);
    ClassDB::bind_method(D_METHOD("get_int", "p_key"), &TomlParser::get_int);
    ClassDB::bind_method(D_METHOD("get_int_or", "p_key", "p_default_value"), &TomlParser::get_int_or, DEFVAL(""));
    ClassDB::bind_method(D_METHOD("get_int_at", "p_keys"), &TomlParser::get_int_at);
    ClassDB::bind_method(D_METHOD("get_int_arr_at", "p_keys"), &TomlParser::get_int_arr_at);
    ClassDB::bind_method(D_METHOD("get_float", "p_key"), &TomlParser::get_float);
    ClassDB::bind_method(D_METHOD("get_float_or", "p_key", "p_default_value"), &TomlParser::get_float_or, DEFVAL(""));
    ClassDB::bind_method(D_METHOD("get_float_at", "p_keys"), &TomlParser::get_float_at);
    ClassDB::bind_method(D_METHOD("get_float_arr_at", "p_keys"), &TomlParser::get_float_arr_at);
    ClassDB::bind_method(D_METHOD("get_bool", "p_key"), &TomlParser::get_bool);
    ClassDB::bind_method(D_METHOD("get_bool_or", "p_key", "p_default_value"), &TomlParser::get_bool_or, DEFVAL(""));
    ClassDB::bind_method(D_METHOD("get_bool_at", "p_keys"), &TomlParser::get_bool_at);
    ClassDB::bind_method(D_METHOD("get_bool_arr_at", "p_keys"), &TomlParser::get_bool_arr_at);
    ClassDB::bind_method(D_METHOD("get_vec2", "p_key"), &TomlParser::get_vec2);
    ClassDB::bind_method(D_METHOD("get_vec2_or", "p_key", "p_default_value"), &TomlParser::get_vec2_or, DEFVAL(""));
    ClassDB::bind_method(D_METHOD("get_vec2_at", "p_keys"), &TomlParser::get_vec2_at);
    ClassDB::bind_method(D_METHOD("get_vec2_arr_at", "p_keys"), &TomlParser::get_vec2_arr_at);
    ClassDB::bind_method(D_METHOD("get_vec2i", "p_key"), &TomlParser::get_vec2i);
    ClassDB::bind_method(D_METHOD("get_vec2i_or", "p_key", "p_default_value"), &TomlParser::get_vec2i_or, DEFVAL(""));
    ClassDB::bind_method(D_METHOD("get_vec2i_at", "p_keys"), &TomlParser::get_vec2i_at);
    ClassDB::bind_method(D_METHOD("get_vec2i_arr_at", "p_keys"), &TomlParser::get_vec2i_arr_at);
    ClassDB::bind_method(D_METHOD("get_vec3", "p_key"), &TomlParser::get_vec3);
    ClassDB::bind_method(D_METHOD("get_vec3_or", "p_key", "p_default_value"), &TomlParser::get_vec3_or, DEFVAL(""));
    ClassDB::bind_method(D_METHOD("get_vec3_at", "p_keys"), &TomlParser::get_vec3_at);
    ClassDB::bind_method(D_METHOD("get_vec3_arr_at", "p_keys"), &TomlParser::get_vec3_arr_at);
    ClassDB::bind_method(D_METHOD("get_vec3i", "p_key"), &TomlParser::get_vec3i);
    ClassDB::bind_method(D_METHOD("get_vec3i_or", "p_key", "p_default_value"), &TomlParser::get_vec3i_or, DEFVAL(""));
    ClassDB::bind_method(D_METHOD("get_vec3i_at", "p_keys"), &TomlParser::get_vec3i_at);
    ClassDB::bind_method(D_METHOD("get_vec3i_arr_at", "p_keys"), &TomlParser::get_vec3i_arr_at);
    ClassDB::bind_method(D_METHOD("get_color", "p_key"), &TomlParser::get_color);
    ClassDB::bind_method(D_METHOD("get_color_or", "p_key", "p_default_value"), &TomlParser::get_color_or, DEFVAL(""));
    ClassDB::bind_method(D_METHOD("get_color_at", "p_keys"), &TomlParser::get_color_at);
    ClassDB::bind_method(D_METHOD("get_color_arr_at", "p_keys"), &TomlParser::get_color_arr_at);

    ClassDB::bind_method(D_METHOD("get_table", "p_key"), &TomlParser::get_table);
    ClassDB::bind_method(D_METHOD("get_table_at", "p_keys"), &TomlParser::get_table_at);
    ClassDB::bind_method(D_METHOD("get_table_keys", "p_key"), &TomlParser::get_table_keys);
    ClassDB::bind_method(D_METHOD("get_array", "p_key"), &TomlParser::get_array);
    ClassDB::bind_method(D_METHOD("get_array_at", "p_keys"), &TomlParser::get_array_at);

    ClassDB::bind_method(D_METHOD("key_exists", "p_key"), &TomlParser::key_exists);
    ClassDB::bind_method(D_METHOD("path_exist", "p_keys"), &TomlParser::path_exist);
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

bool TomlParser::path_exist(const Array &p_keys) const {
    return has_path(p_keys);
}

String TomlParser::get_string(const String &p_key) const {
    return get_string_or(p_key, "");
}
String TomlParser::get_string_or(const String &p_key, const String &p_default_value) const {
    return {toml::find_or<std::string>(*t, to_str(p_key), to_str(p_default_value)).c_str()};
}

String TomlParser::get_string_at(const Array &p_keys) const {
    const toml::value node = find_recursive(p_keys);
    if (node.is_empty() or !node.is_string()) {
        return {};
    }
    return dec_string(node);
}
TypedArray<String> TomlParser::get_string_arr_at(const Array &p_keys) const {
    const toml::value node = find_recursive(p_keys);
    if (node.is_empty() or !node.is_array()) {
        return {};
    }

    TypedArray<String> result = {};
    for (const auto arr = toml::get<std::vector<std::string>>(node); const auto &val : arr) {
        result.append(dec_string(val));
    }
    return result;
}

int64_t TomlParser::get_int(const String &p_key) const {
    return get_int_or(p_key, INT64_MIN);
}
int64_t TomlParser::get_int_or(const String &p_key, const int64_t p_default_value) const {
    return toml::find_or<int64_t>(*t, to_str(p_key), p_default_value);
}
int64_t TomlParser::get_int_at(const Array &p_keys) const {
    const toml::value node = find_recursive(p_keys);
    if (node.is_empty() or !node.is_integer()) {
        return INT64_MIN;
    }
    return toml::get<int64_t>(node);
}
TypedArray<int64_t> TomlParser::get_int_arr_at(const Array &p_keys) const {
    return FIND_TYPED_ARR<int64_t>(p_keys);
}

float TomlParser::get_float(const String &p_key) const {
    return get_float_or(p_key, FLT_MIN);
}
float TomlParser::get_float_or(const String &p_key, const float p_default_value) const {
    return toml::find_or<float>(*t, to_str(p_key), p_default_value);
}
float TomlParser::get_float_at(const Array &p_keys) const {
    const toml::value node = find_recursive(p_keys);
    if (node.is_empty() or !node.is_floating()) {
        return FLT_MIN;
    }
    return toml::get<float>(node);
}
TypedArray<float> TomlParser::get_float_arr_at(const Array &p_keys) const {
    return FIND_TYPED_ARR<float>(p_keys);
}

bool TomlParser::get_bool(const String &p_key) const {
    return get_bool_or(p_key, false);
}
bool TomlParser::get_bool_or(const String &p_key, const int p_default_value) const {
    return toml::find_or<bool>(*t, to_str(p_key), p_default_value);
}
bool TomlParser::get_bool_at(const Array &p_keys) const {
    toml::value node = find_recursive(p_keys);
    if (node.is_empty() or !node.is_boolean()) {
        return false;
    }
    return node.as_boolean();
}
TypedArray<bool> TomlParser::get_bool_arr_at(const Array &p_keys) const {
    return FIND_TYPED_ARR<bool>(p_keys);
}

Vector2 TomlParser::get_vec2(const String &p_key) const {
    return get_vec2_or(p_key, {});
}
Vector2 TomlParser::get_vec2_or(const String &p_key, const Vector2 p_default_value) const {
    auto val = toml::find<std::vector<float>>(*t, to_str(p_key));
    if (val.empty() or val.size() != 2) {
        return p_default_value;
    }
    return { val[0], val[1] };
}
Vector2 TomlParser::get_vec2_at(const Array &p_keys) const {
    auto node = find_recursive(p_keys);
    if (node.is_empty() or node.size() != 2) {
        return {};
    }
    return { dec_float(node[0]), dec_float(node[1]) };
}
TypedArray<Vector2> TomlParser::get_vec2_arr_at(const Array &p_keys) const {
    const toml::value node = find_recursive(p_keys);
    if (node.is_empty()) {
        return {};
    }

    TypedArray<Vector2> result = {};
    for (const auto vecs = toml::get<std::vector<std::vector<float>>>(node); const auto &v : vecs) {
        if (v.size() != 2) {
            log("[ERROR] TomlParser::get_array_vector2 : Expected an array of 2 floats, but found {0}", {v.size()});
            continue;
        }
        result.append(Vector2{ dec_float(node[0]), dec_float(node[1]) });
    }
    return result;
}

Vector2i TomlParser::get_vec2i(const String &p_key) const {
    auto val = toml::find<std::vector<int>>(*t, to_str(p_key));
    if (val.empty() or val.size() != 2) {
        return {};
    }
    return { val[0], val[1] };
}
Vector2i TomlParser::get_vec2i_or(const String &p_key, const Vector2i p_default_value) const {
    auto val = toml::find<std::vector<int>>(*t, to_str(p_key));
    if (val.empty() or val.size() != 2) {
        return p_default_value;
    }
    return { val[0], val[1] };
}
Vector2i TomlParser::get_vec2i_at(const Array &p_keys) const {
    auto node = find_recursive(p_keys);
    if (node.is_empty() or node.size() != 2) {
        return {};
    }
    return { dec_int(node[0]), dec_int(node[1]) };
}
TypedArray<Vector2i> TomlParser::get_vec2i_arr_at(const Array &p_keys) const {
    const toml::value node = find_recursive(p_keys);
    if (node.is_empty()) {
        return {};
    }

    TypedArray<Vector2i> result = {};
    for (const auto vecs = toml::get<std::vector<std::vector<int>>>(node); const auto &v : vecs) {
        if (v.size() != 2) {
            log("[ERROR] TomlParser::get_array_vector2i : Expected an array of 2 ints, but found {0}", {v.size()});
            continue;
        }
        result.append(Vector2i{ dec_int(node[0]), dec_int(node[1]) });
    }
    return result;
}

Vector3 TomlParser::get_vec3(const String &p_key) const {
    auto val = toml::find<std::vector<float>>(*t, to_str(p_key));
    if (val.empty() or val.size() != 3) {
        return {};
    }
    return { val[0], val[1], val[2] };
}
Vector3 TomlParser::get_vec3_or(const String &p_key, const Vector3 p_default_value) const {
    auto val = toml::find<std::vector<float>>(*t, to_str(p_key));
    if (val.empty() or val.size() != 3) {
        return p_default_value;
    }
    return { val[0], val[1], val[2] };
}
Vector3 TomlParser::get_vec3_at(const Array &p_keys) const {
    auto node = find_recursive(p_keys);
    if (node.is_empty() or node.size() != 3) {
        return {};
    }
    return { dec_float(node[0]), dec_float(node[1]), dec_float(node[2]) };
}
TypedArray<Vector3> TomlParser::get_vec3_arr_at(const Array &p_keys) const {
    const toml::value node = find_recursive(p_keys);
    if (node.is_empty()) {
        return {};
    }

    TypedArray<Vector3> result = {};
    for (const auto vecs = toml::get<std::vector<std::vector<float>>>(node); const auto &v : vecs) {
        if (v.size() != 3) {
            log("[ERROR] TomlParser::get_array_vector3 : Expected an array of 3 floats, but found {0}", {v.size()});
            continue;
        }
        result.append(Vector3{ dec_float(node[0]), dec_float(node[1]), dec_float(node[2]) });
    }
    return result;
}

Vector3i TomlParser::get_vec3i(const String &p_key) const {
    auto val = toml::find<std::vector<int>>(*t, to_str(p_key));
    if (val.empty() or val.size() != 3) {
        return {};
    }
    return { val[0], val[1], val[2] };
}
Vector3i TomlParser::get_vec3i_or(const String &p_key, const Vector3i p_default_value) const {
    auto val = toml::find<std::vector<int>>(*t, to_str(p_key));
    if (val.empty() or val.size() != 3) {
        return p_default_value;
    }
    return { val[0], val[1], val[2] };
}
Vector3i TomlParser::get_vec3i_at(const Array &p_keys) const {
    auto node = find_recursive(p_keys);
    if (node.is_empty() or node.size() != 3) {
        return {};
    }
    return { dec_int(node[0]), dec_int(node[1]), dec_int(node[2])};
}
TypedArray<Vector3i> TomlParser::get_vec3i_arr_at(const Array &p_keys) const {
    const toml::value node = find_recursive(p_keys);
    if (node.is_empty()) {
        return {};
    }

    TypedArray<Vector3i> result = {};
    for (const auto vecs = toml::get<std::vector<std::vector<int>>>(node); const auto &v : vecs) {
        if (v.size() != 3) {
            log("[ERROR] TomlParser::get_array_vector3i : Expected an array of 3 ints, but found {0}", {v.size()});
            continue;
        }
        result.append(Vector3i{ dec_int(node[0]), dec_int(node[1]), dec_int(node[2])});
    }
    return result;
}

Color TomlParser::get_color(const String &p_key) const {
    auto &val = toml::find<std::string>(*t, to_str(p_key));
    if (val.empty()) {
        return {};
    }
    Array result = {};
    to_array(val, result);
    return {result[0], result[1], result[2], result[3]};
}
Color TomlParser::get_color_or(const String &p_key, const Color p_default_value) const {
    auto &val = toml::find<std::string>(*t, to_str(p_key));
    if (val.empty()) {
        return p_default_value;
    }
    Array result = {};
    to_array(val, result);
    return {result[0], result[1], result[2], result[3]};
}
Color TomlParser::get_color_at(const Array &p_keys) const {
    const toml::value node = find_recursive(p_keys);
    if (node.is_empty() or !node.is_array()) {
        return Color{0,0,0,0};
    }
    Array result = {};
    to_array(node, result);
    return {result[0], result[1], result[2], result[3]};
}
TypedArray<Color> TomlParser::get_color_arr_at(const Array &p_keys) const {
    const toml::value node = find_recursive(p_keys);
    if (node.is_empty()) {
        return {};
    }

    TypedArray<Color> result = {};
    for (const auto vecs = toml::get<std::vector<std::string>>(node); const auto &v : vecs) {
        Array arr = {};
        to_array(v, arr);
        result.append(Color(arr[0], arr[1], arr[2], arr[3]));
    }
    return result;
}

Dictionary TomlParser::get_table(const String &p_key) const {
    const std::string key = to_str(p_key);
    if (!t->contains(key) || !t->at(key).is_table()) {
        log("[ERROR] TomlParser::get_table : Cannot find table with key '{0}'. Return empty Dictionary.", {p_key});
        return {};
    }
    Dictionary result = {};
    to_dictionary(t->at(key), result);
    return result;
}
Dictionary TomlParser::get_table_at(const Array &p_keys) const {
    const toml::value node = find_recursive(p_keys);
    if (node.is_empty() or !node.is_table()) {
        return {};
    }
    Dictionary result = {};
    to_dictionary(node, result);
    return result;
}

TypedArray<String> TomlParser::get_table_keys(const String &p_key) const {
    const std::string key = to_str(p_key);
    TypedArray<String> result = {};
    if (!t->contains(key)) {
        return result;
    }
    if (!t->at(key).is_table()) {
        return result;
    }
    for(const auto& [k, v] : t->at(key).as_table()) {
        result.append(dec_string(k));
    }
    return result;
}

Array TomlParser::get_array(const String &p_key) const {
    const std::string key = to_str(p_key);
    if (!t->contains(key) || !t->at(key).is_array()) {
        log("[ERROR] TomlParser::get_array : Cannot find array with key '{0}'. Return empty Array.", {p_key});
        return {};
    }
    Array result = {};
    to_array(t->at(key), result);
    return result;
}
Array TomlParser::get_array_at(const Array &p_keys) const {
    const toml::value node = find_recursive(p_keys);
    if (node.is_empty() or !node.is_array()) {
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
