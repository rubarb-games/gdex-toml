#include "toml_parser.h"
#include "../include/toml.hpp"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "godot_cpp/core/class_db.hpp"
// #include <godot_cpp/variant/dictionary.hpp>

using namespace godot;

void TomlParser::_bind_methods() {
    ClassDB::bind_method(D_METHOD("try_parse", "p_content"), &TomlParser::try_parse);
    ClassDB::bind_method(D_METHOD("get_str", "p_key"), &TomlParser::get_str);
    ClassDB::bind_method(D_METHOD("get_arr", "p_key"), &TomlParser::get_arr);
    ClassDB::bind_method(D_METHOD("get_section", "p_key"), &TomlParser::get_section);
    ClassDB::bind_method(D_METHOD("get_tables", "p_key"), &TomlParser::get_tables);
}

TomlParser::TomlParser() = default;

TomlParser::~TomlParser() = default;

//String(msg.c_str()) std::string to String

// Dictionary d = Dictionary();
// // d.set(Variant("something"), Variant("other"));
// d[Variant("something")] = Variant("other");
// return d;

// for (const auto& pair : edges) {
//     std::cout << pair.first;
// }

std::string TomlParser::to_str(const String &p_string) {
    return p_string.utf8().get_data();
}

bool TomlParser::try_parse(const String &p_content) {
    const auto parse_result = toml::try_parse_str(to_str(p_content));

    if (parse_result.is_ok()) {
        t = parse_result.unwrap();
        return true;
    }

    const auto err = parse_result.unwrap_err().at(0);
    UtilityFunctions::print(String(format_error(err).c_str()));
    return false;
}

String TomlParser::get_str(const String &p_key) {
    if (const std::string key = to_str(p_key); t.contains(key) && t.at(key).is_string()) {
        return {t.at(key).as_string().c_str()};
    }
    return "";
}

Array TomlParser::get_arr(const String &p_key) {
    const std::string key = to_str(p_key);
    auto arr = Array();
    if (t.contains(key) && t.at(key).is_array()) {
        const std::vector<toml::value> vec = t.at(key).as_array();
        for (const auto& value : vec) {
            if (value.is_boolean()) {
                arr.append(value.as_boolean());
            }
            if (value.is_integer()) {
                arr.append(value.as_integer());
            }
            // string?
        }
        return arr;
    }
    return arr;
}

Array TomlParser::get_tables(const String &p_key) {
    // godot::UtilityFunctions::print("TomlParser::get_tables for: "+p_key);
    const std::string key = to_str(p_key);
    auto arr = Array();
    if (t.contains(key) && t.at(key).is_array()) {
        // godot::UtilityFunctions::print("TomlParser::get_tables Found "+p_key);
        const std::vector<toml::value> vec = t.at(key).as_array();
        for (const auto& value : vec) {
            arr.append(to_dictionary(value));
        }
    }
    return arr;    
}

Dictionary TomlParser::get_section(const String &p_key) {
//    godot::UtilityFunctions::print("TomlParser::get_section");
    const std::string key = to_str(p_key);
    if (!t.contains(key)) {
        return {};
    }
    if (!t.at(key).is_table()) {
        return {};
    }
    const auto& section = toml::find(t, key);
    return to_dictionary(section);
}

Dictionary TomlParser::to_dictionary(const toml::value &val) {
//    godot::UtilityFunctions::print("to_dictionary");
    auto dic = Dictionary();
    for(const auto& [k, v] : val.as_table()) {
        auto label = String(k.c_str());
//        godot::UtilityFunctions::print("key: " + label);

        if (v.is_boolean()) {
            dic[label] = Variant(v.as_boolean());
        }
        if (v.is_integer()) {
            dic[label] = Variant(v.as_integer());
        }
        if (v.is_floating()) {
            dic[label] = Variant(v.as_floating());
        }
        if (v.is_string()) {
            dic[label] = Variant(String(v.as_string().c_str()));
        }
        if (v.is_array()) {
            auto arr = Array();
            std::vector<toml::value> vec = v.as_array();
            for (const auto& value : vec) {
                if (value.is_boolean()) {
                    arr.append(value.as_boolean());
                }
                if (value.is_integer()) {
                    arr.append(value.as_integer());
                }
                if (value.is_floating()) {
                    arr.append(value.as_floating());
                }
                if (value.is_string()) {
                    arr.append(Variant(String(value.as_string().c_str())));
                }
            }
            dic[label] = Variant(arr);
        }
        if (v.is_table()) {
//            godot::UtilityFunctions::print("key: " + label + " is a table");
            dic[label] = to_dictionary(v);
        }
    }
    return dic;
}