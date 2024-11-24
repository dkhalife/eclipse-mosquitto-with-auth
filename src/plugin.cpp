#include "plugin.h"

#include <algorithm>
#include <iostream>
#include <mosquitto.h>
#include <ranges>
#include <sstream>
#include <string_view>
#include <string>

constexpr const char* c_backends_opt_key = "backends";

Plugin::Plugin(mosquitto_plugin_id_t* identifier, std::vector<mosquitto_opt> options)
    : m_identifier(identifier)
    , m_options(std::move(options))
{
    initializeBackends();
}

Plugin::~Plugin()
{
}

void Plugin::initializeBackends() noexcept
{
    auto backends_opt = std::find_if(m_options.begin(), m_options.end(), [](const mosquitto_opt& opt) noexcept
    {
        return std::string(c_backends_opt_key).compare(opt.key) == 0;
    });

    if (backends_opt == m_options.end())
    {
        mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: required `%s` config is missing", c_backends_opt_key);
        return;
    }

    const std::string& value = backends_opt->value;

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
}

int Plugin::onBasicAuth(const std::string& username, const std::string& password) noexcept
{
    for (auto& backend: m_backends)
    {
        if (backend->Authenticate(username, password))
        {
            return MOSQ_ERR_SUCCESS;
        }
    }

    return MOSQ_ERR_AUTH;
}
