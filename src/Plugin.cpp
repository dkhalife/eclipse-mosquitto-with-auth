#include "Plugin.h"

Plugin::Plugin(mosquitto_plugin_id_t* identifier, std::vector<mosquitto_opt> options)
    : m_identifier(identifier)
    , m_options(options)
{
}

Plugin::~Plugin()
{
}
