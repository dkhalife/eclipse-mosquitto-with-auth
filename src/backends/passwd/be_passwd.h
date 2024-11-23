#pragma once

#include "../backend.h"

class BE_Passwd: public IBackend
{
public:
    BE_Passwd(const std::vector<mosquitto_opt>& options);

    static constexpr const char* kind = "passwd";
};
