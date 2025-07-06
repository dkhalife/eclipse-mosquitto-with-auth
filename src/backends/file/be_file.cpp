#include "be_file.h"

#include <filesystem>
#include <fstream>
#include <mosquitto.h>
#include <sstream>
#include <string>

constexpr const char* c_file_opt_key = "creds_file";
constexpr const char* c_debug_auth_key = "debug_auth";

BE_File::BE_File(const std::map<std::string, std::string>& options)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: backend %s initializing", BE_File::kind);
    if (!initialize(options))
    {
        mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: initialization failed");
    }
}

bool BE_File::initialize(const std::map<std::string, std::string>& options)
{
    if (options.count(c_file_opt_key) == 0)
    {
        mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: required config `%s` is missing", c_file_opt_key);
        return false;
    }

    auto it = options.find(c_debug_auth_key);
    m_debug_auth = (it != options.end() && it->second == "true");

    const std::string& credentialsFilePath = options.at(c_file_opt_key);
    auto credentials = loadFile(credentialsFilePath);
    if (credentials.has_value())
    {
        m_credentials = std::move(credentials.value());
        return true;
    }

    return false;
}

std::optional<std::vector<std::pair<std::string, std::string>>> BE_File::loadFile(const std::string& filePath)
{
    std::vector<std::pair<std::string, std::string>> credentials;

    if (!std::filesystem::exists(filePath))
    {
        mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: file not found: `%s`", filePath.c_str());
        return std::nullopt;
    }

    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: loading credentials from `%s`", filePath.c_str());

    auto file = std::ifstream(filePath);
    std::string line;
    int lineNb = 1;
    while (getline(file, line))
    {
        int iSep = line.find_last_of("::");
        if (iSep == std::string::npos)
        {
            mosquitto_log_printf(MOSQ_LOG_WARNING, "*** auth-plugin: line %i is malformed, skipping it", lineNb);
            continue;
        }

        std::string username = line.substr(0, iSep - 1); // -1 brings us back to the beginning of the separator
        size_t remaining = line.size() - username.size() - 3U; // 2 chars for the separator and 1 for end of line
        std::string password = line.substr(iSep + 1, remaining);
        credentials.emplace_back(make_pair(std::move(username), std::move(password)));

        ++lineNb;
    }

    mosquitto_log_printf(MOSQ_LOG_INFO, "*** auth-plugin: loaded %i credentials from `%s`", credentials.size(), filePath.c_str());

    return credentials;
}

bool BE_File::authenticate(const std::string& username, const std::string& password, const std::string& /*client_id*/)
{
    SHA256 hasher;
    std::string input_hash = hasher(password);

    if (m_debug_auth)
    {
        mosquitto_log_printf(MOSQ_LOG_DEBUG,
                             "*** auth-plugin: username=%s password=%s",
                             username.c_str(), input_hash.c_str());
    }

    for (const auto& item: m_credentials)
    {
        if (item.first == username && item.second == input_hash)
        {
            return true;
        }
    }

    return false;
}

bool BE_File::reload(const std::map<std::string, std::string>& options)
{
    return initialize(options);
}

bool BE_File::checkAcl(const std::string& /*client_id*/, const std::string& /*topic*/, int /*access*/)
{
    return true;
}
