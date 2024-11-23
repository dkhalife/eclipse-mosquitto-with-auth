#pragma once

#include "../backend.h"

class BE_Mysql: public IBackend
{
public:
    BE_Mysql(const std::vector<mosquitto_opt>& options);

    bool Authenticate(const std::string& username, const std::string& password);

    static constexpr const char* kind = "mysql";
};
