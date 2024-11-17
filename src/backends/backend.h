#pragma once

#include <memory>
#include <mosquitto_plugin.h>
#include <string_view>
#include <vector>

class IBackend
{
public:
    virtual ~IBackend(){}
};

std::unique_ptr<IBackend> BackendFactory(std::string_view kind, const std::vector<mosquitto_opt>& options);
