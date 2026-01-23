#ifndef GDEX_TOML_TRANSLATIONS_H
#define GDEX_TOML_TRANSLATIONS_H
#include "toml.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

inline std::string to_str(const String &p_string) {
    return p_string.utf8().get_data();
}

inline toml::value enc_int(int p_value) {
    return toml::value{p_value};
}

inline toml::value enc_float(float p_value) {
    return toml::value{p_value};
}

inline toml::value enc_string(const String &p_value) {
    return toml::value{to_str(p_value)};
}

inline toml::value enc_bool(bool p_value) {
    return toml::value{p_value};
}

inline toml::value enc_color(const Color &p_value) {
    return toml::array{p_value.r, p_value.g, p_value.b, p_value.a};
}

inline toml::value enc_vector2(const Vector2 &p_value) {
    return toml::array{p_value.x, p_value.y};
}

inline toml::value enc_vector2i(const Vector2i &p_value) {
    return toml::array{p_value.x, p_value.y};
}

inline toml::value enc_vector3(const Vector3 &p_value) {
    return toml::array{p_value.x, p_value.y,p_value.z};
}

inline toml::value enc_vector3i(const Vector3i &p_value) {
    return toml::array{p_value.x, p_value.y, p_value.z};
}

inline toml::value enc_vector4(const Vector4 &p_value) {
    return toml::array{p_value.x, p_value.y,p_value.z, p_value.w};
}

inline toml::value enc_vector4i(const Vector4i &p_value) {
    return toml::array{p_value.x, p_value.y, p_value.z, p_value.w};
}

inline toml::value enc(const Variant &p_value) {
    if (p_value.get_type() == Variant::INT) {
        return enc_int(p_value);
    }
    if (p_value.get_type() == Variant::FLOAT) {
        return enc_float(p_value);
    }
    if (p_value.get_type() == Variant::STRING) {
        return enc_string(p_value);
    }
    if (p_value.get_type() == Variant::BOOL) {
        return enc_bool(p_value);
    }
    if (p_value.get_type() == Variant::COLOR) {
        return enc_color(p_value);
    }
    if (p_value.get_type() == Variant::VECTOR2) {
        return enc_vector2(p_value);
    }
    if (p_value.get_type() == Variant::VECTOR2I) {
        return enc_vector2i(p_value);
    }
    if (p_value.get_type() == Variant::VECTOR3) {
        return enc_vector3(p_value);
    }
    if (p_value.get_type() == Variant::VECTOR3I) {
        return enc_vector3i(p_value);
    }
    if (p_value.get_type() == Variant::VECTOR4) {
        return enc_vector4(p_value);
    }
    if (p_value.get_type() == Variant::VECTOR4I) {
        return enc_vector4i(p_value);
    }
    return {"[Could not find encoder for variant]"};
}

inline int dec_int(const toml::value &p_val) {
    #ifdef DEBUG_TOML
    if (!p_val.is_integer()) {
        UtilityFunctions::print("[ERROR] dec_int : Value is not an integer");

    }
    #endif
    return static_cast<int>(p_val.as_integer());
}

inline int64_t dec_long(const toml::value &p_val) {
    #ifdef DEBUG_TOML
    if (!p_val.is_integer()) {
        UtilityFunctions::print("[ERROR] dec_int : Value is not an integer");

    }
    #endif
    return p_val.as_integer();
}

inline float dec_float(const toml::value &p_val) {
    #ifdef DEBUG_TOML
    if (!p_val.is_floating()) {
        UtilityFunctions::print("[ERROR] dec_float : Value is not a float");
    }
    #endif
    return static_cast<float>(p_val.as_floating());
}

inline String dec_string(const toml::value &p_val) {
    #ifdef DEBUG_TOML
    if (!p_val.is_string()) {
        UtilityFunctions::print("[ERROR] dec_string : Value is not a string");
    }
    #endif
    return {p_val.as_string().c_str()};
}

#endif //GDEX_TOML_TRANSLATIONS_H
