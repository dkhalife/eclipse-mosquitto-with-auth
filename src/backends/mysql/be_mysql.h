#pragma once

#include "../backend.h"

class BE_Mysql: public IBackend
{
public:
    BE_Mysql(const std::vector<mosquitto_opt>& options);

    static constexpr const char* kind = "mysql";
};
