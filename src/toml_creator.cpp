#include "toml_creator.h"

#include <deque>

#include "../include/toml.hpp"
#include "translations.h"
#include <godot_cpp/core/class_db.hpp>
#include <memory>

using namespace godot;

TomlCreator::TomlCreator() {
    // disable logging in release build
    enable_logging = true;
    t = std::make_unique<toml::value>();
}

TomlCreator::~TomlCreator() {
    // t = nullptr;
}

void TomlCreator::logging(bool logging) {
    enable_logging = logging;
}

void TomlCreator::_bind_methods() {
    ClassDB::bind_integer_constant("TomlCreator", "ArrayFormat", "AF_DEFAULT_FORMAT", AF_DEFAULT_FORMAT);
    ClassDB::bind_integer_constant("TomlCreator", "ArrayFormat", "AF_ONELINE", AF_ONELINE);
    ClassDB::bind_integer_constant("TomlCreator", "ArrayFormat", "AF_MULTILINE", AF_MULTILINE);
    ClassDB::bind_integer_constant("TomlCreator", "ArrayFormat", "AF_ARRAY_OF_TABLES", AF_ARRAY_OF_TABLES);

    ClassDB::bind_integer_constant("TomlCreator", "TableFormat", "TF_MULTILINE", TF_MULTILINE);
    ClassDB::bind_integer_constant("TomlCreator", "TableFormat", "TF_MULTILINE_ONELINE", TF_MULTILINE_ONELINE);
    ClassDB::bind_integer_constant("TomlCreator", "TableFormat", "TF_DOTTED", TF_DOTTED);
    ClassDB::bind_integer_constant("TomlCreator", "TableFormat", "TF_IMPLICIT", TF_IMPLICIT);
    ClassDB::bind_integer_constant("TomlCreator", "TableFormat", "TF_ONELINE", TF_ONELINE);

    ClassDB::bind_integer_constant("TomlCreator", "IndentChar", "IC_NONE", IC_NONE);
    ClassDB::bind_integer_constant("TomlCreator", "IndentChar", "IC_SPACE", IC_SPACE);
    ClassDB::bind_integer_constant("TomlCreator", "IndentChar", "IC_TAB", IC_TAB);

    ClassDB::bind_method(D_METHOD("logging", "logging"), &TomlCreator::logging);

    ClassDB::bind_method(D_METHOD("set_int", "p_label", "p_value"), &TomlCreator::set_int);
    ClassDB::bind_method(D_METHOD("set_float", "p_label", "p_value"), &TomlCreator::set_float);
    ClassDB::bind_method(D_METHOD("set_string", "p_label", "p_value"), &TomlCreator::set_string);
    ClassDB::bind_method(D_METHOD("set_bool", "p_label", "p_value"), &TomlCreator::set_bool);
    ClassDB::bind_method(D_METHOD("set_color", "p_label", "p_value"), &TomlCreator::set_color);
    ClassDB::bind_method(D_METHOD("set_vec2", "p_label", "p_value"), &TomlCreator::set_vec2);
    ClassDB::bind_method(D_METHOD("set_vec2i", "p_label", "p_value"), &TomlCreator::set_vec2i);
    ClassDB::bind_method(D_METHOD("set_vec3", "p_label", "p_value"), &TomlCreator::set_vec3);
    ClassDB::bind_method(D_METHOD("set_vec3i", "p_label", "p_value"), &TomlCreator::set_vec3i);
    ClassDB::bind_method(D_METHOD("set_vec4", "p_label", "p_value"), &TomlCreator::set_vec4);
    ClassDB::bind_method(D_METHOD("set_vec4i", "p_label", "p_value"), &TomlCreator::set_vec4i);
    ClassDB::bind_method(D_METHOD("set_variant", "p_label", "p_value"), &TomlCreator::set_variant);
    ClassDB::bind_method(D_METHOD("set_array", "p_label", "p_value"), &TomlCreator::set_array);
    ClassDB::bind_method(D_METHOD("set_table", "p_label", "p_value"), &TomlCreator::set_table);

    // ClassDB::bind_method(D_METHOD("set_int_at", "p_keys", "p_label", "p_value"), &TomlCreator::set_int_at);
    // ClassDB::bind_method(D_METHOD("set_float_at", "p_label", "p_value"), &TomlCreator::set_float_at);
    // ClassDB::bind_method(D_METHOD("set_string_at", "p_label", "p_value"), &TomlCreator::set_string_at);
    // ClassDB::bind_method(D_METHOD("set_bool_at", "p_label", "p_value"), &TomlCreator::set_bool_at);
    // ClassDB::bind_method(D_METHOD("set_color_at", "p_label", "p_value"), &TomlCreator::set_color_at);
    // ClassDB::bind_method(D_METHOD("set_vec2_at", "p_label", "p_value"), &TomlCreator::set_vec2_at);
    // ClassDB::bind_method(D_METHOD("set_vec2i_at", "p_label", "p_value"), &TomlCreator::set_vec2i_at);
    // ClassDB::bind_method(D_METHOD("set_vec3_at", "p_label", "p_value"), &TomlCreator::set_vec3_at);
    // ClassDB::bind_method(D_METHOD("set_vec3i_at", "p_label", "p_value"), &TomlCreator::set_vec3i_at);
    // ClassDB::bind_method(D_METHOD("set_vec4_at", "p_label", "p_value"), &TomlCreator::set_vec4_at);
    // ClassDB::bind_method(D_METHOD("set_vec4i_at", "p_label", "p_value"), &TomlCreator::set_vec4i_at);
    // ClassDB::bind_method(D_METHOD("set_variant_at", "p_label", "p_value"), &TomlCreator::set_variant_at);
    // ClassDB::bind_method(D_METHOD("set_array_at", "p_label", "p_value"), &TomlCreator::set_array_at);
    // ClassDB::bind_method(D_METHOD("set_table_at", "p_label", "p_value"), &TomlCreator::set_table_at);

    ClassDB::bind_method(D_METHOD("format_array", "p_label", "p_arrfmt", "p_body_indent", "p_closing_indent"), &TomlCreator::format_array, DEFVAL(4), DEFVAL(2));
    ClassDB::bind_method(D_METHOD("format_table", "p_label", "p_tblfmt", "p_body_indent", "p_closing_indent", "p_name_indent", "p_indent_char"), &TomlCreator::format_table, DEFVAL(4), DEFVAL(2), DEFVAL(0), DEFVAL(0));
    ClassDB::bind_method(D_METHOD("serialize"), &TomlCreator::serialize);
}

void TomlCreator::log(const String &message, const Array &args = {}) const {
    if (enable_logging) {
        UtilityFunctions::print(message.format(args));
    }
}

bool TomlCreator::ensure(const bool condition, const std::string &message, const Array &args = {}) const {
    if (!condition) {
        log(String(message.c_str()), args);
    }
    return condition;
}

void TomlCreator::warn_if(const bool condition, const std::string &message, const Array &args = {}) const {
    if (condition) {
        log(String(message.c_str()), args);
    }
}

void TomlCreator::parse_dict(toml::value &p_toml, const Dictionary &p_dict) {
    for (const auto &key : p_dict.keys()) {
        auto &      variant = p_dict[key];
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

void TomlCreator::parse_array(toml::value &p_toml, const Array &p_arr) {
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

template<typename T>
void TomlCreator::set_value_at(const Array &p_keys, const toml::value& p_value) {
    toml::value &doc = *t;

    if (p_keys.size() == 1) {
        // will overwrite any other values at this key. That's expected, right?
        doc[to_str(p_keys[0])] = p_value;
        return;
    }

    toml::value output = toml::find_or(*t, to_str(p_keys[0]), T{});
    for (int i=1; i<p_keys.size(); i++) {
        if (i == p_keys.size()-1) {
            output[to_str(p_keys[i])] = p_value;
        } else {
            output = toml::find_or(output, to_str(p_keys[i]), T{});
        }
    }

    doc[to_str(p_keys[0])] = output;
}

// template<typename T>
void TomlCreator::set_value_at(const Array &p_keys, const String &p_label, const toml::value& p_value) const {
    toml::value &doc = *t;
    const std::string root_key = to_str(p_keys[0]);

    if (p_keys.size() == 1) {
        // will overwrite any other values at this key. That's expected, right?
        doc[root_key] = toml::table{{to_str(p_label), p_value}};
        return;
    }

    log("root_key: {0}", {root_key.c_str()});
    // toml::value output = toml::find_or_default<toml::table>(doc, root_key);
    // const auto output = std::make_shared<toml::value>(toml::find_or_default<toml::table>(doc, root_key));
    std::vector<std::shared_ptr<toml::value>> tmps = {};
    // tmps.emplace_back(toml::find_or_default<toml::table>(doc, root_key));
    tmps.emplace_back(std::make_shared<toml::value>(toml::find_or_default<toml::table>(doc, root_key)));
    // auto tmp = std::make_shared<toml::value>(output);
    // toml::value &tt = output;

    // if (output.is_empty()) {
    //     log("root_key is empty");
    // }

    for (int i=1; i<p_keys.size(); i++) {
        std::string key = to_str(p_keys[i]);
        log("key {0}: {1}", {i, p_keys[i]});
        log("i: {0} tmp: {1}", {i, i-1});
        toml::value &cur = *tmps[i-1];

        if (!cur.contains(key)) {
            log("add table for key {0}", {p_keys[i]});
            cur[key] = toml::table{};
            // tmps.emplace_back(std::make_shared<toml::value>(cur.at(key)));
            // tmp = *std::make_shared<toml::value>(tmp.at(key));
            // tmp = std::make_shared<toml::value>(tmp->at(key));
            // tmp.reset(tmp->[key]);
            // continue;
            // tmp = toml::table{ { key, toml::table{} } };
            // tmp = tmp.at(key).as_table();
        // } else {
        //     log("output contains key {0}", {p_keys[i]});
        //     log("append table for key {0}", {p_keys[i]});
        //     output = output.at(key).as_table();
        }

        if (!cur.at(key).is_table()) {
            log("entry at key {0} is not a table", {p_keys[i]});
            return;
        }

        // tmp = tmp.at(key).as_table();
        // tmp = std::make_shared<toml::value>(tmp->at(key));
        // tmps.emplace_back(&cur.at(key));
        log("add pointer to table for key {0}", {p_keys[i]});
        tmps.emplace_back(std::make_shared<toml::value>(cur.at(key)));

        if (i == p_keys.size()-1) {
            toml::value &last = *tmps[i];
            log("done. inject {0}", {p_label});
            log("i: {0} last: {1}", {i, i});
            // tmp[to_str(p_label)] = p_value;
            last[to_str(p_label)] = p_value;
        }
    }

    log("write output to {0}", {p_keys[0]});
    // toml::value &doc = *t;
    doc[root_key] = std::move(*tmps[0]);
    // doc[root_key] = std::move(output);
    // t->at(to_str(p_keys[0])) = output;
}

template<typename T>
T TomlCreator::find_recursive(const toml::value& v, std::deque<std::string> keys) {
    if(keys.size() == 1) {
        return toml::find<T>(v, keys.front());
    }

    const toml::value& sub = toml::find(v, keys.front());
    keys.pop_front();
    return find_recursive<T>(sub, std::move(keys));
}

void TomlCreator::set_value(const String &p_label, const Variant &p_value) const {
    const std::string label_str = to_str(p_label);
    toml::value &doc = *t;
    doc[label_str] = enc(p_value);
}

void TomlCreator::set_int(const String &p_label, const int p_value) const { set_value(p_label, p_value); }
void TomlCreator::set_float(const String &p_label, const float p_value) const { set_value(p_label, p_value); }
void TomlCreator::set_bool(const String &p_label, const bool p_value) const { set_value(p_label, p_value); }
void TomlCreator::set_string(const String &p_label, const String &p_value) const { set_value(p_label, p_value); }
void TomlCreator::set_color(const String &p_label, const Color &p_value) const { set_value(p_label, p_value); }
void TomlCreator::set_vec2(const String &p_label, const Vector2 &p_value) const { set_value(p_label, p_value); }
void TomlCreator::set_vec2i(const String &p_label, const Vector2i &p_value) const { set_value(p_label, p_value); }
void TomlCreator::set_vec3(const String &p_label, const Vector3 &p_value) const { set_value(p_label, p_value); }
void TomlCreator::set_vec3i(const String &p_label, const Vector3i &p_value) const { set_value(p_label, p_value); }
void TomlCreator::set_vec4(const String &p_label, const Vector4 &p_value) const { set_value(p_label, p_value); }
void TomlCreator::set_vec4i(const String &p_label, const Vector4i &p_value) const { set_value(p_label, p_value); }
void TomlCreator::set_variant(const String &p_label, const Variant &p_value) const { set_value(p_label, p_value); }

void TomlCreator::set_int_at(const Array &p_keys, const String &p_label, const int p_value) const {
    warn_if(p_keys.size() == 0, "[gdex-toml:set_int_at] p_keys has no elements. Cannot set value.");
    set_value_at(p_keys, p_label, enc(p_value));
}
void TomlCreator::set_float_at(const Array &p_keys, const float p_value) {
    warn_if(p_keys.size() == 0, "[gdex-toml:set_float_at] p_keys has no elements. Cannot set value.");
    set_value_at<toml::value>(p_keys, enc(p_value));
}
void TomlCreator::set_string_at(const Array &p_keys, const String &p_value) {
    warn_if(p_keys.size() == 0, "[gdex-toml:set_string_at] p_keys has no elements. Cannot set value.");
    set_value_at<toml::value>(p_keys, enc(p_value));
}
void TomlCreator::set_bool_at(const Array &p_keys, const bool p_value) {
    warn_if(p_keys.size() == 0, "[gdex-toml:set_bool_at] p_keys has no elements. Cannot set value.");
    set_value_at<toml::value>(p_keys, enc(p_value));
}
void TomlCreator::set_color_at(const Array &p_keys, const Color &p_value) {
    warn_if(p_keys.size() == 0, "[gdex-toml:set_color_at] p_keys has no elements. Cannot set value.");
    set_value_at<toml::value>(p_keys, enc(p_value));
}
void TomlCreator::set_vec2_at(const Array &p_keys, const Vector2 &p_value) {
    warn_if(p_keys.size() == 0, "[gdex-toml:set_vec2_at] p_keys has no elements. Cannot set value.");
    set_value_at<toml::value>(p_keys, enc(p_value));
}
void TomlCreator::set_vec2i_at(const Array &p_keys, const Vector2i &p_value) {
    warn_if(p_keys.size() == 0, "[gdex-toml:set_vec2i_at] p_keys has no elements. Cannot set value.");
    set_value_at<toml::value>(p_keys, enc(p_value));
}
void TomlCreator::set_vec3_at(const Array &p_keys, const Vector3 &p_value) {
    warn_if(p_keys.size() == 0, "[gdex-toml:set_vec3_at] p_keys has no elements. Cannot set value.");
    set_value_at<toml::value>(p_keys, enc(p_value));
}
void TomlCreator::set_vec3i_at(const Array &p_keys, const Vector3i &p_value) {
    warn_if(p_keys.size() == 0, "[gdex-toml:set_vec3i_at] p_keys has no elements. Cannot set value.");
    set_value_at<toml::value>(p_keys, enc(p_value));
}
void TomlCreator::set_vec4_at(const Array &p_keys, const Vector4 &p_value) {
    warn_if(p_keys.size() == 0, "[gdex-toml:set_vec4_at] p_keys has no elements. Cannot set value.");
    set_value_at<toml::value>(p_keys, enc(p_value));
}

void TomlCreator::set_vec4i_at(const Array &p_keys, const Vector4i &p_value) {
    warn_if(p_keys.size() == 0, "[gdex-toml:set_vec4i_at] p_keys has no elements. Cannot set value.");
    set_value_at<toml::value>(p_keys, enc(p_value));
}

void TomlCreator::set_variant_at(const Array &p_keys, const Variant &p_value) {
    warn_if(p_keys.size() == 0, "[gdex-toml:set_variant_at] p_keys has no elements. Cannot set value.");
    set_value_at<toml::value>(p_keys, enc(p_value));
}

void TomlCreator::set_array(const String &p_label, const Array &p_value) const {
    toml::value arr(toml::array{});
    parse_array(arr, p_value);
    // t[to_str(p_label)] = arr;
    // t->at(to_str(p_label)) = arr;
    auto &doc = *t;
    doc[to_str(p_label)] = arr;
}

void TomlCreator::set_array_at(const Array &p_keys, const Array &p_value) {
    warn_if(p_keys.size() == 0, "[gdex-toml:set_array_at] p_keys has no elements. Cannot set value.");
    toml::value arr(toml::array{});
    parse_array(arr, p_value);
    set_value_at<toml::array>(p_keys, arr);
}

void TomlCreator::set_table(const String &p_label, const Dictionary &p_value) const {
    toml::value tbl(toml::table{});
    parse_dict(tbl, p_value);
    // t[to_str(p_label)] = tbl;
    auto &doc = *t;
    doc[to_str(p_label)] = tbl;
}

void TomlCreator::set_table_at(const Array &p_keys, const Dictionary &p_value) {
    warn_if(p_keys.size() == 0, "[gdex-toml:set_table_at] p_keys has no elements. Cannot set value.");
    toml::value tbl(toml::table{});
    parse_dict(tbl, p_value);
    set_value_at<toml::table>(p_keys, tbl);
}

void TomlCreator::format_array(const String &p_key,
                               ArrayFormat   p_arrfmt,
                               const int     p_body_indent = 4,
                               const int     p_closing_indent = 2) const {

    const std::string key = to_str(p_key);

    if (!ensure(t->contains(key),
                "[gdex-toml:format_array] Cannot find toml entry at key '{0}'", {p_key})) {
        return;
    }
    if (!ensure(t->at(key).is_array(),
                "[gdex-toml:format_array] Toml entry at key '{0}' is not an array", {p_key})) {
        return;
    }

    auto&[table_format, indent_type, body_indent, closing_indent] = t->at(key).as_array_fmt();
    table_format   = static_cast<toml::array_format>(p_arrfmt);
    body_indent    = p_body_indent;
    closing_indent = p_closing_indent;
}

void TomlCreator::format_table(const String &p_key,
                               TableFormat   p_tblfmt,
                               const int     p_body_indent = 4,
                               const int     p_closing_indent = 2,
                               const int     p_name_indent = 0,
                               IndentChar    p_indent_char = IC_SPACE) const {

    const std::string key = to_str(p_key);

    if (!ensure(t->contains(key),
                "[gdex-toml:format_table] Cannot find toml entry at key '{0}'", {p_key})) {
        return;
    }
    if (!ensure(t->at(key).is_table(),
                "[gdex-toml:format_table] Toml entry at key '{0}' is not a table", {p_key})) {
        return;
    }

    auto&[table_format, indent_type, body_indent, name_indent, closing_indent] = t->at(key).as_table_fmt();
    table_format   = static_cast<toml::table_format>(p_tblfmt);
    body_indent    = p_body_indent;
    closing_indent = p_closing_indent;
    indent_type    = static_cast<toml::indent_char>(p_indent_char);
    name_indent    = p_name_indent;
}

String TomlCreator::serialize() const {
    const toml::value &doc = *t;
    return {toml::format(doc).c_str()};
}
