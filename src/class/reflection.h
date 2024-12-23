#ifndef _REFLECT_H_
#define _REFLECT_H_
#include <iostream>
#include <nlohmann/json.hpp>
#include <rttr/type>

nlohmann::json to_json(const rttr::instance& obj) {
    nlohmann::json j;
    rttr::type t = obj.get_type();

    for (const auto& prop : t.get_properties()) {
        rttr::variant val = prop.get_value(obj);
        if (val.is_valid()) {
            j[prop.get_name().to_string()] = val.to_string();
        }
    }
    return j;
}

void from_json(const nlohmann::json& j, rttr::instance& obj) {
    rttr::type t = obj.get_type();

    for (const auto& prop : t.get_properties()) {
        if (j.contains(prop.get_name().to_string())) {
            rttr::variant val = prop.get_value(obj);
            if (val.is_valid() && val.can_convert(prop.get_type())) {
                prop.set_value(obj, j[prop.get_name().to_string()]);
            }
        }
    }
}

#endif