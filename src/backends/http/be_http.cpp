#include "be_http.h"

#include <mosquitto.h>
#include <sstream>

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

    initCurl();
}

BE_Http::~BE_Http()
{
    if (m_curl)
    {
        curl_easy_cleanup(m_curl);
    }

    curl_global_cleanup();
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

void BE_Http::initCurl() noexcept
{
    curl_global_init(CURL_GLOBAL_ALL);
    m_curl = curl_easy_init();

    m_headers = curl_slist_append(m_headers, "Content-Type: application/json");
}

bool BE_Http::authenticate(const std::string& username, const std::string& password, const std::string& client_id)
{
    if (!m_curl)
    {
        mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: curl was not initialized");
        return false;
    }

    std::ostringstream url;
    url << m_base_uri << m_auth_path;

	char* escaped_username = curl_easy_escape(m_curl, username.c_str(), 0);
	char* escaped_password = curl_easy_escape(m_curl, password.c_str(), 0);
	char* escaped_clientid = curl_easy_escape(m_curl, client_id.c_str(), 0);

    std::ostringstream data;
    data << "username=" << escaped_username << "&password=" << escaped_password << "&clientid=" << escaped_clientid;

    curl_easy_setopt(m_curl, CURLOPT_URL, url.str().c_str());
	curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
	curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.str().c_str());
    curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headers);
	curl_easy_setopt(m_curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	curl_easy_setopt(m_curl, CURLOPT_USERNAME, username.c_str());
	curl_easy_setopt(m_curl, CURLOPT_PASSWORD, password.c_str());
	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 5);

    CURLcode res = curl_easy_perform(m_curl);

	free(escaped_username);
	free(escaped_password);
	free(escaped_clientid);

    if(res == CURLE_OK)
    {
        long response_code = 500;
        curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &response_code);
        return response_code == 200;
    }

    return false;
}

bool BE_Http::reload(const std::map<std::string, std::string>& options)
{
    setupBaseUri(options);
    setupSubpaths(options);

    return true;
}

bool BE_Http::checkAcl(const std::string& /*client_id*/, const std::string& /*topic*/, int /*access*/)
{
    return false;
}
