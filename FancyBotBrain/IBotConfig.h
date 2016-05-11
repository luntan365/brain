#pragma once

#include "json.hpp"

class IBotConfig {
public:
    virtual ~IBotConfig ()
    {
    }

    virtual nlohmann::json ToJson() = 0;

    virtual bool FromJson(const nlohmann::json& json) = 0;
};
