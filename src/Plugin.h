#pragma once

#include "backends/backend.h"

#include <mosquitto_plugin.h>
#include <vector>

class Plugin
{
public:
    Plugin(mosquitto_plugin_id_t* identifier, std::vector<mosquitto_opt> options);
    ~Plugin();

    int onBasicAuth(const std::string& username, const std::string& password) noexcept;

private:
    void initializeBackends() noexcept;

    std::vector<mosquitto_opt> m_options;
    mosquitto_plugin_id_t* m_identifier;

    std::vector<std::unique_ptr<IBackend>> m_backends;
};
