#pragma once

#include "../backend.h"

class BE_Passwd: public IBackend
{
public:
    BE_Passwd(const std::vector<mosquitto_opt>& options);

    bool Authenticate(const std::string& username, const std::string& password);

    static constexpr const char* kind = "passwd";
};
