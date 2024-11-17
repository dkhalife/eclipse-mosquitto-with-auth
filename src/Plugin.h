#pragma once

#include <mosquitto_plugin.h>
#include <vector>

class Plugin
{
public:
    Plugin(mosquitto_plugin_id_t* identifier, std::vector<mosquitto_opt> options);
    ~Plugin();

private:
    std::vector<mosquitto_opt> m_options;
    mosquitto_plugin_id_t* m_identifier;
};
