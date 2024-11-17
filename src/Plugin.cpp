#include "plugin.h"

#include <algorithm>
#include <mosquitto.h>
#include <ranges>
#include <string>
#include <string_view>

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

    const std::string& delim = ",";
    const auto tokens = value 
                    | std::views::split(delim)
                    | std::ranges::to<std::vector<std::string_view>>();
    for (const auto& kind: tokens) {
        mosquitto_log_printf(MOSQ_LOG_INFO, "*** auth-plugin: initializing backend: `%s`", kind);

        std::unique_ptr<IBackend> backend = BackendFactory(kind, m_options);
        if (backend != nullptr)
        {
            m_backends.push_back(std::move(backend));
        }
    }
}
