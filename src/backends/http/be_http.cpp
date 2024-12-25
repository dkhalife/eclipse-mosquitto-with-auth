#include "be_http.h"

#include <mosquitto.h>

constexpr const char* c_http_use_https = "use_https";
constexpr const char* c_http_host = "http_host";
constexpr const char* c_http_port = "http_port";
constexpr const char* c_http_auth_path = "http_auth_path";
constexpr const char* c_http_acl_path = "http_acl_path";

BE_Http::BE_Http(const std::map<std::string, std::string>& options)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: backend %s initializing", BE_Http::kind);

    setupBaseUri(options);
    setupSubpaths(options);
}

void BE_Http::setupBaseUri(const std::map<std::string, std::string>& options) noexcept
{
    std::string base_uri;
    if (options.count(c_http_use_https) != 0 && options.at(c_http_use_https) == "true")
    {
        base_uri = "https://";
    }
    else
    {
        base_uri = "http://";
    }

    if (options.count(c_http_host) != 0)
    {
        base_uri.append(options.at(c_http_host));
    }
    else
    {
        base_uri.append("localhost");
    }

    if (options.count(c_http_port) != 0)
    {
        base_uri.append(":" + options.at(c_http_port));
    }

    mosquitto_log_printf(MOSQ_LOG_INFO, "*** auth-plugin: using base uri = %s", base_uri.c_str());
    m_base_uri = std::move(base_uri);
}

void BE_Http::setupSubpaths(const std::map<std::string, std::string>& options) noexcept
{
    if (options.count(c_http_auth_path) == 0)
    {
        mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: required config `%s` is missing", c_http_auth_path);
        return;
    }

    if (options.count(c_http_auth_path) != 0)
    {
        m_auth_path = options.at(c_http_auth_path);
    }

    if (options.count(c_http_acl_path) != 0)
    {
        m_acl_path = options.at(c_http_acl_path);
    }
}

bool BE_Http::authenticate(const std::string& username, const std::string& password)
{
    return false;
}
