#include "plugin.h"

#include <algorithm>
#include <iostream>
#include <mosquitto.h>
#include <sstream>
#include <string>

constexpr const char* c_backends_opt_key = "backends";

Plugin::Plugin(mosquitto_plugin_id_t* identifier, std::map<std::string, std::string> options)
    : m_identifier(identifier)
    , m_options(std::move(options))
{
    initializeBackends();
    registerEvents();
}

Plugin::~Plugin()
{
    unregisterEvents();
}

void Plugin::initializeBackends() noexcept
{
    if (m_options.count(c_backends_opt_key) == 0)
    {
        mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: required `%s` config is missing", c_backends_opt_key);
        return;
    }

    const std::string& value = m_options[c_backends_opt_key];
    mosquitto_log_printf(MOSQ_LOG_INFO, "*** auth-plugin: initializing backends: `%s`", value.c_str());

    std::string kind;
    std::stringstream ss(value);
    while (std::getline(ss, kind, ','))
    {
        mosquitto_log_printf(MOSQ_LOG_INFO, "*** auth-plugin: initializing backend: `%s`", kind.c_str());

        std::unique_ptr<IBackend> backend = BackendFactory(kind, m_options);
        if (backend == nullptr)
        {
            mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: unknown backend kind: `%s`", kind.c_str());
            continue;
        }

        m_backends.push_back(std::move(backend));
    }

    mosquitto_log_printf(MOSQ_LOG_INFO, "*** auth-plugin: done initializing backends");
}

void Plugin::registerEvents() noexcept
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: registering events");
    
    int hr = mosquitto_callback_register(m_identifier, MOSQ_EVT_BASIC_AUTH, Plugin::onEvent, nullptr, this);
    if (hr != MOSQ_ERR_SUCCESS)
    {
        mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: unable to register for basic auth events, hr = %s", hr);
    }
}

void Plugin::unregisterEvents() noexcept
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: unregistering events");
    
    int hr = mosquitto_callback_unregister(m_identifier, MOSQ_EVT_BASIC_AUTH, Plugin::onEvent, nullptr);
    if (hr != MOSQ_ERR_SUCCESS)
    {
        mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: unable to unregister basic auth  callback");
    }
}

int Plugin::onEvent(int event_id, void* event_data, void* user_data) noexcept
{
    Plugin* self = reinterpret_cast<Plugin*>(user_data);

    if (event_id == MOSQ_EVT_BASIC_AUTH)
    {
        mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: received a basic auth event");
	    mosquitto_evt_basic_auth* ed = reinterpret_cast<mosquitto_evt_basic_auth*>(event_data);
        return self->onBasicAuth(*ed);
    }
    else
    {
        mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: received an unexpected event, event_id = %i", event_id);
        return MOSQ_ERR_UNKNOWN;
    }

    return MOSQ_ERR_SUCCESS;
}

int Plugin::onBasicAuth(const mosquitto_evt_basic_auth& event_data) noexcept
{
    for (auto& backend: m_backends)
    {
        if (backend->authenticate(event_data.username, event_data.password))
        {
            return MOSQ_ERR_SUCCESS;
        }
    }

    return MOSQ_ERR_AUTH;
}
