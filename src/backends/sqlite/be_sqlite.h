#pragma once

#include "../backend.h"

class BE_Sqlite: public IBackend
{
public:
    BE_Sqlite(const std::vector<mosquitto_opt>& options);

    static constexpr const char* kind = "sqlite";
};
